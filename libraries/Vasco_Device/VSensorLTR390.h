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
    void sleep(bool isSleeping);
    
    field_data   getUvIndex() { return _data.uvIndex; }
    field_data   getVisible() { return _data.visible; }
    unsigned int getProcessTime() { return _data.processTime; }

  private:
  
    Adafruit_LTR390 _ltr;
    unsigned int _timer = 0;
    bool _enabled = false;

    float _readUVS(); // read sensor value in UVI
    float _readALS(); // reans sensor value in Lux
  
    struct fields {
      field_data   uvIndex = {"Index UV", "", 1.0};
      field_data   visible = {"Lumière visible", "lux", 10.0};
      unsigned int processTime;      
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
      
      if (value <= 0) { _data.visible.status = ORANGE; _data.visible.text = "obscurité"; } 
      else if (value <= 1) { _data.visible.status = JAUNE; _data.visible.text = "minimum humain"; } 
      else if (value <= 20) { _data.visible.status = JAUNE; _data.visible.text = "pénombre"; } 
      else if (value <= 100) { _data.visible.status = VERT; _data.visible.text = "tamisée"; } 
      else if (value <= 500) { _data.visible.status = VERT; _data.visible.text = "confortable"; } 
      else if (value <= 1000) { _data.visible.status = VERT; _data.visible.text = "bureau"; } 
      else if (value <= 10000) { _data.visible.status = VERT; _data.visible.text = "ciel couvert"; } 
      else if (value <= 50000) { _data.visible.status = VERT; _data.visible.text = "ciel nuageux"; } 
      else if (value <= 100000) { _data.visible.status = JAUNE; _data.visible.text = "ciel d'été"; } 
      else { _data.visible.status = ORANGE; _data.visible.text = "trop lumineux"; }
    }
};

#endif
