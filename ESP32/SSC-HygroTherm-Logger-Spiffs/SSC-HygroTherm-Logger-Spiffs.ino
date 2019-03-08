
/*
 *                      ein auf dem Arduino Projekt basierender
 *
 *              > > > Stromspar-Check (SSC) HYGROTHERM-DATALOGGER < < <
 *
 *          Copyright 2018 bei Michael Wolkstein (Stromsparhelfer beim SSC Bielefeld)
 *
 * Der SSC HYGROTHERM-DATALOGGER ist für Menschen im Transferleistungsbezug und
 * möglicher weise schwierigen Mietverhältnissen. Er soll helfen Schimmelbildung
 * und zu hohe Heizkosten durch falsches Lüften und Heizen zu vermeiden.
 * Das Gerät dient dazu dem Mietern anzuzeigen ob die von SSC und den Mietern erdachten
 * Maßnahmen zum besseren Lüftungs und Heizverhalten funktionieren, oder ob die Maßnahmen
 * gegebenfalls angepasst und verbessert werden können/müssen. Die Mieter lernen dabei
 * das Raumklima besser zu verstehen und ihr Heiz und Lüftungs- Verhalten entsprechend zu verbessern.
 * Desweiteren kann die detalierte Auswertung der vom Datenlogger erfassten Daten helfen die
 * SSC Wärmemaßnahmen individuell auf den Haushalt anzupassen.
 *
 * Langfristig betrachte hilt richtiges Heizen und Lüften "Heizenergie" EINZUSPAREN! Komunen werden
 * so finanziell bei den "Kosten für die die Unterkunft" etlastet. Am meisten Profitiert allerdings unsere
 * Umwelt. Wenn alleine durch richtiges heitzen und lüften pro Haushalt bis zu 20% Heizenergie eingesparrt
 * werden kann und man dabei rechnet das ca. 70% des Gesamtenergiebedarfs eines Haushaltes alleine für Wärmeenergie
 * aufgebracht werden, ergibt sich grob eine bis zu 14% Einsparung des gesamten im Haushalt anfallenden Energiebedarfs!
 * Ich denke das ist schon nicht schlecht:).
 *
 * WICHTIG! DAS IST EIN DATENLOGGER FÜR MIETER... - NICHT etwa ein KOSTENGÜNSTIGES WERKZEUG für VERMIETER
 * Dieses Projekt dient nicht dazu falsches Heiz und Lüftungsverhalten bei Mietern
 * zu entlarven und diesen anzulasten. Es soll eben genau vermeiden das Vermierter ihren Mietern
 * falsches Heiz- und Lüftungs- Verhalten zu Last legen können um dann ihrerseits den Mietern
 * anfallende Folgekosten aufzubrummen.
 *
 * Funktionen:
    * Temperatur, Relative Luftfeuchte und Luftdruck messen
    * Taupunkt rechnerisch ermitteln und Anzeigen
    * Langzeitverlauf der Luftfeuchtigkeit anzeigen um zu visualisieren wie es dem Mieter
      gelingt die Luftfeuchtigkeit durch die angewendeten Maßnahmen zu regulieren
    * Einstellen des Gewünschten Temperatur- und Luftfeuchte- Niveau
    * SD Karten Temperatur und Luftfeuchte Datenlogger zur späteren detalierten Datenanalyse
 *
 *
 */

// ############### Libraries ######################
#include "bitmaps.h"
//#include "RTClib.h"                   // R-T-C lib
#include <Wire.h>
#include <WiFi.h>
#include <BME280I2C.h>
#include <EnvironmentCalculations.h>
#include "SSD1306.h"
#include <FIR.h>
#include <Preferences.h>
#include "FS.h"
#include "SPIFFS.h"
#include "SPI.h"


WiFiServer server(80);

#define ADC_PIN 35 //35(adc1_7) 13(adc2_4), 27(adc2_7)
#define ADC_DIVIDER 645//623.0 adc1_7 , 993.932038835 adc2_4, 772.81 adc1_1
//###################### PREFERENCES #############
Preferences preferences;
int hg_Mid = 50;

