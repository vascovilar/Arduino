#include "VGearSdCard.h"

void VGearSdCard::begin()
{
  Serial.print("Initialize SD card ...");
    
  _spi.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  
  if (!SD.begin(SD_CS, _spi)) {
    Serial.println("Communication failed");
    _status = false;
    return;
  }
  
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    _status = false;
    return;
  }
  
  Serial.println("OK");
  int size = SD.cardSize() / (1024 * 1024);
  float used = (SD.usedBytes() / SD.totalBytes()) * 100.0; 
  Serial.println("Size: " + String(size) + " Mo (" + String(used) + "% used)");
  _status = true; 
}

bool VGearSdCard::status()
{ 
  return _status;
}

Page VGearSdCard::list(const String &path)
{
  Page list;
  list.total = 0;
  list.length = PAGE_LENGTH;

  File dir = SD.open(path);

  if (!dir.isDirectory()) {
    Serial.println(path + " is not a directory");
    dir.close();
  }

  dir.rewindDirectory();

  while (true)
  {
    File file = dir.openNextFile();
    if (!file) {
      break;
    }

    // push value in buffer by sliding others from end to start
    for(int i = PAGE_LENGTH - 1; i >= 0 ; i--) 
    {
      if(i == 0) {
        list.data[0] = String(file.name());
      } else {
        list.data[i] = list.data[i-1];  
      }
    }

    list.total = list.total + 1;

    file.close();
  }

  dir.close();

  return list;
}

File VGearSdCard::open(const String &path)
{
  File file = SD.open(path);

  if (!file) {
    Serial.println(path + " not found");
  }

  if (file.isDirectory()) {
    Serial.println(path + " is not a file");
    file.close();
  }  

  return file;
}

void VGearSdCard::close(File file)
{
  file.close();
}

void VGearSdCard::save(const String &path, uint8_t * data, size_t length)
{
  File file = SD.open(path, FILE_WRITE);

  if (!file) {
    Serial.println("Error while writing " + path);
    return;
  } 

  file.write(data, length);
  Serial.println("File created: " + path);
  
  file.close();
}

void VGearSdCard::remove(const String &path) 
{
  File file = SD.open(path);
  
  if (!file.isDirectory()) {
    file.close();
    SD.remove(path);
    return;
  }

  file.rewindDirectory();

  while (true) 
  {
    File entry = file.openNextFile();
    if (!entry) {
      break;
    }

    String entryPath = path + "/" + entry.name();
    
    if (entry.isDirectory()) {
      entry.close();
      remove(entryPath);
    } else {
      entry.close();
      SD.remove((char *)entryPath.c_str());
    }

    yield();
  }

  SD.rmdir(path);
  file.close();
}

String VGearSdCard::date(const String &path)
{
  File file = SD.open(path);
  time_t time = file.getLastWrite();
  file.close();

  tm * timeinfo = localtime(&time);
  char timeStamp[80];
  snprintf(timeStamp, 80, "%04d-%02d-%02d %02d:%02d:%02d", 
    timeinfo->tm_year + 1900, 
    timeinfo->tm_mon + 1, 
    timeinfo->tm_mday, 
    timeinfo->tm_hour, 
    timeinfo->tm_min, 
    timeinfo->tm_sec
  );

  return timeStamp;
}
