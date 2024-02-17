#ifndef VCommon_h
#define VCommon_h

#include "Arduino.h"

//#define BUFFER_ARRAY_LENGTH  120

enum comfort_code {
  GRIS = 0,   // rien
  VERT = 1,   // confort
  JAUNE = 2,  // perturbé
  ORANGE = 3, // gêne
  ROUGE = 4,  // mauvais
  VIOLET = 5  // danger
};

struct field_data {
  float         value;
  String        label;
  String        unit;
  comfort_code  status;
  String        text;
};

enum sensors_code {
  TEMPERATURE = 0,
  PRESSURE = 1,
  HUMIDITY = 2,
  AIR_QUALITY = 3,
  CO2_EQUIVALENT = 4,
  VOC_EQUIVALENT = 5,
  UV_INDEX = 6,
  VISIBLE = 7,
  EMF_FIELD = 8,
  EMF_FREQUENCY = 9,
  MEMORY = 10
};

#endif