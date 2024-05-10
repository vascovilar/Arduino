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
    bool    sleep();
    bool    wake();
    bool    check();
    bool    update();
    float   read();

    // data updated
    vfield  getSatellite() { return _data.satellite; }
    vfield  getFixQuality() { return _data.fixQuality; }
    vfield  getAltitude() { return _data.altitude; }
    vfield  getSpeed() { return _data.speed; }
    float   getLatitude() { return _data.latitude; }
    float   getLongitude() { return _data.longitude; }
    float   getDirectionAngle() { return _data.directionAngle; }
    float   getCompassAngle() { return _data.compassAngle; }
    String  getDateTime() { return _data.dateTime; }
    String  getLatCardinal() { return _data.latCardinal; }
    String  getLongCardinal() { return _data.longCardinal; }
    String  getIsoDecimalLabel() { return _data.isoLabel; }

  private:

    Adafruit_GPS _gps = Adafruit_GPS(&Wire);
    byte    _i2cAddress;
    String  _convertToDateTime(int year, int month, int day, int hour, int minute, int second);
    float   _convertToKmH(float knot);

    // human readable buffer. Updated by udpate function
    struct fields {
      vfield   satellite = {"Satelites connectés", "", 1.0};
      vfield   fixQuality = {"Qualité du réseau", "", 1.0};
      vfield   altitude = {"Altitude", "m", 10.0};
      vfield   speed = {"Vitesse", "km/h", 3.0};
      float    latitude; // 7 decimals
      float    longitude; // 7 decimals
      float    directionAngle; // North = 0
      float    compassAngle; // North = 0
      String   dateTime;      // timestamp like '1970-01-29 00:00:00'
      String   latCardinal;   // N or S
      String   longCardinal;  // W or E
      String   isoLabel; // iso like '0.0000001,0.0000001 NE'
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

    vlegend _altitudes[1] = {
      {0, VERT, "ok"},
    };

    vlegend _speeds[1] = {
      {0, VERT, "ok"},
    };
};

#endif