//###################### DEEP SLEEP #############
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  3        /* Time ESP32 will go to sleep (in seconds) */


//##################### SPIFFS ##############
#define FORMAT_SPIFFS false

//##################### DEBUG ###############

// if deb rtc is set, we connect every boot to ntp server
#define DEB_RTC false

#define DEB_BTN false
#define DEB_BME false
#define DEB_FIR false
#define DEB_BAT false
#define DEB_LSL false
#define DEB_SPIFFS true
#define DEB_WIFI true
#define DEB_SERVER true

bool setupdone = false;

//#####################LOGGER ###############
String loggerString = "";
String currentDataFile = "";

//##################### Fir Filter ##########
FIR<float, 8> firHum;
float coefAvrHum[8] = { 1., 1., 1., 1., 1., 1., 1., 1.};// For a moving average we want all of the coefficients to be unity.

FIR<int, 32> firBAT_DAC;
int coefAvrBAT_DAC[32] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};// For a moving average we want all of the coefficients to be unity.
float lipoVoltage = 0.0f;
int batteryLevel = 0;
//###################### RTC ################
// nothing since we use internal clock
struct tm local;


String testvar = "";
//###################### DISPLAY ################
#define SDA    4
#define SCL   15
#define RST   16 //RST must be set by software

#define V2  1

#ifdef V2 //WIFI Kit series V1 not support Vext control
  #define Vext  21
#endif

#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH  128

SSD1306  display(0x3c, SDA, SCL, RST);

bool displayIsOn = true;

int selectedDisplay = 0; // 0 = main screen, 1 = dawpoint
bool onServerDisplay = false;

#define AIR_GOOD 0
#define AIR_WET 1
#define AIR_DRY 2
#define AIR_NOTHING 3
int aircondition =0;
//###################DISPLAY END#####################

#define NTP_SERVER0 "0.de.pool.ntp.org"
#define NTP_SERVER1 "1.de.pool.ntp.org"
#define NTP_SERVER2 "2.de.pool.ntp.org"
#define NTP_SERVER3 "3.de.pool.ntp.org"
#define NTP_SERVER4 "ntp.mydomain.com"

#define TZ_INFO "WEST-1DWEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00"
// Western European Time



// Temp und Hum sensor config
 BME280I2C::Settings settings(
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::Mode_Forced,
   BME280::StandbyTime_1000ms,
   BME280::Filter_16,
   BME280::SpiEnable_False,
   0x76 // I2C address. I2C specific.
);

 BME280I2C bme(settings);
// More BME 280 Configs
 float temp(NAN), hum(NAN), pres(NAN);
 float myDawPoint = 0.0f;
 float absHUM = 0.0f;
 BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
 BME280::PresUnit presUnit(BME280::PresUnit_hPa);
 EnvironmentCalculations::TempUnit     envTempUnit =  EnvironmentCalculations::TempUnit_Celsius;


 String strTemp ="";
 String strHum ="";
 String strTime = "";
 String strDawPoint = "";
 String strHumABS = "";

 bool internalbuttonOld = false;
 bool internalbuttonRise = false;
 bool internalbuttonRose = false;

 unsigned long lastbtnRose = 0;


// slow loop logger 10 - 60 seconds
// mid loop BME280 1000ms
// fast loop Buttons 50ms with delay()
unsigned long displaytime = 720 * 100;
unsigned long lastdisplayt = 0;
unsigned long slowtime = 60000; // 60 seconds
unsigned long lastslow = 0;
unsigned long midtime =  1000;
unsigned long lastmid = 0;
unsigned long looptime = 0;

bool isSpiffs = false;
bool spiffsFail = false;
bool warnscrentoggel = false;

float loopHum=0.0f;

void enableMyDisplay(){
  display.displayOn();
}


