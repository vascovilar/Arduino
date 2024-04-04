/*
 * Read data on Adafruit PA1010D for GPS navigation
 * Ref: https://learn.adafruit.com/adafruit-mini-gps-pa1010d-module/overview
 * Doc: https://cdn-learn.adafruit.com/assets/assets/000/084/295/original/CD_PA1010D_Datasheet_v.03.pdf?1573833002
 * 
 * Implementation:
 *
 *   #include <VSensorPA1010D.h>
 *
 *   VSensorPA1010D gps(0x10); 
 *
 *   void setup() {
 *     gps.init();
 *   }
 *   void loop() {
 *     float altitude = gps.getAltitude().value;
 *   }
 */

#ifndef VSensorPA1010D_h
#define VSensorPA1010D_h

#include "Arduino.h"
#include "VUsePins.h"
#include "VDevice.h"
#include "VSensor.h"
#include "Adafruit_GPS.h"
#include "Wire.h"

class VSensorPA1010D : public VDevice, public VSensor, public VUseI2cPins
{
  public:

    VSensorPA1010D(byte addr) : VDevice(GPS_NAViGATION), VSensor(false) {
      _i2cAddress = addr;
    };
 
    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update();
    float   read();

    // data updated
    vfield_data   getSatellite() { return _data.satellite; }
    vfield_data   getFixQuality() { return _data.fixQuality; }
    vfield_data   getLatitude() { return _data.latitude; }
    vfield_data   getLongitude() { return _data.longitude; }
    vfield_data   getAltitude() { return _data.altitude; }
    vfield_data   getSpeed() { return _data.speed; }
    vfield_data   getDirectionAngle() { return _data.directionAngle; }
    vfield_data   getCompassAngle() { return _data.compassAngle; }
    String        getDateTime() { return _data.dateTime; }
    String        getLatCardinal() { return _data.latCardinal; }
    String        getLongCardinal() { return _data.longCardinal; }
    String        getIsoDecimalLabel() { return String(_data.latitude.value, 7) + " " + _data.latCardinal + "," + String(_data.longitude.value, 7) + " " + _data.longCardinal; }
    
  private:

    Adafruit_GPS _gps = Adafruit_GPS(&Wire);
    byte         _i2cAddress;

    struct fields {
      vfield_data   satellite = {"Satelites connectés", "", 1.0};
      vfield_data   fixQuality = {"Qualité du réseau", "", 1.0};
      vfield_data   latitude = {"Latitude", "°", 0.0000001}; // 7 decimals
      vfield_data   longitude = {"Longitude", "°", 0.0000001}; // 7 decimals
      vfield_data   altitude = {"Altitude", "m", 10.0};
      vfield_data   speed = {"Vitesse", "km/h", 3.0};
      vfield_data   directionAngle = {"Cap", "°", 20.0}; // North = 0
      vfield_data   compassAngle = {"Compas", "°", 20.0}; // North = 0
      String        dateTime;      // in timestamp GMT
      String        latCardinal;   // N or S
      String        longCardinal;  // W or E
    };
    struct fields _data;

    vlegend_data _satellites[5] = {
      {0, ROUGE, "aucun"},
      {3, ORANGE, "faible précision"},
      {7, JAUNE, "précision moyenne"},
      {13, VERT, "bonne précision"},
      {14, VERT, "mamimum"},
    };

    vlegend_data _fixQualities[3] = {
      {0, VERT, ""},
      {1, VERT, "GPS"},
      {3, VERT, "DGPS"},
    };

    vlegend_data _latitudes[1] = {
      {0, VERT, "ok"},
    };

    vlegend_data _longitudes[1] = {
      {0, VERT, "ok"},
    };

    vlegend_data _altitudes[1] = {
      {0, VERT, "ok"},
    };

    vlegend_data _speeds[1] = {
      {0, VERT, "ok"},
    };

    vlegend_data _directionAngles[1] = {
      {0, VERT, "ok"},
    };

    vlegend_data _compassAngles[1] = {
      {0, VERT, "ok"},
    };

    String _convertToDateTime(int year, int month, int day, int hour, int minute, int second);
    float _convertToKmH(float knot);
};

#endif
