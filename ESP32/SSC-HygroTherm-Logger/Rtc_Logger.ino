
void rtcLogger(float myLoopHum){


    loggerString = "";
    currentDataFile = "/";
    currentDataFile += String(mynow.month());
    currentDataFile += "_";
    currentDataFile += String(mynow.year());
    currentDataFile += ".CSV";

    // so you have to close this one before opening another.
    bool firstLine = false;
    char wandel[14];
    currentDataFile.toCharArray(wandel, 14);
    //30.12.1899 12:30:00
    // loggerString()
    //loggerString += String(mynow.unixtime(), DEC);
    loggerString += String(mynow.day(), DEC);
    loggerString += ".";
    loggerString += String(mynow.month(), DEC);
    loggerString += ".";
    loggerString += String(mynow.year(), DEC);
    loggerString += " ";
    loggerString += String(mynow.hour(), DEC);
    loggerString += ":";
    loggerString += String(mynow.minute(), DEC);
    loggerString += ":";
    loggerString += String(mynow.second(), DEC);
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
    if (SD.exists(wandel)) {
      if(DEB_SD) Serial.println("file exist");
    }
    else {
      if(DEB_SD) Serial.println("file not exist first write, we create an first line");
      firstLine = true;
    }

    if(firstLine && isSdCard){ // create file header
     writeFile(SD, wandel, "SSC-HYGROTERM-LOGFILE als CSV-Liste\n");
     appendFile(SD, wandel, "Spalten:\n");
     appendFile(SD, wandel, "Datum-Zeit T:M:YYYY Std:Min:Sek, Raumtemperatur in °C, Relative Luftfeute in %, Luftdruck in hPa, Taupunkt 100%, Absulute Luftfeuchte in g/m³,Lipo Spannung in V\n");
    }

    char loggerchar[60];
    loggerString.toCharArray(loggerchar, 60);
    if(isSdCard) appendFile(SD, wandel, loggerchar);


    // print to the serial port too:
    if(DEB_SDS) Serial.println(loggerString);

}
