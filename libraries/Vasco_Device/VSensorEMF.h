/*
 * Read electro-magnetic field level with custom sensor
 * Implementation:
 *
 *   #include <VSensorEMF.h>
 *
 *   VSensorEMF emf; 
 *
 *   void setup() {
 *     emf.begin(36);
 *   }
 * 
 *   void loop() {
 *     if (emf.update(1000)) {
 *       Serial.println(emf.getFrequency().value);
 *     };
 *   }
 * 
 *   (or)
 * 
 *   void loop() {
 *     if (emf.update(1000)) {
 *       float* buffer = emf.getBuffer();
 *       for (int i = 0; i<100; i++) {
 *         Serial.println(buffer[i]);
 *       } 
 *     }
 *   }
 */

#ifndef VSensorEMF_h
#define VSensorEMF_h

#include "Arduino.h"
#include "VSensors.h"
#include "driver/adc.h"

class VSensorEMF
{  
  public:
  
    void begin(int pin); // pin must be 36
    bool update(int delay); // delay in milliseconds
    void snap(); // delay in microseconds for one of the 100 measure
    float read(); // read sensor, gives a value from 0 to 4096, % returned
    void sleep(bool isSleeping);

    field_data   getMaxValue() { return _data.maxValue; }
    field_data   getFrequency() { return _data.frequency; }
    float*       getBuffer() { return _data.buffer; }
    unsigned int getProcessTime() { return _processTime; } // in milliseconds
    bool         isEnabled() { return _enabled; }

    
  private:
  
    unsigned int _timer = 0;
    unsigned int _processTime = 0;
    bool _enabled = false;
      
    struct fields {
      field_data   maxValue = {"Intensité EMF", "%", 1.0};
      field_data   frequency = {"Fréquence EMF", "Hz", 10.0};
      float        buffer[100];
    };
    fields _data;

    void _setMaxValue(float value)
    {
      _data.maxValue.value = value;

      if (value <= 0) { _data.maxValue.status = VERT; _data.maxValue.text = "aucun champ"; } 
      else if (value <= 20) { _data.maxValue.status = VERT; _data.maxValue.text = "champ faible"; } 
      else if (value <= 40) { _data.maxValue.status = JAUNE; _data.maxValue.text = "champ moyen"; } 
      else if (value <= 80) { _data.maxValue.status = ORANGE; _data.maxValue.text = "champ fort"; } 
      else { _data.maxValue.status = ROUGE; _data.maxValue.text = "champ maximum"; }
    }

    void _setFrequency(float value)
    {
      _data.frequency.value = value;

      if (value <= 0) { _data.frequency.status = VERT; _data.frequency.text = "non detectée"; } 
      else { _data.frequency.status = VERT; _data.frequency.text = ""; }
    }
};

#endif
