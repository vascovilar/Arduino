/*
 * Read VDeviceJST2765 thumbstick
 * Implementation:
 *
 *   #include <VDeviceJST2765.h>
 *
 *   VDeviceJST2765 joy; 
 *
 *   void setup() {
 *     joy.begin(34, 39, 0);
 *   }
 * 
 *   void loop() {
 *     if (joy.update(10)) {
 *       Serial.println(String(joy.getX()) + "    " + String(joy.getY()) + "     " + String(joy.getClick()));
 *     };
 *   }
 */

#ifndef VDeviceJST2765_h
#define VDeviceJST2765_h

#include "Arduino.h"

class VDeviceJST2765
{  
  public:
  
    void begin(int xpin, int ypin, int clickpin);
    bool update(int delay);
    
    int   getX() { return (int) _x; }
    int   getY() { return (int) _y; }
		bool  getClick() { return _click; }
        
  private:
  
    int _xpin;
    int _ypin;
    int _clickpin;
    
		unsigned int _timer = 0;
    float _x = 0;
    float _y = 0;
		bool _click = false;
		int _xo = 0;
		int _yo = 0;
		int _xmax = 0;
		int _ymax = 0;

		float _readX(); // read in %
		float _readY();
		bool _readClick();
		float _linearize(int value, bool axis); // 1 for X, 0 for Y
};

#endif
