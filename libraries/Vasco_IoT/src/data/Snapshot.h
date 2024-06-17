#ifndef Snapshot_h
#define Snapshot_h

#include "Arduino.h"
#include "../interface/Data.h"
#include "../interface/Sensor.h"

static const int    VSNAPSHOT_MAX_SIZE = 240; // 240


class Snapshot : public Data
{
  public:

    Snapshot(Sensor &obj, int length) : _sensor(obj) { _hydrate(length); }

    ~Snapshot() {}

    float   buffer[VSNAPSHOT_MAX_SIZE];
    int     length = 0;
    float   maximum = 0;
    float   minimum = 9999999;
    float   time = 0;  // milliseconds
    float   score;    // absolute max value

  private:

    Sensor  &_sensor; // by  reference
    int     _timer = micros();
    void    _hydrate(int length);

};

#endif
