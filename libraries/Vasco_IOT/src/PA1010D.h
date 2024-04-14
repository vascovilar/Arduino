/*
 * Read data on Adafruit PA1010D for GPS navigation
 * Ref: https://learn.adafruit.com/adafruit-mini-gps-pa1010d-module/overview
 * Doc: https://cdn-learn.adafruit.com/assets/assets/000/084/295/original/CD_PA1010D_Datasheet_v.03.pdf?1573833002
 * 
 * Implementation:
 *
 *   #include <PA1010D.h>
 *
 *   PA1010D gps(0x10); 
 *
 *   void setup() {
 *     gps.init();
 *   }
 *   void loop() {
 *     float altitude = gps.getAltitude().value;
 *   }
 */

#ifndef PA1010D_h
#define PA1010D_h

#include "Arduino.h"
#include "interface/Device.h"
#include "interface/Sensor.h"
#include "plugin/Pins.h"
#include "Adafruit_GPS.h"
#include "Wire.h"


class PA1010D : public Device, public Sensor, public I2cPins
{
  public:
  
    PA1010D(byte addr) : Device(GPS_NAVIGATOR), Sensor(false) { _i2cAddress = addr; }
    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update();
    float   read();
    // data updated
    vfield  getSatellite() { return _data.satellite; }
    vfield  getFixQuality() { return _data.fixQuality; }
    vfield  getLatitude() { return _data.latitude; }
    vfield  getLongitude() { return _data.longitude; }
    vfield  getAltitude() { return _data.altitude; }
    vfield  getSpeed() { return _data.speed; }
    vfield  getDirectionAngle() { return _data.directionAngle; }
    vfield  getCompassAngle() { return _data.compassAngle; }
    String  getDateTime() { return _data.dateTime; }
    String  getLatCardinal() { return _data.latCardinal; }
    String  getLongCardinal() { return _data.longCardinal; }
    String  getIsoDecimalLabel() { return String(_data.latitude.value, 7) + " " + _data.latCardinal + "," + String(_data.longitude.value, 7) + " " + _data.longCardinal; }
    
  private:

    Adafruit_GPS _gps = Adafruit_GPS(&Wire);
    byte    _i2cAddress;
    String  _convertToDateTime(int year, int month, int day, int hour, int minute, int second);
    float   _convertToKmH(float knot);
    
    // human readable
    struct fields {
      vfield   satellite = {"Satelites connectés", "", 1.0};
      vfield   fixQuality = {"Qualité du réseau", "", 1.0};
      vfield   latitude = {"Latitude", "°", 0.0000001}; // 7 decimals
      vfield   longitude = {"Longitude", "°", 0.0000001}; // 7 decimals
      vfield   altitude = {"Altitude", "m", 10.0};
      vfield   speed = {"Vitesse", "km/h", 3.0};
      vfield   directionAngle = {"Cap", "°", 20.0}; // North = 0
      vfield   compassAngle = {"Compas", "°", 20.0}; // North = 0
      String   dateTime;      // in timestamp GMT
      String   latCardinal;   // N or S
      String   longCardinal;  // W or E
    };
    fields _data;

    vlegend _satellites[5] = {
      {0, ROUGE, "aucun"},
      {3, ORANGE, "faible précision"},
      {7, JAUNE, "précision moyenne"},
      {13, VERT, "bonne précision"},
      {14, VERT, "mamimum"},
    };

    vlegend _fixQualities[3] = {
      {0, VERT, ""},
      {1, VERT, "GPS"},
      {3, VERT, "DGPS"},
    };

    vlegend _latitudes[1] = {
      {0, VERT, "ok"},
    };

    vlegend _longitudes[1] = {
      {0, VERT, "ok"},
    };

    vlegend _altitudes[1] = {
      {0, VERT, "ok"},
    };

    vlegend _speeds[1] = {
      {0, VERT, "ok"},
    };

    vlegend _directionAngles[1] = {
      {0, VERT, "ok"},
    };

    vlegend _compassAngles[1] = {
      {0, VERT, "ok"},
    };
};

#endif
