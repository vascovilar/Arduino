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
#include "VEnum.h"
#include "driver/adc.h"

class VSensorEMF
{  
  public:
  
    void begin(int pin); // pin must be 36
    bool update(int delay); // delay in milliseconds
    void sync(int delay); // delay in microseconds for one of the 100 measure
    
    field_data  getMaxValue() { return _data.maxValue; } // % of sensor value
    field_data  getFrequency() { return _data.frequency; } // in Hz
    float*      getBuffer() { return _data.buffer; } // 100 lines of raw data in % of sensor value
    int         getMeasureTime() { return _data.measureTime; } // in milliseconds
    
  private:
  
    unsigned int _timer = 0;
    
    float _read(); // read sensor, gives a value from 0 to 4096, % returned
  
    struct fields {
      field_data  maxValue = {"Intensité EMF", "%", 5.0};
      field_data  frequency = {"Fréquence EMF", "Hz", 10.0};
      float       buffer[100] = {};
      int         measureTime = 0;
    };
    fields _data;

    field_data _setMaxValue(field_data field, float value)
    {
      field.value = value;

      if (value <= 0) { field.status = GRIS; field.text = "aucun champ"; } 
      else if (value <= 10) { field.status = VERT; field.text = "champ faible"; } 
      else if (value <= 40) { field.status = JAUNE; field.text = "champ moyen"; } 
      else if (value <= 80) { field.status = ORANGE; field.text = "champ fort"; } 
      else { field.status = ROUGE; field.text = "champ maximum"; }

      return field;
    }

    field_data _setFrequency(field_data field, float value)
    {
      field.value = value;

      if (value <= 0) { field.status = GRIS; field.text = "non detectée"; } 
      else { field.status = VERT; field.text = ""; }
      
      return field;
    }
};

#endif
