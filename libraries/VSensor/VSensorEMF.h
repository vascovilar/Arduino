/*
 * Read electro-magnetic field level with custom sensor
 * Implementation:
 *
 *   #include <VSensorEMF.h>
 *
 *   VSensorEMF emf; 
 *
 *   void setup() {
 *     emf.begin(36);
 *   }
 *   void loop() {
 *     if (emf.update(100)) {
 *       float g = emf.getGauss();
 *     };
 *   }
 */

#ifndef VSensorEMF_h
#define VSensorEMF_h

#include "Arduino.h"
#include "driver/adc.h"

struct emf_field_data {
  float    gauss;    // index
};

class VSensorEMF
{
  public:
    void begin(int antenna);
    bool update(int delay);
    float* snap();
    
    emf_field_data dump() { return _data; }
    float getGauss() { return _data.gauss; }
    
  private:
    emf_field_data _data;
    uint32_t _timer = millis();
    float _max = 0;
    float _buffer[100];

    float _read();
};

#endif
