#include <VSensorGPS.h>

VSensorGPS   gps; 

void setup() 
{
  // global
  Serial.begin(115200);
  Wire.begin(21, 22);

  gps.begin(0x10);
  delay(2000);
}

void loop() 
{
  if (gps.update(1000)) {
    String output = "Sat:" + String(gps.getSatellites()) + "  " + gps.getFixQuality() + "\n"
                  + gps.getDateTime() + "\n"
                  + String(gps.getLatitude(), 7) + " " + String(gps.getLatCardinal()) + "\n"
                  + String(gps.getLongitude(), 7) + " " + String(gps.getLongCardinal()) + "\n"
                  + "Altitude:  " + String(gps.getAltitude(), 2) + " m\n"
                  + "Compass:   " + String(gps.getCompassAngle(), 2) + " deg " + String(gps.getComCardinal()) + "\n"
                  + "Speed:     " + String(gps.getSpeed(), 2) + " km/h\n"
                  + "Direction: " + String(gps.getDirectionAngle(), 2) + " deg\n";
  
    Serial.println(output);    
  }
}
