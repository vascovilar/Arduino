/*
 * Read data on BME680 meteo device from Adafruit
 * Ref: https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas
 * Doc: https://cdn-shop.adafruit.com/product-files/3660/BME680.pdf
 * 
 * Implementation:
 *
 *   #include <BME680.h>
 *
 *   Air meteo(0); 
 *
 *   void setup() {
 *     meteo.init();
 *   }
 *   void loop() {
 *     Serial.println(meteo.getTemperature().value);
 *   }
 */

#ifndef BME680_h
#define BME680_h

#include "Arduino.h"
#include "interface/Sensor.h"
#include "interface/Device.h"
#include "plugin/Pins.h"
#include "bsec.h"
#include "Wire.h"


class BME680 : public Device, public Sensor, public I2cPins
{
  public:
    BME680(byte addr) : Device(METEO_SENSOR), Sensor(false) { _i2cAddress = addr; }
    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update();
    float   read();   
    // data updated
    vfield  getTemperature() { return _data.temperature; }
    vfield  getPressure() { return _data.pressure; }
    vfield  getHumidity() { return _data.humidity; }
    vfield  getGasResistance() { return _data.gasResistance; }
    vfield  getAirQuality() { return _data.airQuality; }
    vfield  getCo2Equivalent() { return _data.co2Equivalent; }
    vfield  getVocEquivalent() { return _data.vocEquivalent; }
    vfield  getGasPercentage() { return _data.gasPercentage; }
    vfield  getIaqAccuracy() { return _data.iaqAccuracy; }
    
  private:
    Bsec    _iaq;
    byte    _i2cAddress;
    void    _checkIaqSensorStatus();
    float   _convertToMilliBar(float pressure);
    float   _convertToKiloOhm(float resistance);
    
    // human readable
    struct fields {
      vfield   temperature = {"Température", "°C", 0.5};
      vfield   pressure = {"Pression", "mBar", 1.0}; 
      vfield   humidity = {"Humidité", "%", 1.0};
      vfield   gasResistance = {"Resistivité air", "kOhm", 10000.0};
      vfield   airQuality = {"Qualité air", "", 10.0};
      vfield   co2Equivalent = {"Equivalent CO2", "ppm", 50.0};
      vfield   vocEquivalent = {"Equivalent VOC", "ppm", 0.5};
      vfield   gasPercentage = {"Particules air", "%", 5.0};
      vfield   iaqAccuracy = {"Précision air", "", 1.0};
    };
    fields _data;

    vlegend _temperatures[6] = {
      {0, ORANGE, "glacé"},
      {18, JAUNE, "froid"},
      {25, VERT, "confortable"},
      {30, JAUNE, "chaud"},
      {35, ORANGE, "très chaud"},
      {85, ROUGE, "trop chaud"},
    };
    
    vlegend _pressures[10] = {
      {920, VIOLET, "ouragan classe 5"},
      {944, VIOLET, "ouragan classe 4"},
      {964, ROUGE, "ouragan classe 3"},
      {980, ROUGE, "ouragan classe 2"},
      {985, ORANGE, "tempête"},
      {1000, JAUNE, "pluie ou vent"},
      {1020, VERT, "variable"},
      {1035, VERT, "beau temps"},
      {1050, JAUNE, "très sec"},
      {10000, ORANGE, "hyperbare"},
    };
    
    vlegend _humidities[6] = {
      {10, VIOLET, "dangereux"},
      {20, ROUGE, "sec"},
      {50, VERT, "confortable"},
      {60, JAUNE, "humide"},
      {90, ORANGE, "très humide"},
      {100, ROUGE, "tropical"},
    };
    
    vlegend _gasResistances[1] = {
      {0, VERT, "ok"},
    };

    vlegend _airQualities[8] = {
      {20, VERT, "excellente"},
      {60, VERT, "bonne"},
      {100, JAUNE, "moyenne"},
      {150, ORANGE, "passable"},
      {200, ORANGE, "mauvaise"},
      {300, ROUGE, "très mauvaise"},
      {400, ROUGE, "pire"},
      {1000, VIOLET, "toxique"},
    };

    vlegend _co2Equivalents[1] = {
      {0, VERT, "ok"},
    };

    vlegend _vocEquivalents[1] = {
      {0, VERT, "ok"},
    };

    vlegend _gasPercentages[5] = {
      {5, VERT, "air pur"},
      {20, JAUNE, "aérer"},
      {40, ORANGE, "ventiler"},
      {80, ROUGE, "attention"},
      {100, VIOLET, "maximum"},
    };

    vlegend _iaqAccuracies[4] = {
      {0, VERT, "non fiable"},
      {1, JAUNE, "faible"},
      {2, ORANGE, "moyenne"},
      {3, ROUGE, "haute"},
    };
};

#endif
