

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
   if(DEB_SPIFFS)  Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
       if(DEB_SPIFFS)  Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
       if(DEB_SPIFFS)  Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
           if(DEB_SPIFFS)  Serial.print("  DIR : ");
           if(DEB_SPIFFS)  Serial.print (file.name());
            time_t t= file.getLastWrite();
            struct tm * tmstruct = localtime(&t);
           if(DEB_SPIFFS)  Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
           if(DEB_SPIFFS)  Serial.print("  FILE: ");
           if(DEB_SPIFFS)  Serial.print(file.name());
           if(DEB_SPIFFS)  Serial.print("  SIZE: ");
           if(DEB_SPIFFS)  Serial.print(file.size()/1024.0);
           if(DEB_SPIFFS)  Serial.print("kilobytes");
           time_t t= file.getLastWrite();
           struct tm * tmstruct = localtime(&t);
           if(DEB_SPIFFS)  Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
        }
        file = root.openNextFile();
    }
}

void getListOfFileNames(fs::FS &fs, String &liste, const char * dirname, uint8_t levels){
   if(DEB_SPIFFS)  Serial.printf("Listing directory return list: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
       if(DEB_SPIFFS)  Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
       if(DEB_SPIFFS)  Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
           if(DEB_SPIFFS)  Serial.print("  DIR : ");
           if(DEB_SPIFFS)  Serial.print (file.name());
            time_t t= file.getLastWrite();
            struct tm * tmstruct = localtime(&t);
           if(DEB_SPIFFS)  Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
           if(DEB_SPIFFS)  Serial.print("  FILE: ");
           if(DEB_SPIFFS)  Serial.print(file.name());
           if(DEB_SPIFFS)  Serial.print("  SIZE: ");
           if(DEB_SPIFFS)  Serial.print(file.size()/1024.0);
           if(DEB_SPIFFS)  Serial.print("kilobytes");
           time_t t= file.getLastWrite();
           struct tm * tmstruct = localtime(&t);
           if(DEB_SPIFFS)  Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
           liste += "FILE: ";
           liste += String(file.name());
           liste += " SIZE: ";
           liste += String(file.size()/1024.0);
           liste += " kBytes";
           liste += "<br>\n";
        }
        file = root.openNextFile();
    }  
}

void createDir(fs::FS &fs, const char * path){
   if(DEB_SPIFFS)  Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
       if(DEB_SPIFFS)  Serial.println("Dir created");
    } else {
       if(DEB_SPIFFS)  Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
   if(DEB_SPIFFS)  Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
       if(DEB_SPIFFS)  Serial.println("Dir removed");
    } else {
       if(DEB_SPIFFS)  Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
   if(DEB_SPIFFS)  Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
       if(DEB_SPIFFS)  Serial.println("Failed to open file for reading");
        return;
    }

   if(DEB_SPIFFS)  Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
   if(DEB_SPIFFS)  Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
       if(DEB_SPIFFS)  Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
       if(DEB_SPIFFS)  Serial.println("File written");
    } else {
       if(DEB_SPIFFS)  Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
   if(DEB_SPIFFS)  Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
       if(DEB_SPIFFS)  Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
       if(DEB_SPIFFS)  Serial.println("Message appended");
    } else {
       if(DEB_SPIFFS)  Serial.println("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
   if(DEB_SPIFFS)  Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
       if(DEB_SPIFFS)  Serial.println("File renamed");
    } else {
       if(DEB_SPIFFS)  Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
   if(DEB_SPIFFS)  Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
       if(DEB_SPIFFS)  Serial.println("File deleted");
    } else {
       if(DEB_SPIFFS)  Serial.println("Delete failed");
    }
}
