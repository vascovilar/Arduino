/*
 * Read electro-magnetic field level with custom sensor
 * Implementation:
 *
 *   #include <VSensorEMF.h>
 *
 *   const int ANTENNAPIN = 5;
 *
 *   VSensorEMF emf; 
 *
 *   void setup() {
 *     battery.begin(ANTENNAPIN);
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

struct emf_field_data {
  float    gauss;    // in gauss
};

class VSensorEMF
{
  public:
    void begin(int antennaPin);
    bool update(int delay);

    emf_field_data dump() { return _data; }
    float getGauss() { return _data.gauss; }

  private:
    int _antennaPin;
    emf_field_data _data;
    uint32_t _timer = millis();
    
    float _read();
};

#endif
