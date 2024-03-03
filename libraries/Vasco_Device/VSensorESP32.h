/*
 * Read ESP32 info
 * Implementation:
 *
 *   #include <VSensorESP32.h>
 *
 *   VSensorESP32 esp; 
 *
 *   void setup() {
 *     esp.begin();
 *   }
 * 
 *   void loop() {
 *     if (esp.update(1000)) {
 *       Serial.println(esp.getLoad().value);
 *     };
 *   }
 */

#ifndef VSensorESP32_h
#define VSensorESP32_h

#include "Arduino.h"
#include "VSensors.h"

class VSensorESP32
{  
  public:
  
    void begin();
    bool update(int delay);
    void sync();
    void sleep(bool isSleeping);
    
    field_data   getLoad() { return _data.load; }
    field_data   getMemory() { return _data.memory; }
    unsigned int getProcessTime() { return _processTime; }
    bool         isEnabled() { return _enabled; }
        
  private:
  
    unsigned int _timer = 0;
    unsigned int _processTime = 0;
    bool _enabled = false;

    float _readMemoryUsage();
  
    struct fields {
      field_data   load = {"Charge ESP", "%", 20.0};
      field_data   memory = {"Mémoire utilisée", "%", 20.0};
    };
    fields _data;

    void _setLoad(float value)
    { 
      _data.load.value = value;

      if (value <= 90) { _data.load.status = VERT; _data.load.text = "ok"; } 
      else if (value <= 100) { _data.load.status = JAUNE; _data.load.text = "chargé"; } 
      else { _data.load.status = ROUGE; _data.load.text = "attention"; }
    }

    void _setMemory(float value)
    {
      _data.memory.value = value;
      
      if (value <= 90) { _data.memory.status = VERT; _data.memory.text = "ok"; } 
      else if (value <= 100) { _data.memory.status = JAUNE; _data.memory.text = "chargé"; } 
      else { _data.memory.status = ROUGE; _data.memory.text = "attention"; }
    }
};

#endif
