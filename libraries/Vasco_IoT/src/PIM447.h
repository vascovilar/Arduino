/*
 * Read data on Pimoroni Trackball device
 * Ref: https://shop.pimoroni.com/products/trackball-breakout?variant=27672765038675
 * Doc: https://www.nuvoton.com/export/resource-files/DS_MS51FB9AE_MS51XB9AE_MS51XB9BE_EN_Rev1.pdf
 */

#ifndef PIM447_h
#define PIM447_h

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Device.h"
#include "interface/Pointer.h"
#include "interface/Screen.h"
#include "component/Pins.h"
#include "Wire.h"
#include "pimoroniTrackball.h"


class PIM447 : public Device, public Pointer, public I2cPins, public Data
{
  public:

    PIM447(byte addr) : Device(POINTER) { _i2cAddress = addr; }

    // interface
    bool      init();
    bool      sleep();
    bool      wake();
    bool      check();
    bool      update();
    vpointer  get();
    void      set(vpointer pointer);
    void      reset();

    // api
    void      led(int hex, byte brightness = 0); // this led dont use PWM, it's a 3 color onboarded led
    void      led(vstatus code);
    int       getMagnitude() { return _magnitude; } // 0~100%

  private:

    pimoroniTrackball _trackBall = pimoroniTrackball();
    byte              _i2cAddress;
    int               _width = VSCREEN_WIDTH;
    int               _height = VSCREEN_HEIGHT;
    int               _x = 0;
    int               _y = 0;
    bool              _focus = false;
    bool              _click = false;
    bool              _left = false;
    bool              _right = false;
    bool              _up = false;
    bool              _down = false;
    int               _magnitude = 0;
};

#endif
