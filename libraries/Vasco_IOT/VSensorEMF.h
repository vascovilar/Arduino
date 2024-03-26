/*
 * Read electro-magnetic field level with custom sensor
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
#include "VDevice.h"
#include "VSensor.h"

class VSensorEMF : public VDevice, public VSensor, public VAnalogPin
{  
  static const int _ADC_MAX_VALUE = 4095;
  static const int _ADC_ZERO_VALUE = 0;
  static const int _ADC_SMOOTH_FACTOR = 0;

  public:

    VSensorEMF(byte pin) : VDevice(CUSTOM_EMF), VSensor(pin) {};
 
    bool init();
    bool wake();
    bool sleep();
    bool sync();
    bool event();
    float check(); 
    
    field_data   getMaxValue() { return _data.maxValue; }
    field_data   getFrequency() { return _data.frequency; }

  private:

    struct fields {
      field_data   maxValue = {"Intensité EMF", "%", 1.0};
      field_data   frequency = {"Fréquence EMF", "Hz", 10.0};
    };
    fields _data;

    legend_data _maxValues[5] = {
      {0, VERT, "aucun champ"},
      {20, VERT, "champ faible"},
      {40, JAUNE, "champ moyen"},
      {80, ORANGE, "champ fort"},
      {100, ROUGE, "champ maximum"},
    };
    
    legend_data _frequencies[2] = {
        {0, VERT, "non detectée"},
        {40000, VERT, ""},
      };

};

#endif
