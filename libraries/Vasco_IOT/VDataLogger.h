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
    bool update(int delay);
    
    void println(String text);
    void print(String text);

    String dump(int delay);

  private:

    long _timer = 0;

    void _addHistory(String text);
    
    struct message {
      String  text;
      long    time;
    };
    message _history[20];
};

#endif