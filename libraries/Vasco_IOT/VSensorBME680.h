/*
 * Read data on BME680
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
#include "VDevice.h"
#include "VSensor.h"
#include "bsec.h"

class VSensorBME680 : public VDevice, public VSensor, public VI2CPins
{
  static const byte _I2C_ADDRESS = 0x77;

  public:

    VSensorBME680(byte pin) : VDevice(ADA_BME680), VSensor(pin) {};

    bool init();
    bool wake();
    bool sleep();
    bool sync();
    bool event();
    float check();
    
    field_data   getTemperature() { return _data.temperature; }
    field_data   getPressure() { return _data.pressure; }
    field_data   getHumidity() { return _data.humidity; }
    field_data   getGasResistance() { return _data.gasResistance; }
    field_data   getAirQuality() { return _data.airQuality; }
    field_data   getCo2Equivalent() { return _data.co2Equivalent; }
    field_data   getVocEquivalent() { return _data.vocEquivalent; }
    field_data   getGasPercentage() { return _data.gasPercentage; }
    field_data   getIaqAccuracy() { return _data.iaqAccuracy; }
    
  private:

    Bsec _iaq;
    
    struct fields {
      field_data   temperature = {"Température", "°C", 0.5};
      field_data   pressure = {"Pression", "mBar", 1.0}; 
      field_data   humidity = {"Humidité", "%", 1.0};
      field_data   gasResistance = {"Resistivité air", "kOhm", 10000.0};
      field_data   airQuality = {"Qualité air", "", 10.0};
      field_data   co2Equivalent = {"Equivalent CO2", "ppm", 50.0};
      field_data   vocEquivalent = {"Equivalent VOC", "ppm", 0.5};
      field_data   gasPercentage = {"Particules air", "%", 5.0};
      field_data   iaqAccuracy = {"Précision air", "", 1.0};
    };
    fields _data;

    legend_data _temperatures[6] = {
      {0, ORANGE, "glacé"},
      {18, JAUNE, "froid"},
      {25, VERT, "confortable"},
      {30, JAUNE, "chaud"},
      {35, ORANGE, "très chaud"},
      {85, ROUGE, "trop chaud"},
    };
    
    legend_data _pressures[10] = {
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
    
    legend_data _humidities[6] = {
      {10, VIOLET, "dangereux"},
      {20, ROUGE, "sec"},
      {50, VERT, "confortable"},
      {60, JAUNE, "humide"},
      {90, ORANGE, "très humide"},
      {100, ROUGE, "tropical"},
    };
    
    legend_data _gasResistances[1] = {
      {0, VERT, "ok"},
    };

    legend_data _airQualities[8] = {
      {20, VERT, "excellente"},
      {60, VERT, "bonne"},
      {100, JAUNE, "moyenne"},
      {150, ORANGE, "passable"},
      {200, ORANGE, "mauvaise"},
      {300, ROUGE, "très mauvaise"},
      {400, ROUGE, "pire"},
      {1000, VIOLET, "toxique"},
    };

    legend_data _co2Equivalents[1] = {
      {0, VERT, "ok"},
    };

    legend_data _vocEquivalents[1] = {
      {0, VERT, "ok"},
    };

    legend_data _gasPercentages[5] = {
      {5, VERT, "air pur"},
      {20, JAUNE, "aérer"},
      {40, ORANGE, "ventiler"},
      {80, ROUGE, "attention"},
      {100, VIOLET, "maximum"},
    };

    legend_data _iaqAccuracies[4] = {
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
