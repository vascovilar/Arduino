/**
 * 
 * common language
 * 
 */
 
#ifndef VLang_h
#define VLang_h

#include "Arduino.h"

#include "VSensors.h"
#include "VSensorAir.h"
#include "VSensorLight.h"


class VLang
{
  public:

    // trad by polymorphism
    String get(int field)                     { return sensor_field_name[field]; }
    String unit(int field)                    { return sensor_field_unit[field]; }
    String get(temperature_status_code code)  { return temperature_status_label[int(code)]; }
    String get(pressure_status_code code)     { return pressure_status_label[int(code)]; }
    String get(humidity_status_code code)     { return humidity_status_label[int(code)]; }
    String get(air_status_code code)          { return air_status_label[int(code)]; }
    String get(uv_status_code code)           { return uv_status_label[int(code)]; }
    
    // tool
    String find(int field, int code) 
    {
      switch (field) 
      {
        case AIR_QUALITY:
          return get((air_status_code)code);
        case PRESSURE:
          return get((pressure_status_code)code);
        case UV_INDEX:
          return get((uv_status_code)code);
        case HUMIDITY:
          return get((humidity_status_code)code);
        case TEMPERATURE:
          return get((temperature_status_code)code);
      }

      return "";
    }

  private:

    String sensor_field_name[10] = {
      "Température",
      "Pression",
      "Humidité",
      "Qualité air",
      "Equivalent CO2",
      "Equivalent VOC",
      "Index UV",
      "Lumière visible",
      "Infrarouges",
      "EMF cumul"
    }; 

    String sensor_field_unit[10] = {
      "°C",
      "hPa",
      "%",
      "index",
      "ppm",
      "ppm",
      "index",
      "lux",
      "lux",
      "index"
    };

    String air_status_label[8] = {
      "excellente",
      "bonne",
      "moyenne",
      "passable",
      "mauvaise",
      "pire",
      "tres mauvaise",
      "toxique"
    };

    String pressure_status_label[10] = {
      "ouragan classe 5",
      "ouragan classe 4",
      "ouragan classe 3",
      "ouragan classe 2",
      "tempête",
      "pluie ou vent",
      "variable",
      "beau temps",
      "très sec",
      "extrême"
    };

    String uv_status_label[5] = {
      "risque faible",
      "risque modéré",
      "risque élevé",
      "risque très élevé",
      "risque extrême"
    };

    String humidity_status_label[5] = {
      "dangereux",
      "confortable",
      "humide",
      "très humide",
      "tropical"
    };  

    String temperature_status_label[5] = {
      "glacé",
      "froid",
      "confortable",
      "chaud",
      "très chaud"
    };  

};

#endif
