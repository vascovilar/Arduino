#ifndef VIcon_h
#define VIcon_h

#include "Arduino.h"
#include "VSensorMeteo.h"

// resize https://www.iloveimg.com/resize-image#resize-options,pixels
// convert http://javl.github.io/image2cpp/ 
// 32x32px

const unsigned char Icon_Sun [] PROGMEM = {
  0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x21, 0x84, 0x00, 
  0x00, 0x11, 0x88, 0x00, 0x04, 0x11, 0x88, 0x20, 0x02, 0x08, 0x10, 0x40, 0x01, 0x00, 0x00, 0x80, 
  0x00, 0x81, 0x81, 0x00, 0x00, 0x0e, 0x70, 0x00, 0x10, 0x10, 0x08, 0x08, 0x0c, 0x20, 0x04, 0x30, 
  0x02, 0x40, 0x02, 0x40, 0x00, 0x40, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00, 0xfc, 0x80, 0x01, 0x3f, 
  0xfc, 0x80, 0x01, 0x3f, 0x00, 0x40, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00, 0x02, 0x40, 0x02, 0x40, 
  0x0c, 0x20, 0x04, 0x30, 0x10, 0x10, 0x08, 0x08, 0x00, 0x0e, 0x70, 0x00, 0x00, 0x81, 0x81, 0x00, 
  0x01, 0x00, 0x00, 0x80, 0x02, 0x08, 0x10, 0x40, 0x04, 0x11, 0x88, 0x20, 0x00, 0x11, 0x88, 0x00, 
  0x00, 0x21, 0x84, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00
};