void processBTN(){
  bool state = digitalRead(0);

  if( state == 0 && internalbuttonOld == 1 ){// Button Pressed
    internalbuttonRose = true;
  }

  if( state == 1 && internalbuttonOld == 0 ){// Button Relised
    internalbuttonRise = true;
  }

  internalbuttonOld = state;
}

bool getBTNRose(){
  bool state = internalbuttonRose;
  internalbuttonRose = false;
  return state;
}

bool getBTNRise(){
  bool state = internalbuttonRise;
  internalbuttonRise = false;
  return state;
}


void setRTC()
{
  display.clear();
  display.drawString(1, 30, "Suche-Wifi");
  display.setFont(ArialMT_Plain_16);
  display.drawString(1, 5, "SSC-AP : 4x01");
  display.display();
  WiFi.mode(WIFI_STA);
  WiFi.begin("SSC-AP", "01010101");
  if(DEB_WIFI) Serial.println("");

  int upper = 0;
  display.setFont(ArialMT_Plain_10);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    if(DEB_WIFI) Serial.print(".");
    display.drawString(10 + upper, 40, "o");
    display.display();
    upper += 5;
  }
  if(DEB_WIFI) Serial.println("");
  if(DEB_WIFI) Serial.print("IP Addresse: ");
  if(DEB_WIFI) Serial.println(WiFi.localIP());
  if(DEB_WIFI) Serial.println("Hole NTP Zeit");

  // try different server
  configTzTime(TZ_INFO, NTP_SERVER0); // ESP32 Systemzeit mit NTP Synchronisieren
  getLocalTime(&local, 20000);      // Versuche 10 s zu Synchronisieren
  if(local.tm_year == 70 ){
    if(DEB_WIFI) Serial.println("Hmm?, 1 neuer versuch Hole NTP Zeit");
    configTzTime(TZ_INFO, NTP_SERVER1); // ESP32 Systemzeit mit NTP Synchronisieren
    getLocalTime(&local, 10000);      // Versuche 10 s zu Synchronisieren
  }
  if(local.tm_year == 70 ){
    if(DEB_WIFI) Serial.println("Hmm?, 2 neuer versuch Hole NTP Zeit");
    configTzTime(TZ_INFO, NTP_SERVER2); // ESP32 Systemzeit mit NTP Synchronisieren
    getLocalTime(&local, 10000);      // Versuche 10 s zu Synchronisieren
  }
  if(local.tm_year == 70 ){
    if(DEB_WIFI) Serial.println("Hmm?, 3 neuer versuch Hole NTP Zeit");
    configTzTime(TZ_INFO, NTP_SERVER3); // ESP32 Systemzeit mit NTP Synchronisieren
    getLocalTime(&local, 10000);      // Versuche 10 s zu Synchronisieren
  }
  if(local.tm_year == 70 ){
    if(DEB_WIFI) Serial.println("Hmm?, 4 neuer versuch Hole NTP Zeit");
    configTzTime(TZ_INFO, NTP_SERVER4); // ESP32 Systemzeit mit NTP Synchronisieren
    getLocalTime(&local, 10000);      // Versuche 10 s zu Synchronisieren
  }
  if(DEB_WIFI) Serial.printf("jahr %04d, monat %02d, tag %02d, %02d:%02d:%02d\n",1900+local.tm_year,1 + local.tm_mon, local.tm_mday , local.tm_hour, local.tm_min , local.tm_sec);

  //rtc.adjust(DateTime(1900+local.tm_year, 1 + local.tm_mon, local.tm_mday , local.tm_hour, local.tm_min , local.tm_sec));// strange? month need +1
  display.clear();
  display.drawString(10, 30, "Stoppe-Wifi & Bt");
  display.display();
  delay(2000);
  WiFi.mode(WIFI_OFF);

  btStop();
}

