/*
 * Read electro-magnetic field level with custom sensor (15cm antenna with 10Mohm pulldown resistance on VP pin)
 * 
 * Implementation:
 *
 *   #include <EMF001.h>
 *
 *   EMF001 emf(36); 
 *
 *   void setup() {
 *     emf.init();
 *   }
 * 
 *   void loop() {
 *     Serial.println(emf.getMaxValue().value);
 *   }
 */

#ifndef EMF001_h
#define EMF001_h

#include "Arduino.h"
#include "interface/Device.h"
#include "interface/Sensor.h"
#include "plugin/Pins.h"


class EMF001 : public Device, public Sensor, public AdcPin
{  
  static const int  _ADC_MAX_VALUE = 4095;
  static const int  _ADC_ZERO_VALUE = 0;

  public:

    EMF001(byte pin) : Device(EMF_SENSOR), Sensor(true) { _analogPin = pin; }
    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update(); 
    float   read();
    // data updated
    vfield  getMaxValue() { return _data.maxValue; }
    vfield  getFrequency() { return _data.frequency; }

  private:
  
    byte    _analogPin;
    float   _maxValue = 0;

    // human readable
    struct fields {
      vfield   maxValue = {"Intensité EMF", "%", 1.0};
      vfield   frequency = {"Fréquence EMF", "Hz", 10.0};
    };
    fields _data;

    vlegend _maxValues[5] = {
      {0, VERT, "aucun champ"},
      {20, VERT, "champ faible"},
      {40, JAUNE, "champ moyen"},
      {80, ORANGE, "champ fort"},
      {100, ROUGE, "champ maximum"},
    };
    
    vlegend _frequencies[2] = {
        {0, VERT, "non detectée"},
        {40000, VERT, ""},
      };

};

#endif
