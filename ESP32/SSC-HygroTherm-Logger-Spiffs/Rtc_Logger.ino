
void rtcLogger(float myLoopHum){


    loggerString = "";
    currentDataFile = "/DATA.CSV";
//    currentDataFile += String(1 + local.tm_mon);
//    currentDataFile += "_";
//    currentDataFile += String(1900+local.tm_year);
//    currentDataFile += ".CSV";

    // so you have to close this one before opening another.
    bool firstLine = false;
    char wandel[14];
    currentDataFile.toCharArray(wandel, 14);
    //30.12.1899 12:30:00
    // loggerString()
    //loggerString += String(mynow.unixtime(), DEC);
    loggerString += String(local.tm_mday, DEC);
    loggerString += ".";
    loggerString += String(1 + local.tm_mon, DEC);
    loggerString += ".";
    loggerString += String(1900+local.tm_year, DEC);
    loggerString += " ";
    loggerString += String(local.tm_hour, DEC);
    loggerString += ":";
    loggerString += String(local.tm_min , DEC);
    loggerString += ":";
    loggerString += String(local.tm_sec, DEC);
    loggerString += ",";
    loggerString += String(temp, 2);// Grad in C°
    loggerString += ",";
    loggerString += String(myLoopHum,1);// Rel Feuchte in %
    loggerString += ",";
    loggerString += String(pres,2);// Druck in Pascal
    loggerString += ",";
    loggerString += String(myDawPoint,2);// Druck in Pascal
    loggerString += ",";
    loggerString += String(absHUM,2);// Druck in Pascal
    loggerString += ",";
    loggerString += String(lipoVoltage,2);// Druck in Pascal
    loggerString += "\n";


    // write my string
    // Check to see if the file exists:
    if (SPIFFS.exists(wandel)) {
      if(DEB_SPIFFS) Serial.println("file exist");
    }
    else {
      if(DEB_SPIFFS) Serial.println("file not exist first write, we create an first line");
      firstLine = true;
    }

    if(firstLine && isSpiffs){ // create file header
     writeFile(SPIFFS, wandel, "SSC-HYGROTERM-LOGFILE als CSV-Liste\n");
     appendFile(SPIFFS, wandel, "Spalten:\n");
     appendFile(SPIFFS, wandel, "Datum-Zeit T:M:YYYY Std:Min:Sek, Raumtemperatur in °C, Relative Luftfeute in %, Luftdruck in hPa, Taupunkt 100%, Absulute Luftfeuchte in g/m³,Lipo Spannung in V\n");
    }

    char loggerchar[60];
    loggerString.toCharArray(loggerchar, 60);
    if(isSpiffs) appendFile(SPIFFS, wandel, loggerchar);


    // print to the serial port too:
    if(DEB_SPIFFS) Serial.println(loggerString);

}
