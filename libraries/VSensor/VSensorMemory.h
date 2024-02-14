/*
 * Read internal memory usage
 * Implementation:
 *
 *   #include <VSensorMemory.h>
 *
 *   VSensorMemory mem; 
 *
 *   void setup() {
 *     mem.begin();
 *   }
 *   void loop() {
 *     if (mem.update(1000)) {
 *       float memory = hard.getMemoryUsage();
 *     }
 *   }
 */

#ifndef VSensorMemory_h
#define VSensorMemory_h

#include "Arduino.h"

struct hardware_field_data {
  float memoryUsage;     // in % used
};

class VSensorMemory
{
  public:

    void begin();
    bool update(int delay);
    hardware_field_data dump() { return _data; }
    
    float getMemoryUsage() { return _data.memoryUsage; }

    float getMemoryUsageTolerance() { return 10.0; }

  private:

    struct hardware_field_data _data;
    uint32_t _timer = millis();

    float _readMemoryUsage();
};

#endif
