/*
 * Read data on Pimoroni Trackball device
 * Ref: https://shop.pimoroni.com/products/trackball-breakout?variant=27672765038675
 * Doc: https://www.nuvoton.com/export/resource-files/DS_MS51FB9AE_MS51XB9AE_MS51XB9BE_EN_Rev1.pdf
 * 
 * Modify library:
 * 
 *   File pimoroniTrackball.h, line 73
 *   -> comment line 'extern pimoroniTrackball trackball;'
 *   
 *   File pimoroniTrackball.cpp line 191
 *   -> comment line 'pimoroniTrackball trackball;'
 * 
 * Implementation:
 *
 *   #include <PIM447.h>
 *
 *   PIM447 trackball(0x0A); 
 *
 *   void setup() {
 *     trackball.init();
 *   }
 * 
 *   void loop() {
 *     Serial.println(trackball.led(0xCC0000, 64));
 *   }
 */

#ifndef PIM447_h
#define PIM447_h

#include "Arduino.h"
#include "interface/Run.h"
#include "plugin/Pins.h"
#include "interface/Data.h"
#include "interface/Device.h"
#include "interface/Sensor.h"
#include "Wire.h"
#include "pimoroniTrackball.h"


class PIM447 : public Run, public I2cPins
{
  public:

    PIM447(byte addr) { _i2cAddress = addr; }   
    // interface
    bool     begin(vrun mode);
    bool     run();
    // api
    void     led(int hex, byte brightness);
    void     led(vstatus code);
    void     setBoundary(int width, int height);
    vpointer getPointer(); 
    void     setPointer(int x, int y);
    void     resetPointer();

  private:
  
    long              _timer = 0;
    pimoroniTrackball _trackBall = pimoroniTrackball();
    byte              _i2cAddress;
    int               _width;
    int               _height;
    int               _x;
    int               _y;
    bool              _focus;
    bool              _click;
    bool              _left;
    bool              _right;
    bool              _up;
    bool              _down;
};

#endif
