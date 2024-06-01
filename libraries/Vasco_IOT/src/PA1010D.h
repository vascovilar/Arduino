/*
 * Read data on Adafruit PA1010D for GPS navigation
 * Ref: https://learn.adafruit.com/adafruit-mini-gps-pa1010d-module/overview
 * Doc: https://cdn-learn.adafruit.com/assets/assets/000/084/295/original/CD_PA1010D_Datasheet_v.03.pdf?1573833002
 * 48.492744141 2.219772949 (parent)
 */

#ifndef PA1010D_h
#define PA1010D_h

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Sensor.h"
#include "component/Pins.h"
#include "Adafruit_GPS.h"
#include "Wire.h"


class PA1010D : public Sensor, public I2cPins
{
  public:

    PA1010D(byte addr) : Sensor(GPS_NAVIGATOR, false) { _i2cAddress = addr; }

    // interfaces
    bool    init();
    bool    sleep();
    bool    wake();
    bool    check();
    bool    update();
    float   read();
    vfield  get(vsensor code)
    {
      switch(code) {
        case ALTITUDE:
          return _altitude;
      }

      return (vfield){};
    }

    // other data updated
    vfield  getSatellite() { return _satellite; }
    vfield  getFixQuality() { return _fixQuality; }
    vfield  getSpeed() { return _speed; }
    float   getLatitude() { return _latitude; }
    float   getLongitude() { return _longitude; }
    float   getDirectionAngle() { return _directionAngle; }
    float   getCompassAngle() { return _compassAngle; }
    String  getDateTime() { return _dateTime; }
    String  getLatCardinal() { return _latCardinal; }
    String  getLongCardinal() { return _longCardinal; }
    String  getIsoDecimalLabel() { return _isoLabel; }

  private:

    Adafruit_GPS _gps = Adafruit_GPS(&Wire);
    byte    _i2cAddress;
    vfield  _satellite = {"Satelites connectés", "", 1};
    vfield  _fixQuality = {"Qualité du réseau", "", 1};
    vfield  _altitude = {"Altitude", "m", 10};
    vfield  _speed = {"Vitesse", "km/h", 3};
    float   _latitude = 0; // 7 decimals
    float   _longitude = 0; // 7 decimals
    float   _directionAngle = 0; // North = 0
    float   _compassAngle = 0; // North = 0
    String  _dateTime = "";      // timestamp like '1970-01-29 00:00:00'
    String  _latCardinal = "";   // N or S
    String  _longCardinal = "";  // W or E
    String  _isoLabel = ""; // iso like '0.0000001,0.0000001 NE'

    String  _convertToDateTime(int year, int month, int day, int hour, int minute, int second);
    float   _convertToKmH(float knot);
    String  _convertToIsoLabel(float latitude, float longitude, String latCardinal, String longCardinal);


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