const unsigned char Icon_LightningAndCloud [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x0f, 0xc2, 0x10, 0x00, 0x38, 0x70, 0x20, 
  0x00, 0x60, 0x18, 0x20, 0x00, 0x80, 0x0c, 0x02, 0x01, 0x80, 0x04, 0x0c, 0x07, 0x00, 0x02, 0x00, 
  0x0c, 0x00, 0x02, 0x00, 0x30, 0x00, 0x02, 0x06, 0x20, 0x00, 0x03, 0xf0, 0x40, 0x00, 0x00, 0x18, 
  0x40, 0x00, 0x00, 0x04, 0x80, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x01, 
  0x80, 0x07, 0xf0, 0x01, 0xc0, 0x04, 0x20, 0x01, 0x40, 0x04, 0x20, 0x01, 0x60, 0x04, 0x60, 0x02, 
  0x20, 0x08, 0x7c, 0x02, 0x18, 0x08, 0x0c, 0x04, 0x0e, 0x08, 0x08, 0x38, 0x01, 0xcf, 0x13, 0xe0, 
  0x00, 0x01, 0x20, 0x00, 0x00, 0x03, 0x60, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02, 0x80, 0x00, 
  0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char Icon_SunAndCloud [] PROGMEM = {
  0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x10, 0x41, 0x80, 0x00, 0x08, 0x01, 0x00, 
  0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x06, 0x0c, 0x08, 0x01, 0x8c, 0x03, 0x18, 
  0x00, 0x18, 0x01, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x20, 0x00, 0x80, 0x00, 0x20, 0x00, 0x40, 
  0x00, 0x20, 0x00, 0x40, 0x07, 0x3c, 0x00, 0x4c, 0x00, 0x63, 0x00, 0x40, 0x00, 0xc0, 0x80, 0x80, 
  0x00, 0x80, 0xe0, 0x80, 0x0f, 0x83, 0x19, 0x00, 0x18, 0x04, 0x07, 0x30, 0x10, 0x08, 0x06, 0x08, 
  0x10, 0x08, 0x03, 0xc0, 0x30, 0x08, 0x02, 0x20, 0x60, 0x38, 0x00, 0x10, 0x40, 0xc0, 0x00, 0x10, 
  0x41, 0x80, 0x00, 0x10, 0x61, 0x00, 0x00, 0x08, 0x33, 0x00, 0x00, 0x04, 0x0f, 0x00, 0x00, 0x04, 
  0x01, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x04, 0x00, 0xc0, 0x00, 0x08, 0x00, 0x7f, 0xff, 0xf0
};

const unsigned char Icon_Snow [] PROGMEM = {
  0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x31, 0x8c, 0x00, 0x00, 0x39, 0x9c, 0x00, 
  0x01, 0x1d, 0xb8, 0x80, 0x01, 0x0d, 0xb0, 0x80, 0x01, 0x07, 0xe0, 0x80, 0x01, 0x83, 0xc1, 0x80, 
  0x39, 0x81, 0x81, 0x9c, 0x1c, 0x81, 0x81, 0x38, 0x07, 0xc1, 0x83, 0xe0, 0x01, 0xc1, 0x83, 0x80, 
  0x0f, 0xe1, 0x87, 0xf0, 0x7e, 0x39, 0x9c, 0x7e, 0x20, 0x0f, 0xf0, 0x04, 0x00, 0x03, 0xc0, 0x00, 
  0x00, 0x03, 0xc0, 0x00, 0x20, 0x0f, 0xf0, 0x04, 0x7e, 0x39, 0x9c, 0x7e, 0x0f, 0xe1, 0x87, 0xf0, 
  0x01, 0xc1, 0x83, 0x80, 0x07, 0xc1, 0x83, 0xe0, 0x1c, 0x81, 0x81, 0x38, 0x39, 0x81, 0x81, 0x9c, 
  0x01, 0x83, 0xc1, 0x80, 0x01, 0x07, 0xe0, 0x80, 0x01, 0x0d, 0xb0, 0x80, 0x01, 0x1d, 0xb8, 0x80, 
  0x00, 0x39, 0x9c, 0x00, 0x00, 0x31, 0x8c, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00
};

const unsigned char Icon_RainAndCloud [] PROGMEM = {
  0x00, 0x01, 0xc0, 0x00, 0x00, 0x0e, 0x38, 0x00, 0x00, 0x18, 0x0c, 0x00, 0x00, 0x70, 0x07, 0x80, 
  0x01, 0x80, 0x00, 0x60, 0x02, 0x20, 0x00, 0x10, 0x04, 0xc0, 0x00, 0x10, 0x09, 0x00, 0x00, 0x08, 
  0x0a, 0x00, 0x00, 0x08, 0x0a, 0x00, 0x00, 0x08, 0x30, 0x00, 0x00, 0x0c, 0x60, 0x00, 0x00, 0x06, 
  0xc0, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 
  0xc0, 0x00, 0x00, 0x03, 0x40, 0x00, 0x00, 0x02, 0x30, 0x00, 0x00, 0x0c, 0x0c, 0x0f, 0xff, 0xf0, 
  0x02, 0x08, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x82, 0x00, 
  0x0c, 0x20, 0x86, 0x10, 0x08, 0x21, 0x04, 0x10, 0x10, 0x41, 0x0c, 0x20, 0x00, 0x00, 0x00, 0x00, 
  0x02, 0x08, 0x61, 0x00, 0x06, 0x10, 0x41, 0x00, 0x04, 0x10, 0x82, 0x00, 0x00, 0x00, 0x82, 0x00
};

const unsigned char Icon_DryDesert [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x02, 0x40, 0x00, 
  0x00, 0x04, 0x20, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x02, 0x40, 0x00, 
  0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x06, 0x40, 0x00, 0x00, 0x08, 0xd0, 0x00, 
  0x00, 0x11, 0x98, 0x00, 0x00, 0x13, 0x18, 0x00, 0x00, 0x26, 0x14, 0x00, 0x00, 0x24, 0x14, 0x00, 
  0x08, 0x08, 0x30, 0x10, 0x04, 0x48, 0x32, 0x20, 0x03, 0x18, 0x78, 0xc0, 0x0c, 0x20, 0x8c, 0x30, 
  0x02, 0xe1, 0x0f, 0x40, 0x00, 0x3e, 0x3c, 0x00, 0x02, 0x10, 0x08, 0x40, 0x01, 0x88, 0x31, 0x80, 
  0x00, 0x46, 0x62, 0x00, 0x00, 0x37, 0xec, 0x00, 0x00, 0x0c, 0xf0, 0x00, 0x00, 0x07, 0x20, 0x00, 
  0x00, 0x0f, 0xf0, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const unsigned char Icon_Night [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 
  0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 
  0x00, 0xc0, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x01, 0xe0, 0x10, 0x00, 
  0x00, 0xc0, 0x18, 0x00, 0x00, 0xc0, 0x18, 0x00, 0x00, 0xc0, 0x18, 0x00, 0x00, 0x00, 0x3c, 0x00, 
  0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x0f, 0xff, 0xf0, 
  0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x18, 0x00, 
  0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

class VIcon
{
  public:
    
    const unsigned char* night() { return Icon_Night; } 
    const unsigned char* lightningAndCloud() { return Icon_LightningAndCloud; } 
    const unsigned char* rainAndCloud() { return Icon_RainAndCloud; } 
    const unsigned char* sunAndCloud() { return Icon_SunAndCloud; } 
    const unsigned char* sun() { return Icon_Sun; } 
    const unsigned char* dryDesert() { return Icon_DryDesert; } 
};

#endif
