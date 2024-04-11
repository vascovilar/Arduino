/*
 * Read data on Pimoroni Trackball device
 * Ref: https://shop.pimoroni.com/products/trackball-breakout?variant=27672765038675
 * Doc: https://github.com/UnfinishedStuff/Pimoroni-trackball-documentation
 * 
 * Implementation:
 *
 *   #include <MS51XB9AE.h>
 *
 *   MS51XB9AE trackball(0x0A); 
 *
 *   void setup() {
 *     trackball.init();
 *   }
 * 
 *   void loop() {
 *     Serial.println(trackball.setRed(255));
 *   }
 */

#ifndef MS51XB9AE_h
#define MS51XB9AE_h

#include "Arduino.h"
#include "interface/Device.h"
#include "plugin/Pins.h"


class MS51XB9AE : public Device, public I2cPins
{
  public:
    MS51XB9AE(byte addr) : Device(TRACKBALL) { _i2cAddress = addr; }   
    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update();
  
  private:
    byte    _i2cAddress;

    // TODO vasco implement trackball driver

};

#endif
