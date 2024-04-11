/*
 * Read data on AH49E hall sensor
 * Ref: https://www.gotronic.fr/pj2-sen-ky035-manual-1934.pdf
 * Doc: https://www.diodes.com/assets/Datasheets/AH49E.pdf
 * 
 * Implementation:
 *
 *   #include <AH49E.h>
 *
 *   AH49E gauss(35); 
 *
 *   void setup() {
 *     gauss.init();
 *   }
 * 
 *   void loop() {
 *     Serial.println(gauss.getMaxValue().value);
 *   }
 */

#ifndef AH49E_h
#define AH49E_h

#include "Arduino.h"
#include "interface/Device.h"
#include "interface/Sensor.h"
#include "plugin/Pins.h"


class AH49E : public Device, public Sensor, public AdcPin
{
  public:
    AH49E(byte pin) : Device(GAUSS_SENSOR), Sensor(true) { _analogPin = pin; }
    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update();

  private:
    byte    _analogPin;
    
    // TODO vasco implement hall sensor driver

};

#endif
