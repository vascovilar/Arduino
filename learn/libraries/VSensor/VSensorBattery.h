/*
 * Read battery level with custom sensor
 * Implementation:
 *
 *   #include <VSensorBattery.h>
 *
 *   const int BRIDGEPIN = 5;
 *
 *   VSensorBattery battery; 
 *
 *   void setup() {
 *     battery.begin(BRIDGEPIN);
 *   }
 *   void loop() {
 *     if (battery.update(100)) {
 *       float v = battery.getVoltage();
 *     };
 *   }
 */

#ifndef VSensorBattery_h
#define VSensorBattery_h

#include "Arduino.h"

struct battery_field_data {
  float    voltage;    // in volt
};

class VSensorBattery
{
  public:
    void begin(int bridgePin);
    bool update(int delay);

    battery_field_data dump() { return _data; }
    float getVoltage() { return _data.voltage; }

  private:
    int _bridgePin;
    struct battery_field_data _data;
    uint32_t _timer = millis();
    
    float _read();
};

#endif
