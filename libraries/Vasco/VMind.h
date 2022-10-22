/**
 * 
 * 
 * 
 */

#ifndef VMind_h
#define VMind_h

#include "Arduino.h"
#include "VBuffer.h"
#include "VSensorMeteo.h"
#include "VSensorLight.h"

#define MIND_SENSITIVITY 12

enum sensor_field_code {
  TEMPERATURE = 0,    // in °C
  PRESSURE = 1,       // in hPa 
  HUMIDITY = 2,       // in %
  AIR_QUALITY = 3,    // index
  CO2_EQUIVALENT = 4, // in ppm
  VOC_EQUIVALENT = 5, // in ppm
  UV_INDEX = 6,       // in UV index
  VISIBLE = 7,        // in lux
  INFRARED = 8        // in lux
};

enum mind_comfort_code {
  GRIS = 0,   // inactif
  VERT = 1,   // confort
  JAUNE = 2,  // perturbé
  ORANGE = 3, // gêne
  ROUGE = 4,  // mauvais
  VIOLET = 5  // danger
};

struct mind_graph_data {
  float    value;
  float    maximum;
  float    minimum;
  float    average;
  float    delta;
  float    tolerance;
  float    top;
  float    bottom;
  int      status;
  String   color;
  String   background;
  String   comment;
};

class VMind
{
  public:

    void analyse(int field, int status, float* values);
    mind_graph_data info(int field);
    
    // conversions by polymorphism
    mind_comfort_code get(temperature_status_code code) { return mind_temperature_comfort[int(code)]; }
    mind_comfort_code get(pressure_status_code code)    { return mind_pressure_comfort[int(code)]; }
    mind_comfort_code get(humidity_status_code code)    { return mind_humidity_comfort[int(code)]; }
    mind_comfort_code get(air_status_code code)         { return mind_air_comfort[int(code)]; }
    mind_comfort_code get(uv_status_code code)          { return mind_uv_comfort[int(code)]; }
    
    // tool
    mind_comfort_code find(int code, int value) 
    {
      switch (code) 
      {
        case TEMPERATURE:
          return get((temperature_status_code) value);
        case PRESSURE:
          return get((pressure_status_code) value);
        case HUMIDITY:
          return get((humidity_status_code) value);
        case AIR_QUALITY:
          return get((air_status_code) value);
        case UV_INDEX:
          return get((uv_status_code) value);
      }

      return GRIS;
    }

  private:

    mind_graph_data _comfort[11];
    mind_graph_data _buildGraph(int field, int status, buffer_data_stat stat);
    mind_graph_data _addAnalysis(mind_graph_data data, buffer_data_stat stat);
    String _color(mind_comfort_code code) { return mind_comfort_color[int(code)]; }
    float _tolerance(int code) { return sensor_tolerance_value[int(code)]; }

    String mind_comfort_color[6] {
      "#444444",  // GRIS
      "#4eb400",  // VERT
      "#f7e400",  // JAUNE
      "#f85900",  // ORANGE
      "#d8001d",  // ROUGE
      "#6B49C8"   // VIOLET
    };

    // TODO: auto tolerance to remove sensor fields from VMind
    float sensor_tolerance_value[9] = {
      1.0,        // TEMPERATURE
      5.0,        // PRESSURE
      5.0,        // HUMIDITY
      50.0,       // AIR_QUALITY
      200.0,      // CO2_EQUIVALENT
      1.0,        // VOC_EQUIVALENT
      1.0,        // UV_INDEX
      10.0,       // VISIBLE
      100.0       // INFRARED
    };

    // conversions

    mind_comfort_code mind_temperature_comfort[5] {
      ORANGE,     // FROZEN
      JAUNE,      // COLD
      VERT,       // CONFORT
      JAUNE,      // HOT
      ORANGE      // VERY_HOT
    };

    mind_comfort_code mind_pressure_comfort[10] {
      VIOLET,     // HURRICANE_5
      VIOLET,     // HURRICANE_4
      ROUGE,      // HURRICANE_3
      ROUGE,      // HURRICANE_2
      ORANGE,     // STORMY
      JAUNE,      // RAIN
      VERT,       // CHANGING
      VERT,       // FAIR
      ORANGE,     // VERY_DRY
      ROUGE       // WARNING
    };

    mind_comfort_code mind_humidity_comfort[5] {
      ROUGE,      // NO
      VERT,       // CONFORT
      JAUNE,      // HUMID
      ORANGE,     // VERY_HUMID
      ROUGE       // TROPICAL
    };

    mind_comfort_code mind_air_comfort[8] {
      VERT,       // PURE
      VERT,       // GOOD
      JAUNE,      // AVERAGE
      ORANGE,     // NOT_GOOD
      ORANGE,     // BAD
      ROUGE,      // WORSE
      ROUGE,      // VERY_BAD
      VIOLET      // TOXIC
    };

    mind_comfort_code mind_uv_comfort[5] {
      VERT,       // LOW
      JAUNE,      // MODERATE
      ORANGE,     // HIGH
      ROUGE,      // VERY_HIGH
      VIOLET      // EXTREME
    };

};

#endif
