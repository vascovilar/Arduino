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
 *     Serial.println(trackball.led(0xCC0000, 128));
 *   }
 */

#ifndef PIM447_h
#define PIM447_h

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Device.h"
#include "plugin/Pins.h"
#include "Wire.h"
#include "pimoroniTrackball.h"


class PIM447 : public Device, public I2cPins
{
  public:

    PIM447(byte addr) : Device(POINTER) { _i2cAddress = addr; }

    // interface
    bool     init();
    bool     sleep();
    bool     wake();
    bool     check();
    bool     update();

    // data updated
    vmouse getMouse() { return vmouse {_x, _y, _focus, _click, _left, _right, _up, _down}; }

    // api
    void     led(int hex, byte brightness);
    void     led(vstatus code);
    void     setBoundary(int width, int height);
    void     setMouse(int x, int y);
    void     resetMouse();

  private:

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
