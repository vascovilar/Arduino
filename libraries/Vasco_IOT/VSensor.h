#ifndef VSensor_h
#define VSensor_h

#include "Arduino.h"

#define VSENSOR_COUNT  11
enum vsensor_code {
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
};

#define VSTATUS_COUNT  6
enum vstatus_code {
  GRIS = 0,   // inactif
  VERT = 1,   // confort
  JAUNE = 2,  // perturbé
  ORANGE = 3, // gênestatuses
  ROUGE = 4,  // mauvais
  VIOLET = 5, // danger
};

struct vfield_data {
  String        label;
  String        unit;
  float         tolerance;
  float         value;
  vstatus_code  status;
  String        text;
};

struct vlegend_data {
  float         limit;
  vstatus_code  status;
  String        text;
};


class VSensor
{
  public:

    VSensor(bool isRealtime) {
      _isRealtime = isRealtime;
    }

    virtual float  read(); // read instant sensor value out of processtime incretementation, or return 0 

    bool           isRealTime() { return _isRealtime; }
    
  protected:

    bool           _isRealtime;

    void _feed(vfield_data &field, float value, vlegend_data* data, int length)
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
