/*
 * Read ambiant sound level with DFRobot SEN0487
 * Ref: https://wiki.dfrobot.com/Fermion_MEMS_Microphone_Sensor_SKU_SEN0487
 * Doc: https://dfimg.dfrobot.com/nobody/wiki/5160bfe4d49deff484e1bd66a44d743a.pdf
 * 
 * Implementation:
 *
 *   #include <SEN0487.h>
 *
 *   SEN0487 ear(39); 
 *
 *   void setup() {
 *     ear.init();
 *   }
 * 
 *   void loop() {
 *     Serial.println(ear.getMaxValue().value);
 *   }
 */

#ifndef SEN0487_h
#define SEN0487_h

#include "Arduino.h"
#include "interface/Device.h"
#include "interface/Sensor.h"
#include "plugin/Pins.h"

class SEN0487 : public Device, public Sensor, public AdcPin
{  
  static const int  _ADC_MAX_VALUE = 4000;
  static const int  _ADC_ZERO_VALUE = 1638;
  static const int  _NOISE_THRESOLD_VALUE = 0.38;

  public:

    SEN0487(byte pin) : Device(MICROPHONE_SENSOR), Sensor(true) { _analogPin = pin; }
    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update();
    float   read();
    // data updated
    vfield  getMaxValue() { return _data.maxValue; }
    
  private:
  
    byte     _analogPin;
    float    _maxValue = 0;
     
    // human readable
    struct fields {
      vfield   maxValue = {"Intensité Sonore", "%", 1.0};
    };
    fields _data;

    vlegend _maxValues[6] = {
      {0, VERT, "silence"},
      {3, VERT, "calme"},
      {10, VERT, "moyenne"},
      {50, JAUNE, "bruit"},
      {90, ORANGE, "fort"},
      {100, ROUGE, "maximale"},
    };
};

#endif