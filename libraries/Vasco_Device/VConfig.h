#ifndef VConfig_h
#define VConfig_h

#define VSENSORS_COUNT  9

enum comfort_code {
  GRIS = 0,   // rien
  VERT = 1,   // confort
  JAUNE = 2,  // perturbé
  ORANGE = 3, // gêne
  ROUGE = 4,  // mauvais
  VIOLET = 5  // danger
};

struct field_data {
  String        label;
  String        unit;
  float         tolerance;
  float         value;
  comfort_code  status;
  String        text;
};

enum sensors_code {
  TEMPERATURE = 0,
  PRESSURE = 1,
  HUMIDITY = 2,
  AIR_QUALITY = 3,
  GAS_PERCENTAGE = 4,
  EMF_FIELD = 5,
  UV_INDEX = 6,
  VISIBLE = 7,
  ESP_LOAD = 8
};

#endif