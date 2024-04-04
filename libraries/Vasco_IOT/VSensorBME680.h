/*
 * Read data on BME680 meteo device from Adafruit
 * Ref: https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas
 * Doc: https://cdn-shop.adafruit.com/product-files/3660/BME680.pdf
 * 
 * Implementation:
 *
 *   #include <VSensorBME680.h>
 *
 *   VSensorAir meteo(0); 
 *
 *   void setup() {
 *     meteo.init();
 *   }
 *   void loop() {
 *     Serial.println(meteo.getTemperature().value);
 *   }
 */

#ifndef VSensorBME680_h
#define VSensorBME680_h

#include "Arduino.h"
#include "VUsePins.h"
#include "VDevice.h"
#include "VSensor.h"
#include "bsec.h"
#include "Wire.h"

class VSensorBME680 : public VDevice, public VSensor, public VUseI2cPins
{
  public:

    VSensorBME680(byte addr) : VDevice(METEO_SENSOR), VSensor(false) {
      _i2cAddress = addr;
    };

    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update();
    float   read();
    
    // data updated
    vfield_data   getTemperature() { return _data.temperature; }
    vfield_data   getPressure() { return _data.pressure; }
    vfield_data   getHumidity() { return _data.humidity; }
    vfield_data   getGasResistance() { return _data.gasResistance; }
    vfield_data   getAirQuality() { return _data.airQuality; }
    vfield_data   getCo2Equivalent() { return _data.co2Equivalent; }
    vfield_data   getVocEquivalent() { return _data.vocEquivalent; }
    vfield_data   getGasPercentage() { return _data.gasPercentage; }
    vfield_data   getIaqAccuracy() { return _data.iaqAccuracy; }
    
  private:

    Bsec         _iaq;
    byte         _i2cAddress;
    
    struct fields {
      vfield_data   temperature = {"Température", "°C", 0.5};
      vfield_data   pressure = {"Pression", "mBar", 1.0}; 
      vfield_data   humidity = {"Humidité", "%", 1.0};
      vfield_data   gasResistance = {"Resistivité air", "kOhm", 10000.0};
      vfield_data   airQuality = {"Qualité air", "", 10.0};
      vfield_data   co2Equivalent = {"Equivalent CO2", "ppm", 50.0};
      vfield_data   vocEquivalent = {"Equivalent VOC", "ppm", 0.5};
      vfield_data   gasPercentage = {"Particules air", "%", 5.0};
      vfield_data   iaqAccuracy = {"Précision air", "", 1.0};
    };
    fields _data;

    vlegend_data _temperatures[6] = {
      {0, ORANGE, "glacé"},
      {18, JAUNE, "froid"},
      {25, VERT, "confortable"},
      {30, JAUNE, "chaud"},
      {35, ORANGE, "très chaud"},
      {85, ROUGE, "trop chaud"},
    };
    
    vlegend_data _pressures[10] = {
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
    
    vlegend_data _humidities[6] = {
      {10, VIOLET, "dangereux"},
      {20, ROUGE, "sec"},
      {50, VERT, "confortable"},
      {60, JAUNE, "humide"},
      {90, ORANGE, "très humide"},
      {100, ROUGE, "tropical"},
    };
    
    vlegend_data _gasResistances[1] = {
      {0, VERT, "ok"},
    };

    vlegend_data _airQualities[8] = {
      {20, VERT, "excellente"},
      {60, VERT, "bonne"},
      {100, JAUNE, "moyenne"},
      {150, ORANGE, "passable"},
      {200, ORANGE, "mauvaise"},
      {300, ROUGE, "très mauvaise"},
      {400, ROUGE, "pire"},
      {1000, VIOLET, "toxique"},
    };

    vlegend_data _co2Equivalents[1] = {
      {0, VERT, "ok"},
    };

    vlegend_data _vocEquivalents[1] = {
      {0, VERT, "ok"},
    };

    vlegend_data _gasPercentages[5] = {
      {5, VERT, "air pur"},
      {20, JAUNE, "aérer"},
      {40, ORANGE, "ventiler"},
      {80, ROUGE, "attention"},
      {100, VIOLET, "maximum"},
    };

    vlegend_data _iaqAccuracies[4] = {
      {0, VERT, "non fiable"},
      {1, JAUNE, "faible"},
      {2, ORANGE, "moyenne"},
      {3, ROUGE, "haute"},
    };



    void _checkIaqSensorStatus();
    float _convertToMilliBar(float pressure);
    float _convertToKiloOhm(float resistance);
};

#endif
