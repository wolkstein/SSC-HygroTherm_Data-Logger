
bool initSDCard(){
  return SD.begin();
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  if( DEB_SD ) Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    if( DEB_SD ) Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    if( DEB_SD ) Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      if( DEB_SD ) Serial.print("  DIR : ");
      if( DEB_SD ) Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      if( DEB_SD ) Serial.print("  FILE: ");
      if( DEB_SD ) Serial.print(file.name());
      if( DEB_SD ) Serial.print("  SIZE: ");
      if( DEB_SD ) Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path) {
  if( DEB_SD ) Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    if( DEB_SD ) Serial.println("Dir created");
    sdcardFail = false;
  } else {
    if( DEB_SD ) Serial.println("mkdir failed");
    sdcardFail = true;
  }
}

void removeDir(fs::FS &fs, const char * path) {
  if( DEB_SD ) Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    if( DEB_SD ) Serial.println("Dir removed");
    sdcardFail = false;
  } else {
    if( DEB_SD ) Serial.println("rmdir failed");
    sdcardFail = true;
  }
}

void readFile(fs::FS &fs, const char * path) {
  if( DEB_SD ) Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    if( DEB_SD ) Serial.println("Failed to open file for reading");
    sdcardFail = true;
    return;
  }else{
    sdcardFail = false;
  }

  if( DEB_SD ) Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  if( DEB_SD ) Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    if( DEB_SD ) Serial.println("Failed to open file for writing");
    sdcardFail = true;
    return;
  }
  if (file.print(message)) {
    if( DEB_SD ) Serial.println("File written");
    sdcardFail = false;
  } else {
    if( DEB_SD ) Serial.println("Write failed");
    sdcardFail = true;
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  if( DEB_SD ) Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    if( DEB_SD ) Serial.println("Failed to open file for appending");
    sdcardFail = true;
    return;
  }
  if (file.print(message)) {
    if( DEB_SD ) Serial.println("Message appended");
    sdcardFail = false;
  } else {
    if( DEB_SD ) Serial.println("Append failed");
    sdcardFail = true;
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2) {
  if( DEB_SD ) Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    if( DEB_SD ) Serial.println("File renamed");
    sdcardFail = false;
  } else {
    if( DEB_SD ) Serial.println("Rename failed");
    sdcardFail = true;
  }
}

void deleteFile(fs::FS &fs, const char * path) {
  if( DEB_SD ) Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    if( DEB_SD ) Serial.println("File deleted");
    sdcardFail = false;
  } else {
    if( DEB_SD ) Serial.println("Delete failed");
    sdcardFail = true;
  }
}

void testFileIO(fs::FS &fs, const char * path) {
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file) {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    if( DEB_SD ) Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    if( DEB_SD ) Serial.println("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if (!file) {
    if( DEB_SD ) Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) {
    file.write(buf, 512);
  }
  end = millis() - start;
  if( DEB_SD ) Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}