void setup()
{
  pinMode(0,INPUT_PULLUP);
  pinMode(25, OUTPUT);
  Serial.begin(115200);
  
  adcAttachPin(ADC_PIN);
  //analogSetClockDiv(9);// ~50ms
  
  delay(1000);
  Serial.println("");

  Wire.begin();

  // Preferences
  preferences.begin("ssc-hg", false);
  // Remove all preferences under the opened namespace
  //preferences.clear();
  // Or remove the counter key only
  //preferences.remove("hg_Mid");
  hg_Mid = preferences.getUInt("hg_Mid", 50);
  // Store the hg_Mid to the Preferences
  preferences.putUInt("hg_Mid", hg_Mid);

  // Close the Preferences
  preferences.end();


  display.init();
  display.flipScreenVertically();
  display.setContrast(100);
  display.setBrightness(50);

  display.setFont(ArialMT_Plain_10);
  //display.setTextAlignment(TEXT_ALIGN_RIGHT);
  //display.drawString(10, 30, "SSC-HYGROMETER");
  display.drawXbm(20,0, 64, 64, sscLogo);
  display.display();
  delay(2000);

  // Init Time
  setRTC();

  setenv("TZ", TZ_INFO, 1);
  tzset();



  while(!bme.begin())
  {
    if(DEB_BME) Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
  
  
  // Fir Filter Set the coefficients
  firHum.setFilterCoeffs(coefAvrHum);
  firBAT_DAC.setFilterCoeffs(coefAvrBAT_DAC);
  if(DEB_FIR) Serial.print("Hum Gain set: ");
  if(DEB_FIR) Serial.println(firHum.getGain());
  if(DEB_FIR) Serial.print("BAT_DAC Gain set: ");
  if(DEB_FIR) Serial.println(firBAT_DAC.getGain());



  if(!SPIFFS.begin()){
    if( DEB_SPIFFS ) Serial.println("SPIFFS Mount Failed");
    isSpiffs = false;
  }else{
    if( DEB_SPIFFS ) Serial.println("Mount SPIFFS");
    isSpiffs = true;
  }

  if(isSpiffs){
    if( DEB_SPIFFS ) listDir(SPIFFS, "/",1);
    if( FORMAT_SPIFFS ){
      SPIFFS.format();
      if( DEB_SPIFFS ) listDir(SPIFFS, "/",1);
    }
  }


  WiFi.mode(WIFI_OFF);
  btStop();

  delay(100);

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0,0); //Internal button
  setupdone = true;
  lastbtnRose = millis();
  server.begin();
}


