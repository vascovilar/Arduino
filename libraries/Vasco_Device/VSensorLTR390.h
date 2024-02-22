/*
 * Read uv index and visible light with LTR390
 * Implementation:
 *
 *   #include <VSensorLTR390.h>
 *
 *   VSensorLTR390 light; 
 *
 *   void setup() {
 *     light.begin();
 *   }
 * 
 *   void loop() {
 *     if (light.update(1000)) {
 *       Serial.println(light.getUvIndex().value);
 *     };
 *   }
 */

#ifndef VSensorLTR390_h
#define VSensorLTR390_h

#include "Arduino.h"
#include "VConfig.h"
#include "Adafruit_LTR390.h"

class VSensorLTR390
{  
  public:
  
    void begin(int addr); // i2c must be 0x53
    bool update(int delay); // delay in milliseconds
    void sync();
    
    field_data  getUvIndex() { return _data.uvIndex; }
    field_data  getVisible() { return _data.visible; }
    int         getProcessTime() { return _processTime; } // in milliseconds

  private:
  
    Adafruit_LTR390 _ltr;
    unsigned int _timer;
    unsigned int _processTime;
  
    struct fields {
      field_data  uvIndex = {"Index UV", "", 1.0};
      field_data  visible = {"Lumière visible", "lux", 10.0};
    };
    fields _data;

    void _setUvIndex(float value)
    { 
      _data.uvIndex.value = value;

      if (value <= 2) { _data.uvIndex.status = VERT; _data.uvIndex.text = "risque faible"; } 
      else if (value <= 5) { _data.uvIndex.status = JAUNE; _data.uvIndex.text = "risque modéré"; } 
      else if (value <= 7) { _data.uvIndex.status = ORANGE; _data.uvIndex.text = "risque élevé"; } 
      else if (value <= 10) { _data.uvIndex.status = ROUGE; _data.uvIndex.text = "risque très élevé"; } 
      else { _data.uvIndex.status = VIOLET; _data.uvIndex.text = "risque extrême"; }
    }

    void _setVisible(float value)
    { 
      _data.visible.value = value;
      _data.visible.status = VERT;
    }
};

#endif
