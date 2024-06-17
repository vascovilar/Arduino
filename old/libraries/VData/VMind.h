#ifndef VMind_h
#define VMind_h

#include "Arduino.h"
#include "VBuffer.h"
#include "VSensors.h"
#include "VSensorAir.h"
#include "VSensorLight.h"

#define MIND_SENSITIVITY 12

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
  float    top;
  float    bottom;
  int      status;
  String   color;
  int      alert;
  String   comment;
};

class VMind
{
  public:

    void analyse(int field, int status, float tolerance, float* values)
    {
      mind_graph_data info;
      
      buffer_data_stat stat = VBuffer::stat(values, 0, BUFFER_ARRAY_LENGTH, tolerance);
      info.value = stat.value;
      info.maximum = stat.maximum;
      info.minimum = stat.minimum;
      info.average = stat.average;      
      info.delta = stat.delta;
      info.top = stat.top;
      info.bottom = stat.bottom;
      info.status = status;    
      info.color = _color(find(field, status));

      buffer_data_stat slice = VBuffer::stat(values, 0, int(BUFFER_ARRAY_LENGTH / MIND_SENSITIVITY));
      buffer_data_stat other = VBuffer::stat(values, int(BUFFER_ARRAY_LENGTH / MIND_SENSITIVITY), BUFFER_ARRAY_LENGTH);
      info.comment  = String(stat.trend * 100) + "% /" + stat.tolerance + " ~" + stat.delta + " "; 
      if (slice.average < other.average) { info.comment += "down "; };
      if (slice.average > other.average) { info.comment += "up "; };
      if (slice.delta < other.delta) { info.comment += "steady "; };
      if (slice.delta > other.delta) { info.comment += "excited "; };

      info.alert = 0;
      if (abs(stat.trend) > 0.5) { 
        info.alert = 1;
      }
      if (abs(stat.trend) > 1) {
        info.alert = 2;
      }

      _comfort[field] = info;
    }
  
    mind_graph_data info(int field)
    {
      return _comfort[field];
    }
    
    // conversions by polymorphism
    mind_comfort_code get(temperature_status_code code) { return mind_temperature_comfort[int(code)]; }
    mind_comfort_code get(pressure_status_code code)    { return mind_pressure_comfort[int(code)]; }
    mind_comfort_code get(humidity_status_code code)    { return mind_humidity_comfort[int(code)]; }
    mind_comfort_code get(air_status_code code)         { return mind_air_comfort[int(code)]; }
    mind_comfort_code get(uv_status_code code)          { return mind_uv_comfort[int(code)]; }
    
    // tool
    mind_comfort_code find(int field, int value) 
    {
      switch (field) 
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
    String _color(mind_comfort_code code) { return mind_comfort_color[int(code)]; }

    String mind_comfort_color[6] {
      "#444444",  // GRIS
      "#4eb400",  // VERT
      "#f7e400",  // JAUNE
      "#f85900",  // ORANGE
      "#d8001d",  // ROUGE
      "#6B49C8"   // VIOLET
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
