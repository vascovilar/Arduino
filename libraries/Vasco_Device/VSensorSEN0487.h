/*
 * Read ambiant sound level with SEN0487
 * Implementation:
 *
 *   #include <VSensorSEN0487.h>
 *
 *   VSensorSEN0487 ear; 
 *
 *   void setup() {
 *     ear.begin(39);
 *   }
 * 
 *   void loop() {
 *     if (ear.update(1000)) {
 *       Serial.println(ear.getMaxValue().value);
 *     };
 *   }
 */

#ifndef VSensorSEN0487_h
#define VSensorSEN0487_h

#include "Arduino.h"
#include "VSensors.h"

class VSensorSEN0487
{  
  public:
  
    void begin(int pin); // pin must be ADC pin
    bool update(int delay); // delay in milliseconds
    void snap(); // delay in microseconds for one of the 100 measure
    float read(); // read sensor, gives a value from 0 to 4096, % returned
    void sleep(bool isSleeping);

    field_data   getMaxValue() { return _data.maxValue; }
    float*       getBuffer() { return _data.buffer; }
    unsigned int getProcessTime() { return _processTime; } // in milliseconds
    bool         isEnabled() { return _enabled; }
    
  private:
  
    unsigned int _pin = 0;
    unsigned int _timer = 0;
    unsigned int _processTime = 0;
    bool _enabled = false;
    float _max = 0;
     
    struct fields {
      field_data   maxValue = {"Intensité Sonore", "%", 2.0};
      float        buffer[100];
    };
    fields _data;

    void _setMaxValue(float value)
    {
      _data.maxValue.value = value;

      if (value <= 0) { _data.maxValue.status = VERT; _data.maxValue.text = "silence"; } 
      else if (value <= 3) { _data.maxValue.status = VERT; _data.maxValue.text = "calme"; }
      else if (value <= 10) { _data.maxValue.status = VERT; _data.maxValue.text = "moyenne"; }
      else if (value <= 50) { _data.maxValue.status = JAUNE; _data.maxValue.text = "bruit"; } 
      else if (value <= 90) { _data.maxValue.status = ORANGE; _data.maxValue.text = "forte"; } 
      else { _data.maxValue.status = ROUGE; _data.maxValue.text = "maximale"; }
    }

};

#endif
