/*
 * Read data on AH49E hall sensor
 * Ref: https://www.gotronic.fr/pj2-sen-ky035-manual-1934.pdf
 * Doc: https://www.diodes.com/assets/Datasheets/AH49E.pdf
 * 
 * Implementation:
 *
 *   #include <VSensorAH49E.h>
 *
 *   VSensorAH49E gauss(35); 
 *
 *   void setup() {
 *     gauss.init();
 *   }
 * 
 *   void loop() {
 *     Serial.println(gauss.getMaxValue().value);
 *   }
 */

#ifndef VSensorAH49E_h
#define VSensorAH49E_h

#include "Arduino.h"
#include "VUsePins.h"
#include "VDevice.h"
#include "VSensor.h"

class VSensorAH49E : public VDevice, public VSensor, public VUseAdcPin
{
  public:

    VSensorAH49E(byte pin) : VDevice(TRACKBALL), VSensor(true) { 
      _analogPin = pin;
    }
    
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
