/*
 * Read uv index and visible light with LTR390
 * Implementation:
 *
 *   #include <VDataLogger.h>
 *
 *   VDataLogger log; 
 *
 *   void setup() {
 *     log.begin();
 *   }
 * 
 *   void loop() {
 *     log.println("test");
 *     delay(1000);
 *   }
 */

#ifndef VDataLogger_h
#define VDataLogger_h

#include "Arduino.h"

class VDataLogger
{
  public:

    void start();
    bool update(unsigned int delay);
    
    void println(String text);
    void print(String text);

    String dump(unsigned int delay);

  private:

    unsigned int _timer = 0;

    void _addHistory(String text);
    
    struct message {
      String        text;
      unsigned int  time;
    };
    message _history[20];
};

#endif