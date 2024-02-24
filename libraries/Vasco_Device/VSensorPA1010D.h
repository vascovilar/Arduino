/*
 * Read data on PA1010D
 * Implementation:
 *
 *   #include <VSensorPA1010D.h>
 *
 *   VSensorPA1010D gps; 
 *
 *   void setup() {
 *     gps.begin(0x10);
 *   }
 *   void loop() {
 *     if (gps.update(1000)) {
 *      float altitude = gps.getAltitude().value;
 *     }
 *   }
 */

#ifndef VSensorPA1010D_h
#define VSensorPA1010D_h

#include "Arduino.h"
#include "VConfig.h"
#include "Adafruit_GPS.h"

class VSensorPA1010D
{
  public:

    void begin(int addr); // I2C must be 0x10
    bool update(int delay);
    void sync();
    void sleep(bool isSleeping);  
    
    field_data   getSatellites() { return _data.satellites; }
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
    unsigned int getProcessTime() { return _data.processTime; }
    
  private:

    Adafruit_GPS _gps;
    unsigned int _timer = 0;
    bool _enabled = false;

    String _convertToDateTime(int year, int month, int day, int hour, int minute, int second);
    float _convertToKmH(float knot);

    struct fields {
      field_data   satellites = {"Satelites connectés", "", 1.0}; // max 14 
      field_data   fixQuality = {"Qualité du réseau", "", 1.0};    // enum DGPS, GPS or ""
      field_data   latitude = {"Latitude", "°", 0.0000001}; // 7 decimals
      field_data   longitude = {"Longitude", "°", 0.0000001}; // 7 decimals
      field_data   altitude = {"Altitude", "m", 10.0};
      field_data   speed = {"Vitesse", "km/h", 3.0};
      field_data   directionAngle = {"Cap", "°", 20.0}; // North = 0
      field_data   compassAngle = {"Compas", "°", 20.0}; // North = 0
      String       dateTime;      // in timestamp GMT
      String       latCardinal;   // enum N or S
      String       longCardinal;  // enum W or E
      unsigned int processTime;
    };
    struct fields _data;

    void _setSatellites(float value)
    {
      _data.satellites.value = value;
      
      if (value <= 0) { _data.satellites.status = ROUGE; _data.satellites.text = "aucun"; } 
      else if (value <= 3) { _data.satellites.status = ORANGE; _data.satellites.text = "faible précision"; } 
      else if (value <= 7) { _data.satellites.status = JAUNE; _data.satellites.text = "précision moyenne"; } 
      else if (value <= 13) { _data.satellites.status = VERT; _data.satellites.text = "bonne précision"; } 
      else { _data.satellites.status = VERT; _data.satellites.text = "mamimum"; }
    }

    void _setFixQuality(float value)
    {
      _data.fixQuality.value = value;
      
      if (value <= 0) { _data.fixQuality.status = ROUGE; _data.fixQuality.text = ""; } 
      else if (value <= 1) { _data.satellites.status = JAUNE; _data.satellites.text = "GPS"; } 
      else { _data.satellites.status = VERT; _data.satellites.text = "DGPS"; }      
    }

    void _setLatitude(float value)
    {
      _data.latitude.value = value;
      _data.latitude.status = VERT;
    }
    
    void _setLongitude(float value)
    {
      _data.longitude.value = value;
      _data.longitude.status = VERT;
    }

    void _setAltitude(float value)
    {
      _data.altitude.value = value;
      _data.altitude.status = VERT;
    }

    void _setSpeed(float value)
    {
      _data.speed.value = value;
      _data.speed.status = VERT;
    }

    void _setDirectionAngle(float value)
    {
      _data.directionAngle.value = value;
      _data.directionAngle.status = VERT;
    }

    void _setCompassAngle(float value)
    {
      _data.compassAngle.value = value;
      _data.compassAngle.status = VERT;
    }  
};

#endif
