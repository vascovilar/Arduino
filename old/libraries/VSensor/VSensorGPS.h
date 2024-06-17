/*
 * Read data on PA1010D
 * Implementation:
 *
 *   #include <VSensorGPS.h>
 *
 *   VSensorGPS gps; 
 *
 *   void setup() {
 *     gps.begin(0x10);
 *   }
 *   void loop() {
 *     if (gps.update(1000)) {
 *      float altitude = gps.getAltitude();
 *     }
 *   }
 */

#ifndef VSensorGPS_h
#define VSensorGPS_h

#include "Arduino.h"
#include "Adafruit_GPS.h"

#define KNOTS_TO_KMH 1.852 // one knot is 1.852 km/h

struct gps_field_data {
  String   dateTime;      // in timestamp GMT
  String   fixQuality;    // enum DGPS, GPS or ""
  int      satellites;    // in quantity number (max 14) 
  float    latitude;      // in Decimal degrees, 7 decimals
  String   latCardinal;   // enum N or S
  float    longitude;     // in Decimal degrees, 7 decimals
  String   longCardinal;  // enum W or E
  float    altitude;      // in meters
  float    speed;         // in km/h
  float    directionAngle;// in degrees (North = 0)
  float    compassAngle;  // in degrees (North = 0)
  String   comCardinal;   // enum
};

class VSensorGPS
{
  public:
    void begin(int addr);
    bool update(int delay);
    
    String getIsoDecimalLabel();
    
    gps_field_data dump() { return _data; }
    String getDateTime() { return _data.dateTime; }
    String getFixQuality() { return _data.fixQuality; }
    int getSatellites() { return _data.satellites; }
    float getLatitude() { return _data.latitude; }
    String getLatCardinal() { return _data.latCardinal; }
    float getLongitude() { return _data.longitude; }
    String getLongCardinal() { return _data.longCardinal; }
    float getAltitude() { return _data.altitude; }
    float getSpeed() { return _data.speed; }
    float getDirectionAngle() { return _data.directionAngle; }
    float getCompassAngle() { return _data.compassAngle; }
    String getComCardinal() { return _data.comCardinal; }
    
  private:
    Adafruit_GPS _gps;
    struct gps_field_data _data;
    uint32_t _timer = millis();
    
    String _convertToDateTime(int year, int month, int day, int hour, int minute, int second);
    String _convertToQualityLabel(int quality);
    float _convertToKmH(float knot);
};

#endif
