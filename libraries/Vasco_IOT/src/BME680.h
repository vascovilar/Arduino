/*
 * Read data on BME680 meteo device from Adafruit
 * Ref: https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas
 * Doc: https://cdn-shop.adafruit.com/product-files/3660/BME680.pdf
 */

#ifndef BME680_h
#define BME680_h

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Sensor.h"
#include "inherit/Pins.h"
#include "bsec.h"
#include "Wire.h"


class BME680 : public Sensor, public I2cPins
{
  public:

    BME680(byte addr) : Sensor(METEO_SENSOR, false) { _i2cAddress = addr; }

    // interfaces
    bool    init();
    bool    sleep();
    bool    wake();
    bool    check();
    bool    update();
    float   read();
    vfield  get(vsensor code)
    {
      switch(code) {
        case TEMPERATURE:
          return _temperature;
        case PRESSURE:
          return _pressure;
        case HUMIDITY:
          return _humidity;
        case AIR_QUALITY:
          return _airQuality;
        case GAS_PERCENTAGE:
          return _gasPercentage;
      }

      return (vfield){};
    }

    // other data updated
    vfield  getGasResistance() { return _gasResistance; }
    vfield  getCo2Equivalent() { return _co2Equivalent; }
    vfield  getVocEquivalent() { return _vocEquivalent; }
    vfield  getIaqAccuracy() { return _iaqAccuracy; }

  private:

    Bsec    _iaq = Bsec();
    byte    _i2cAddress;
    vfield  _temperature = {"Température", "°C", 1};
    vfield  _pressure = {"Pression", "mBar", 1};
    vfield  _humidity = {"Humidité", "%", 5};
    vfield  _gasResistance = {"Resistivité air", "kOhm", 10000};
    vfield  _airQuality = {"Qualité air", "", 10};
    vfield  _co2Equivalent = {"Equivalent CO2", "ppm", 50};
    vfield  _vocEquivalent = {"Equivalent VOC", "ppm", 1};
    vfield  _gasPercentage = {"Particules air", "%", 5};
    vfield  _iaqAccuracy = {"Précision air", "", 1};

    void    _checkIaqSensorStatus();
    float   _convertToMilliBar(float pressure);
    float   _convertToKiloOhm(float resistance);

    vlegend _temperatures[6] = {
      {0, ORANGE, "glacé"},
      {18, JAUNE, "froid"},
      {30, VERT, "confortable"},
      {35, JAUNE, "chaud"},
      {45, ORANGE, "très chaud"},
      {85, ROUGE, "danger"},
    };

    vlegend _pressures[10] = {
      {920, ROUGE, "ouragan classe 5"},
      {944, ROUGE, "ouragan classe 4"},
      {964, ROUGE, "ouragan classe 3"},
      {980, ORANGE, "ouragan classe 2"},
      {985, ORANGE, "tempête"},
      {1000, JAUNE, "pluie ou vent"},
      {1020, VERT, "variable"},
      {1035, VERT, "beau temps"},
      {1050, JAUNE, "très sec"},
      {10000, ORANGE, "hyperbare"},
    };

    vlegend _humidities[6] = {
      {10, ROUGE, "dangereux"},
      {20, ORANGE, "sec"},
      {50, VERT, "confortable"},
      {60, JAUNE, "humide"},
      {90, ORANGE, "très humide"},
      {100, ORANGE, "tropical"},
    };

    vlegend _gasResistances[1] = {
      {0, VERT, "ok"},
    };

    vlegend _airQualities[8] = {
      {20, VERT, "excellente"},
      {60, VERT, "bonne"},
      {100, VERT, "moyenne"},
      {150, JAUNE, "passable"},
      {200, ORANGE, "mauvaise"},
      {300, ORANGE, "très mauvaise"},
      {400, ROUGE, "danger"},
      {1000, ROUGE, "toxique"},
    };

    vlegend _co2Equivalents[1] = {
      {0, VERT, "ok"},
    };

    vlegend _vocEquivalents[1] = {
      {0, VERT, "ok"},
    };

    vlegend _gasPercentages[6] = {
      {5, VERT, "air pur"},
      {15, VERT, "air bon"},
      {30, JAUNE, "aérer"},
      {50, JAUNE, "ventiler"},
      {80, ORANGE, "attention"},
      {100, ROUGE, "maximum"},
    };

    vlegend _iaqAccuracies[4] = {
      {0, ROUGE, "non fiable"},
      {1, ORANGE, "faible"},
      {2, JAUNE, "moyenne"},
      {3, VERT, "haute"},
    };
};

#endif
