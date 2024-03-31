/*
 * Read data on PA1010D
 * Implementation:
 *
 *   #include <VSensorPA1010D.h>
 *
 *   VSensorPA1010D gps(0); 
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
#include "VDevice.h"
#include "VSensor.h"
#include "Adafruit_GPS.h"
#include "Wire.h"

class VSensorPA1010D : public VDevice, public VSensor, public VI2CPins
{
  public:

    VSensorPA1010D(byte pin) : VDevice(ADA_PA1010D), VSensor(pin) {};
 
    bool init();
    bool wake();
    bool sleep();
    bool sync();
    bool event();
    float check();

    field_data   getSatellite() { return _data.satellite; }
    field_data   getFixQuality() { return _data.fixQuality; }
    field_data   getLatitude() { return _data.latitude; }
    field_data   getLongitude() { return _data.longitude; }
    field_data   getAltitude() { return _data.altitude; }
    field_data   getSpeed() { return _data.speed; }
    field_data   getDirectionAngle() { return _data.directionAngle; }
    field_data   getCompassAngle() { return _data.compassAngle; }
    String       getDateTime() { return _data.dateTime; }
    String       getLatCardinal() { return _data.latCardinal; }
    String       getLongCardinal() { return _data.longCardinal; }
    String       getIsoDecimalLabel() { return String(_data.latitude.value, 7) + " " + _data.latCardinal + "," + String(_data.longitude.value, 7) + " " + _data.longCardinal; }
    
  private:

    Adafruit_GPS _gps = Adafruit_GPS(&Wire);

    struct fields {
      field_data   satellite = {"Satelites connectés", "", 1.0};
      field_data   fixQuality = {"Qualité du réseau", "", 1.0};
      field_data   latitude = {"Latitude", "°", 0.0000001}; // 7 decimals
      field_data   longitude = {"Longitude", "°", 0.0000001}; // 7 decimals
      field_data   altitude = {"Altitude", "m", 10.0};
      field_data   speed = {"Vitesse", "km/h", 3.0};
      field_data   directionAngle = {"Cap", "°", 20.0}; // North = 0
      field_data   compassAngle = {"Compas", "°", 20.0}; // North = 0
      String       dateTime;      // in timestamp GMT
      String       latCardinal;   // N or S
      String       longCardinal;  // W or E
    };
    struct fields _data;

    legend_data _satellites[5] = {
      {0, ROUGE, "aucun"},
      {3, ORANGE, "faible précision"},
      {7, JAUNE, "précision moyenne"},
      {13, VERT, "bonne précision"},
      {14, VERT, "mamimum"},
    };

    legend_data _fixQualities[3] = {
      {0, VERT, ""},
      {1, VERT, "GPS"},
      {3, VERT, "DGPS"},
    };

    legend_data _latitudes[1] = {
      {0, VERT, "ok"},
    };

    legend_data _longitudes[1] = {
      {0, VERT, "ok"},
    };

    legend_data _altitudes[1] = {
      {0, VERT, "ok"},
    };

    legend_data _speeds[1] = {
      {0, VERT, "ok"},
    };

    legend_data _directionAngles[1] = {
      {0, VERT, "ok"},
    };

    legend_data _compassAngles[1] = {
      {0, VERT, "ok"},
    };

    String _convertToDateTime(int year, int month, int day, int hour, int minute, int second);
    float _convertToKmH(float knot);
};

#endif
