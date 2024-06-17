/*
 * Read data on SI1145
 * Implementation:
 *
 *   #include <VSensorLight.h>
 *
 *   VSensorLight light; 
 *
 *   void setup() {
 *     light.begin(0x60);
 *   }
 *   void loop() {
 *     if (light.update(1000)) {
 *       float uv = light.getUVIndex();
 *     }
 *   }
 */

#ifndef VSensorLight_h
#define VSensorLight_h

#include "Arduino.h"
#include "Adafruit_SI1145.h"

struct light_field_data {
  float    uvIndex;     // in UV index
  float    visible;     // in lux
  float    infraRed;    // in lux
};

enum uv_status_code {
  V_LOW_RISK = 0,
  V_MODERATE_RISK = 1,
  V_HIGH_RISK = 2,
  V_VERY_HIGH_RISK = 3,
  V_EXTREME_RISK = 4
};

class VSensorLight
{
  public:

    int   status();
    void  begin(int addr);
    bool  update(int delay);
    light_field_data dump() { return _data; }

    float getUVIndex() { return _data.uvIndex; }
    float getVisible() { return _data.visible; }
    float getInfraRed() { return _data.infraRed; }

    float getUVIndexTolerance() { return 1.0; }
    float getVisibleTolerance() { return 100.0; }
    float getInfraRedTolerance() { return 100.0; }

    uv_status_code getUVIndexStatus();    

  private:

    Adafruit_SI1145 _uv;
    struct light_field_data _data;
    uint32_t _timer = millis();
    int _status = 2;
    
    float _convertToUVIndex(float uv);
};

#endif
