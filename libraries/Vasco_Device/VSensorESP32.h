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
#include "VConfig.h"

class VSensorESP32
{  
  public:
  
    void begin();
    bool update(int delay);
    void sync();
    void sleep(bool isSleeping);
    
    field_data  getLoad() { return _data.load; }
    int         getProcessTime() { return _data.processTime; } // in milliseconds
        
  private:
  
    unsigned int _timer;
    bool _enabled = true;
  
    struct fields {
      field_data   load = {"Charge ESP", "%", 20.0};
      unsigned int processTime;
    };
    fields _data;

    void _setLoad(float value)
    { 
      _data.load.value = value;

      if (value <= 0) { _data.load.status = VERT; _data.load.text = "éteint"; } 
      else if (value <= 90) { _data.load.status = VERT; _data.load.text = "ok"; } 
      else if (value <= 100) { _data.load.status = JAUNE; _data.load.text = "chargé"; } 
      else { _data.load.status = ROUGE; _data.load.text = "attention"; }
    }
};

#endif