void loop()
{
  looptime = millis();
  getLocalTime(&local);
  // ######## Fast Loop 50ms ###########
  // read battery
  // 4,05 is 2524, div is 623.0 on adc1 pin 35
  // 4,187 is 2722 , divider is 650.107at default analogSetClockDiv
  // 4,182V is 3090.0 ADC Value. my divider is 738.880918 at analogSetClockDiv(9)
  int batteryDac = analogRead(ADC_PIN);
  int batteryDac_Fir = firBAT_DAC.processReading(batteryDac);
  lipoVoltage = float(batteryDac_Fir) / ADC_DIVIDER ;
  if(DEB_BAT) Serial.printf("Lipo on %d vs. Fir %d read %3.2f V\n",batteryDac, batteryDac_Fir,lipoVoltage);


  //read internal button
  processBTN();
  bool btnIsDownPressed = getBTNRose();
  bool btnIsReleased = getBTNRise();

  if(btnIsDownPressed){// button pressed
    if(DEB_BTN) Serial.println("Internal BTN pressed");
    lastbtnRose = millis();
    selectedDisplay++;
    if(selectedDisplay > 1) selectedDisplay = 0;

  }

  if(btnIsReleased){
    if(DEB_BTN) Serial.println("Internal BTN released");
    lastdisplayt = looptime;
    if(!displayIsOn){
      displayIsOn = true;
      enableMyDisplay();
    }
    if(DEB_BTN) Serial.printf("btn long rpress time %d\n", millis()- lastbtnRose);
    if(millis()- lastbtnRose > 4000) onServerDisplay = !onServerDisplay;

  }

  // ######## MIDLOOP ########## 1000 ms
  if(looptime >= lastmid + midtime){

    bme.read(pres, temp, hum, tempUnit, presUnit);
    if(DEB_FIR) Serial.println("P-Fir-Filter");
    loopHum = firHum.processReading(hum);
    float paraA = 7.6f;
    float paraB = 240.7f;
    if( temp >=0.0f) {
      paraA = 7.5f;
      paraB = 237.3f;
    }

    float mySDD =  6.1078*pow(10,((paraA*temp)/(paraB+temp)));
    float myDD = loopHum/100.0*mySDD;

    float helpmeV = log10(myDD/6.1078);
    myDawPoint = (paraB*helpmeV)/(paraA-helpmeV);

    absHUM = (100000.0*18.016/8314.3 * myDD)/(temp + 273.15);//18.016 Molekulargewicht des Wasserdampfes in kg/kmol, 8314.3  # universelle Gaskonstante in J(Kmol*K)

    // AIR_GOOD 0
    // AIR_WET 1
    // AIR_DRY 2
    // AIR_NOTHING 3
    aircondition =AIR_NOTHING;
    if(loopHum > hg_Mid + 10) aircondition = AIR_WET;
    if(loopHum < 40) aircondition = AIR_DRY;
    if(temp > 18 && temp < 25){
      if(loopHum > 40 && loopHum < hg_Mid + 10) aircondition = AIR_GOOD;
    }

    //mynow = rtc.now();
    if(DEB_RTC) Serial.printf("Datum: %02d.%02d.%04d  Zeit: %02d:%02d:%02d\n",1900+local.tm_year,1 + local.tm_mon, local.tm_mday , local.tm_hour, local.tm_min , local.tm_sec);
    if(DEB_BME) Serial.printf("Temperatur: %3.1f°C, Relative Luftfeuchte: %3.0f%% : %3.0f%%, Luftdruck: %3.0fhPa\n", temp,hum,loopHum,pres);

    int bHelper = int(lipoVoltage * 100.01);
    batteryLevel = map(bHelper, 330, 419,0,20);
    if(DEB_BAT) Serial.printf("bLevel: %d, bHelper: %d\n", batteryLevel, bHelper);

    strTemp =  String(temp,1) + "C°";
    strHum = String(loopHum,0) + "%RE";
    strDawPoint =  String(myDawPoint,1) + "C°";
    strHumABS = String(absHUM,1) + "g/m³";
    if(displayIsOn){// update display
      if(!onServerDisplay && selectedDisplay == 0) mainScreen();
      if(!onServerDisplay && selectedDisplay == 1) tpScreen();
      if(onServerDisplay) serverScreen();
    }

    lastmid = looptime;
  }

  // ##### slowloop (logger)######## slowtime = 20 seconds
  if(looptime >= lastslow + slowtime){
    if( DEB_SPIFFS ) Serial.println("Slow Loop -----------------------");
    digitalWrite(25,HIGH);
    if(DEB_SPIFFS) Serial.printf("loopHum befor rtc logger f_call in slowloop %3.2f\n",loopHum);
    rtcLogger(loopHum);
    digitalWrite(25,LOW);
    if(DEB_SPIFFS) Serial.printf("SPIFFS is mounted %d, SPIFFS IO fail: %d\n",isSpiffs, spiffsFail);
    lastslow = looptime;

  }

  // ##### display shutoff 5min ########
  if(!onServerDisplay && displayIsOn && looptime >= lastdisplayt + displaytime ){

    if( DEB_LSL ) Serial.println("Display off");
    display.displayOff();
    displayIsOn = false;
    lastdisplayt = looptime;
    delay(10);
    esp_light_sleep_start();
  }

  if(displayIsOn){
    delay(50);
  }
  else
  {
    long timenow=millis();
    selectedDisplay = 0;
    if(aircondition == AIR_WET && !onServerDisplay){
      if(warnscrentoggel){
        display.displayOn();
        warnSleepScreen();
      }
      else
      {
        display.displayOff();
      }
      warnscrentoggel = !warnscrentoggel;
    }else
    {
      display.displayOff();
    }

    if( DEB_LSL ) Serial.printf("Enter sleep at %d ms one run need %d ms\n",timenow,timenow-looptime);
    esp_light_sleep_start();
  }
}



