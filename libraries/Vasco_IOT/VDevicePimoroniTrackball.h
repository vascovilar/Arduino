/*
 * Read data on Pimoroni Trackball device
 * Ref: https://shop.pimoroni.com/products/trackball-breakout?variant=27672765038675
 * Doc: https://github.com/UnfinishedStuff/Pimoroni-trackball-documentation
 * 
 * Implementation:
 *
 *   #include <VDevicePimoroniTrackball.h>
 *
 *   VDevicePimoroniTrackball trackball(0x0A); 
 *
 *   void setup() {
 *     trackball.init();
 *   }
 * 
 *   void loop() {
 *     Serial.println(trackball.setRed(255));
 *   }
 */

#ifndef VDevicePimoroniTrackball_h
#define VDevicePimoroniTrackball_h

#include "Arduino.h"
#include "VUsePins.h"
#include "VDevice.h"


class VDevicePimoroniTrackball : public VDevice, public VUseI2cPins
{
  public:

    VDevicePimoroniTrackball(byte addr) : VDevice(TRACKBALL) { 
      _i2cAddress = addr;
    }
    
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
