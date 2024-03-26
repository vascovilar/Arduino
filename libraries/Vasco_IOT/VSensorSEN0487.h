/*
 * Read ambiant sound level with SEN0487
 * Implementation:
 *
 *   #include <VSensorSEN0487.h>
 *
 *   VSensorSEN0487 ear(39); 
 *
 *   void setup() {
 *     ear.init();
 *   }
 * 
 *   void loop() {
 *     Serial.println(ear.getMaxValue().value);
 *   }
 */

#ifndef VSensorSEN0487_h
#define VSensorSEN0487_h

#include "Arduino.h"
#include "VDevice.h"
#include "VSensor.h"

class VSensorSEN0487 : public VDevice, public VSensor, public VAnalogPin
{  
  static const int _ADC_MAX_VALUE = 4000;
  static const int _ADC_ZERO_VALUE = 1638;
  static const int _ADC_SMOOTH_FACTOR = 10;
  static const int _NOISE_THRESOLD_VALUE = 0.38;

  public:

    VSensorSEN0487(byte pin) : VDevice(FERMION_SEN0487), VSensor(pin) {};
  
    bool init();
    bool wake();
    bool sleep();
    bool sync();
    bool event();
    float check();

    field_data   getMaxValue() { return _data.maxValue; }
    
  private:
     
    struct fields {
      field_data   maxValue = {"Intensité Sonore", "%", 2.0};
    };
    fields _data;

    legend_data _maxValues[6] = {
      {0, VERT, "silence"},
      {3, VERT, "calme"},
      {10, VERT, "moyenne"},
      {50, JAUNE, "bruit"},
      {90, ORANGE, "fort"},
      {100, ROUGE, "maximale"},
    };
};

#endif
