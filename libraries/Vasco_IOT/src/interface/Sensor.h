#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"

static const byte VSENSOR_COUNT = 12;

enum vsensor {
  TEMPERATURE = 0,
  PRESSURE = 1,
  HUMIDITY = 2,
  AIR_QUALITY = 3,
  GAS_PERCENTAGE = 4,
  EMF_LEVEL = 5,
  UV_INDEX = 6,
  VISIBLE = 7,
  EAR_LEVEL = 8,
  ALTITUDE = 9,
  MEMORY_USED = 10,
  GAUSS_LEVEL = 11,
};

static const byte VSTATUS_COUNT = 6;

enum vstatus {
  GRIS = 0,   // inactif
  VERT = 1,   // confort
  JAUNE = 2,  // perturbé
  ORANGE = 3, // gêne
  ROUGE = 4,  // mauvais
  VIOLET = 5, // danger
};

struct vfield {
  String    label;
  String    unit;
  float     tolerance;
  float     value;
  vstatus   status;
  String    text;
};

struct vlegend {
  float     limit;
  vstatus   status;
  String    text;
};


class Sensor
{
  public:

    Sensor(bool isRealtime) { _isRealtime = isRealtime; }
    virtual float   read(); // read instant sensor value out of calibration or processtime incretementation, or return 0 if non applicable
    bool            isRealTime() { return _isRealtime; } // if so get access to read() realtime raw data

  protected:

    bool    _isRealtime;
    void    _feed(vfield &field, float value, vlegend* data, int length)
    {
      field.value = value;
      for (int i = 0; i < length; i ++) {
        if (value <= data[i].limit || i == length - 1) {
          field.status = data[i].status;
          field.text = data[i].text;
          break;
        }
      }
    }

  private:
  
    struct    fields {}; // override these fields by inheritance in child class
    fields    _data; // override too
};

#endif