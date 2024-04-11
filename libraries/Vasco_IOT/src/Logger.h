/*
 * Read uv index and visible light with LTR390
 * Implementation:
 *
 *   #include <Logger.h>
 *
 *   Logger log; 
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

#ifndef Logger_h
#define Logger_h

#include "Arduino.h"
#include "interface/Run.h"


class Logger : public Run
{
  static const int DEFAULT_UPDATE_TIME = 1000;

  public:
    // interface
    bool    begin(vrun mode);
    bool    run();  
    // api
    void    println(String text);
    void    print(String text);
    String  dump(int delay);

  private:
    long    _timer = 0;
    void    _addHistory(String text);
    
    struct message {
      long    time;
      String  text;
    };
    message _history[20];
};

#endif