// Test code for Adafruit GPS That Support Using I2C
//
// This code shows how to parse data from the I2C GPS
//
// Pick one up today at the Adafruit electronics shop
// and help support open source hardware & software! -ada

#include <VScreenMini.h>
#include <Adafruit_GPS.h>

// Connect to the GPS on the hardware I2C port
VScreenMini view;
Adafruit_GPS GPS(&Wire);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

uint32_t timer = millis();


void setup()
{
  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  delay(2000);
  Serial.println("Adafruit I2C GPS library basic test!");

  view.begin(0x3C);
  
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(0x10);  // The I2C address to use is 0x10
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  // Ask for firmware version
  GPS.println(PMTK_Q_RELEASE);
}

void loop() // run over and over again
{
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO) {
    if (c) { Serial.print(c); }
  }
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    //Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) { // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
    }
  }


  // approximately every 1 seconds or so, print out the current stats
  if (millis() - timer > 1000) {
    timer = millis(); // reset the timer
    
    String output = "";

    output += "Fix:" + String(GPS.fix) + " ";
    if (GPS.fixquality == 2) {
      output += "DGPS";
    } else if(GPS.fixquality == 1) {
      output += "GPS ";
    } else {
      output += "    ";
    }
    output += " Sat:" + String(GPS.satellites) + "\n";


    if (GPS.day < 10) { output += "0"; }
    output += String(GPS.day) + "/";
    if (GPS.month < 10) { output += "0"; }
    output += String(GPS.month) + "/20";
    if (GPS.year < 10) { output += "0"; }
    output += String(GPS.year) + " ";
    if (GPS.hour < 10) { output += "0"; }
    output += String(GPS.hour) + ":";
    if (GPS.minute < 10) { output += "0"; }
    output += String(GPS.minute) + ":";
    if (GPS.seconds < 10) { output += "0"; }
    output += String(GPS.seconds) + "\n";

    output += String(GPS.lat) + " " + String(GPS.latitudeDegrees, 7) + "\n" ;
    output += String(GPS.lon) + " " + String(GPS.longitudeDegrees, 6) + "\n";
    output += "H " + String(GPS.altitude, 2) + " m\n";
    output += "Orientation: " + String(GPS.magvariation, 2) + String(GPS.mag) + "\n";
    output += "Speed(knot): " + String(GPS.speed, 2) + "\n";
    output += "Angle(deg) : " + String(GPS.angle, 2) + "\n";
    
    view.clear();

    view.rectangleBox(118, 0, 10, 64, GPS.fix);
    view.printAt(0, 0, output);
    view.update();
     

    // OLD
    Serial.println("********************************************************");
    Serial.print("Time: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
    Serial.println("********************************************************");
    
  }
}