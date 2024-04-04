/*
 * Read ambiant sound level with DFRobot SEN0487
 * Ref: https://wiki.dfrobot.com/Fermion_MEMS_Microphone_Sensor_SKU_SEN0487
 * Doc: https://dfimg.dfrobot.com/nobody/wiki/5160bfe4d49deff484e1bd66a44d743a.pdf
 * 
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
#include "VUsePins.h"
#include "VDevice.h"
#include "VSensor.h"

class VSensorSEN0487 : public VDevice, public VSensor, public VUseAdcPin
{  
  static const int _ADC_MAX_VALUE = 4000;
  static const int _ADC_ZERO_VALUE = 1638;
  static const int _NOISE_THRESOLD_VALUE = 0.38;

  public:

    VSensorSEN0487(byte pin) : VDevice(MICROPHONE_SENSOR), VSensor(true) {
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
    
  private:

    byte     _analogPin;
    float    _maxValue = 0;
     
    struct fields {
      vfield_data   maxValue = {"Intensité Sonore", "%", 2.0};
    };
    fields _data;

    vlegend_data _maxValues[6] = {
      {0, VERT, "silence"},
      {3, VERT, "calme"},
      {10, VERT, "moyenne"},
      {50, JAUNE, "bruit"},
      {90, ORANGE, "fort"},
      {100, ROUGE, "maximale"},
    };
};

#endif