void serverScreen(){

  display.setFont(ArialMT_Plain_16);
  display.clear();
  display.drawString(1, 30, "Suche-Wifi");
  display.setFont(ArialMT_Plain_16);
  display.drawString(1, 5, "SSC-AP : 4x01");
  display.display();
  WiFi.mode(WIFI_STA);
  WiFi.begin("SSC-AP", "01010101");
  if(DEB_WIFI) Serial.println("");

  int upper = 0;
  display.setFont(ArialMT_Plain_10);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    if(DEB_WIFI) Serial.print(".");
    display.drawString(10 + upper, 40, "o");
    display.display();
    upper += 5;
  }

  if(DEB_WIFI) Serial.println("");
  if(DEB_WIFI) Serial.print("IP Addresse: ");
  if(DEB_WIFI) Serial.println(WiFi.localIP());
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(1, 0, "Adresse:");
  String url = WiFi.localIP().toString().c_str();
  display.drawString(0, 20, url);
  display.display();

  bool serveon = true;
  String myDFileName = "";
  myDFileName += "/DATA_";
  myDFileName += String(local.tm_mday, DEC);
  myDFileName += "-";
  myDFileName += String(1 + local.tm_mon, DEC);
  myDFileName += "-";
  myDFileName += String(1900+local.tm_year, DEC);
  myDFileName += "_";
  myDFileName += String(local.tm_hour, DEC);
  myDFileName += "-";
  myDFileName += String(local.tm_min , DEC);
  myDFileName += ".CSV";

  while (serveon){
  
  if(DEB_SERVER) Serial.println("Wait for client");// print a message out the serial port
    WiFiClient client = server.available();   // listen for incoming clients

    if (client) {                             // if you get a client,
      if(DEB_SERVER) Serial.println("New Client.");           // print a message out the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          if(DEB_SERVER) Serial.write(c);     // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              //test
              String allfiles = "";
              getListOfFileNames(SPIFFS, allfiles, "/", 1);

              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              // the content of the HTTP response follows the header:
              //client.print("Click <a href=\"/DATA.CSV\">here</a> Download Logfile.<br>");

              client.print("<h2>SSC Daten Logger</h2>");
              client.print(allfiles);

              client.print("<form>");
                client.print("<button formaction="+myDFileName+">Download Logfile</button>");
              client.print("</form>");
              client.print("<form>");
                client.print("<button formaction=\"/Delete\">Delete Logfile</button>");
              client.print("</form>");
              client.print("<form>");
                client.print("<button formaction=\"/Stop\">Stop Server!</button>");
              client.print("</form>");

              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            } else {    // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }

          // Check to see if the client request was "GET /H" or "GET /L":
          if (currentLine.endsWith("GET "+ myDFileName +"")) {
            File datafile = SPIFFS.open("/DATA.CSV" , FILE_READ); // Now read data from FS

            if (datafile) {
              if (datafile.available()) { // If data is available and present
                //String dataType = "application/octet-stream";
                client.write(datafile);
                client.println(); //file end
              }
            }
            datafile.close(); // close the file
          }


          if (currentLine.endsWith("GET /Delete")) {
            //digitalWrite(5, LOW);                // GET /L turns the LED off
            deleteFile(SPIFFS,"/DATA.CSV");

          }
          if (currentLine.endsWith("GET /Stop")) {
            client.print("Stoppe den Server und werde wieder ein Hgrometer Daten Logger<br>");
            delay(1000);
            client.stop();
            serveon = false;
          }
        }
      }
      // close the connection:
      client.stop();
    }
  }
  //server.stop();
  WiFi.mode(WIFI_OFF);
  btStop();
  Serial.println("Client Disconnected.");
  onServerDisplay = false;
}
