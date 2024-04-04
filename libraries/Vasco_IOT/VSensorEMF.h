/*
 * Read electro-magnetic field level with custom sensor (15cm antenna with 10Mohm pulldown resistance on VP pin)
 * 
 * Implementation:
 *
 *   #include <VSensorEMF.h>
 *
 *   VSensorEMF emf(36); 
 *
 *   void setup() {
 *     emf.init();
 *   }
 * 
 *   void loop() {
 *     Serial.println(emf.getMaxValue().value);
 *   }
 */

#ifndef VSensorEMF_h
#define VSensorEMF_h

#include "Arduino.h"
#include "VUsePins.h"
#include "VDevice.h"
#include "VSensor.h"

class VSensorEMF : public VDevice, public VSensor, public VUseAdcPin
{  
  static const int _ADC_MAX_VALUE = 4095;
  static const int _ADC_ZERO_VALUE = 0;

  public:

    VSensorEMF(byte pin) : VDevice(EMF_SENSOR), VSensor(true) {
      _analogPin = pin;
    };
 
    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update(); 
    float   read();

    // data updated
    vfield_data   getMaxValue() { return _data.maxValue; }
    vfield_data   getFrequency() { return _data.frequency; }

  private:

    byte     _analogPin;
    float    _maxValue = 0;

    struct fields {
      vfield_data  maxValue = {"Intensité EMF", "%", 1.0};
      vfield_data  frequency = {"Fréquence EMF", "Hz", 10.0};
    };
    fields _data;

    vlegend_data _maxValues[5] = {
      {0, VERT, "aucun champ"},
      {20, VERT, "champ faible"},
      {40, JAUNE, "champ moyen"},
      {80, ORANGE, "champ fort"},
      {100, ROUGE, "champ maximum"},
    };
    
    vlegend_data _frequencies[2] = {
        {0, VERT, "non detectée"},
        {40000, VERT, ""},
      };

};

#endif
