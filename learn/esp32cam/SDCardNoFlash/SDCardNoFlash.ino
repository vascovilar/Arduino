// Libraries for SD card
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Define CS pin for the SD card module
#define SD_MISO 2
#define SD_MOSI 15
#define SD_SCLK 14
#define SD_CS 13
SPIClass sdSPI(VSPI);

String dataMessage;

void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(115200);

  // Initialize SD card
  sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, sdSPI)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;
  }

  // list directory
  File root = SD.open("/");
  printDirectory(root, 0);
  Serial.println("done!");



  /*
// old ///////////////////////////////////////////////////////////////

  Serial.println("2");
  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  File file = SD.open("/data1.txt");
  if(!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data1.txt", "Reading ID, Date, Hour, Temperature \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();
  logSDCard();  
*/
}

void loop() {
}



/////////////////////////////////////////////////////////////////

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }

    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }

    Serial.print(entry.name());

    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }

    entry.close();
  }
}







// old ///////////////////////////////////////////////////////////////

// Write the sensor readings on the SD card
void logSDCard() {
  //dataMessage = String(readingID) + "," + String(dayStamp) + "," + String(timeStamp) + "," +
  //              String(temperature) + "\r\n";
  dataMessage = "Hello World \n";
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/data1.txt", dataMessage.c_str());
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}