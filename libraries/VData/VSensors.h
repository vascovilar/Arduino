/**
 * 
 * 
 * 
 */

#ifndef VSensors_h
#define VSensors_h

#include "Arduino.h"

enum sensor_field_code {
  TEMPERATURE = 0,    // in °C
  PRESSURE = 1,       // in hPa 
  HUMIDITY = 2,       // in %
  AIR_QUALITY = 3,    // index
  CO2_EQUIVALENT = 4, // in ppm
  VOC_EQUIVALENT = 5, // in ppm
  UV_INDEX = 6,       // in UV index
  VISIBLE = 7,        // in lux
  INFRARED = 8,       // in lux
  EMF = 9             // index
};

#endif
