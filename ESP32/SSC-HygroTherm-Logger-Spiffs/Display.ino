

  // AIR_GOOD 0
  // AIR_WET 1
  // AIR_DRY 2
  // AIR_NOTHING 3
  
void mainScreen(){
  
  display.clear();
  // battery
  display.drawRect(101, 1, 24, 9);//außen
  display.fillRect(125,3,2,5);// spitze
  display.fillRect(103, 3, batteryLevel, 5); 
  if(aircondition == AIR_GOOD) display.drawXbm(101,25, 24, 24, d_Good);
  if(aircondition == AIR_WET) display.drawXbm(101,25, 24, 24, d_Wet);
  // temperatur and hum   
  display.setFont(ArialMT_Plain_24); 
  display.drawString(0, 0, strTemp);
  display.drawString(0, 25, strHum);
  // time
  display.setFont(ArialMT_Plain_10);
 
  String timestring = String(local.tm_mday)+"."
                     +String(1 + local.tm_mon)+"."
                     +String(1900+local.tm_year)+" - "
                     +String(local.tm_hour)+":"
                     +String(local.tm_min)+":"
                     +String(local.tm_sec);
                     //+String(lipoVoltage,2);
                     
  display.drawString(0, 50, timestring);
  display.display(); 
}


void tpScreen(){
  
  display.clear();
  // battery
  display.drawRect(101, 1, 24, 9);//außen
  display.fillRect(125,3,2,5);// spitze
  display.fillRect(103, 3, batteryLevel, 5);
  // temperatur and hum   
  display.setFont(ArialMT_Plain_24); 
  display.drawString(0, 0, strDawPoint);
  display.drawString(0, 25, strHumABS);
  // time
  display.setFont(ArialMT_Plain_10);
  
//  String timestring = String(mynow.day())+"."
//                     +String(mynow.month())+"."
//                     +String(mynow.year())+" - "
//                     +String(mynow.hour())+":"
//                     +String(mynow.minute())+":"
//                     +String(mynow.second());
  
  String strLipoVoltage = "Lipo Volts: " + String(lipoVoltage ,2);
                     
  display.drawString(0, 50, strLipoVoltage);
  display.display(); 
}

void warnSleepScreen(){
  
  display.clear(); 
  display.setFont(ArialMT_Plain_24); 
  display.drawString(13, 0, "LÜFTEN!");
  display.drawXbm(55,30, 24, 24, d_Wet);
  display.display(); 
}
