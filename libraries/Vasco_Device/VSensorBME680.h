/*
 * Read data on BME680
 * Implementation:
 *
 *   #include <VSensorBME680.h>
 *
 *   VSensorAir meteo; 
 *
 *   void setup() {
 *     meteo.begin(0x76);
 *   }
 *   void loop() {
 *     if (meteo.update(1000)) {
 *       Serial.println(meteo.getTemperature().value);
 *     };
 *   }
 */

#ifndef VSensorBME680_h
#define VSensorBME680_h

#include "Arduino.h"
#include "VConfig.h"
#include "bsec.h"

class VSensorBME680
{
  public:
    
    void begin(int addr); // i2c must be 0x76 or 0x77
    bool update(int delay); // 3000 ms minimum
    void sync(); // read device registery data
    void sleep(bool isSleeping);

    field_data   getTemperature() { return _data.temperature; }
    field_data   getPressure() { return _data.pressure; }
    field_data   getHumidity() { return _data.humidity; }
    field_data   getGasResistance() { return _data.gasResistance; }
    field_data   getAirQuality() { return _data.airQuality; }
    field_data   getCo2Equivalent() { return _data.co2Equivalent; }
    field_data   getVocEquivalent() { return _data.vocEquivalent; }
    field_data   getGasPercentage() { return _data.gasPercentage; }
    unsigned int getProcessTime() { return _data.processTime; }
    
  private:

    Bsec _iaq;
    unsigned int _timer = 0;
    bool _enabled = false;
    
    void _checkIaqSensorStatus();
    float _convertToMilliBar(float pressure);
    float _convertToKiloOhm(float resistance);

    struct fields {
      field_data   temperature = {"Température", "°C", 0.5};
      field_data   pressure = {"Pression", "mBar", 1.0}; 
      field_data   humidity = {"Humidité", "%", 1.0};
      field_data   gasResistance = {"Resistivité de l'air", "kOhm", 10000.0};
      field_data   airQuality = {"Qualité de l'air", "", 10.0};
      field_data   co2Equivalent = {"Equivalent CO2", "ppm", 50.0};
      field_data   vocEquivalent = {"Equivalent VOC", "ppm", 0.5};
      field_data   gasPercentage = {"Particules / Solvants", "%", 5.0};
      unsigned int processTime;
    };
    fields _data;

    void _setTemperature(float value)
    {
      _data.temperature.value = value;

      if (value <= 0) { _data.temperature.status = ORANGE; _data.temperature.text = "glacé"; } 
      else if (value <= 18) { _data.temperature.status = JAUNE; _data.temperature.text = "froid"; } 
      else if (value <= 25) { _data.temperature.status = VERT; _data.temperature.text = "confortable"; } 
      else if (value <= 30) { _data.temperature.status = JAUNE; _data.temperature.text = "chaud"; } 
      else if (value <= 35) { _data.temperature.status = ORANGE; _data.temperature.text = "très chaud"; } 
      else { _data.temperature.status = ROUGE; _data.temperature.text = "trop chaud"; }
    }

    void _setPressure(float value)
    {
      _data.pressure.value = value;

      if (value <= 920) { _data.pressure.status = VIOLET; _data.pressure.text = "ouragan classe 5"; } 
      else if (value <= 944) { _data.pressure.status = VIOLET; _data.pressure.text = "ouragan classe 4"; } 
      else if (value <= 964) { _data.pressure.status = ROUGE; _data.pressure.text = "ouragan classe 2"; } 
      else if (value <= 980) { _data.pressure.status = ROUGE; _data.pressure.text = "ouragan classe 2"; } 
      else if (value <= 985) { _data.pressure.status = ORANGE; _data.pressure.text = "tempête"; } 
      else if (value <= 1000) { _data.pressure.status = JAUNE; _data.pressure.text = "pluie ou vent"; } 
      else if (value <= 1020) { _data.pressure.status = VERT; _data.pressure.text = "variable"; } 
      else if (value <= 1035) { _data.pressure.status = VERT; _data.pressure.text = "beau temps"; } 
      else if (value <= 1050) { _data.pressure.status = JAUNE; _data.pressure.text = "très sec"; } 
      else { _data.pressure.status = ORANGE; _data.pressure.text = "maximum"; }
    }

    void _setHumidity(float value)
    {
      _data.humidity.value = value;

      if (value <= 10) { _data.humidity.status = VIOLET; _data.humidity.text = "dangereux"; } 
      else if (value <= 20) { _data.humidity.status = ROUGE; _data.humidity.text = "sec"; } 
      else if (value <= 50) { _data.humidity.status = VERT; _data.humidity.text = "confortable"; } 
      else if (value <= 60) { _data.humidity.status = JAUNE; _data.humidity.text = "humide"; } 
      else if (value <= 90) { _data.humidity.status = ORANGE; _data.humidity.text = "très humide"; } 
      else { _data.humidity.status = ROUGE; _data.humidity.text = "tropical"; }
    }

    void _setGasResistance(float value)
    {
      _data.gasResistance.value = value;
      _data.gasResistance.status = VERT;
    }

    void _setAirQuality(float value)
    {
      _data.airQuality.value = value;

      if (value <= 20) { _data.airQuality.status = VERT; _data.airQuality.text = "excellente"; } 
      else if (value <= 50) { _data.airQuality.status = VERT; _data.airQuality.text = "bonne"; } 
      else if (value <= 100) { _data.airQuality.status = JAUNE; _data.airQuality.text = "moyenne"; } 
      else if (value <= 150) { _data.airQuality.status = ORANGE; _data.airQuality.text = "passable"; } 
      else if (value <= 200) { _data.airQuality.status = ORANGE; _data.airQuality.text = "mauvaise"; } 
      else if (value <= 300) { _data.airQuality.status = ROUGE; _data.airQuality.text = "pire"; } 
      else if (value <= 400) { _data.airQuality.status = ROUGE; _data.airQuality.text = "très mauvaise"; } 
      else { _data.airQuality.status = VIOLET; _data.airQuality.text = "toxique"; }
    }

    void _setCo2Equivalent(float value)
    {
      _data.co2Equivalent.value = value;
      _data.co2Equivalent.status = VERT;
    }

    void _setVocEquivalent(float value)
    {
      _data.vocEquivalent.value = value;
      _data.vocEquivalent.status = VERT;
    }

    void _setGasPercentage(float value)
    {
      _data.gasPercentage.value = value;

      if (value <= 5) { _data.gasPercentage.status = VERT; _data.gasPercentage.text = "air pur"; } 
      else if (value <= 20) { _data.gasPercentage.status = JAUNE; _data.gasPercentage.text = "aérer"; } 
      else if (value <= 40) { _data.gasPercentage.status = ORANGE; _data.gasPercentage.text = "ventiler"; } 
      else if (value <= 80) { _data.gasPercentage.status = ROUGE; _data.gasPercentage.text = "attention"; } 
      else { _data.gasPercentage.status = VIOLET; _data.gasPercentage.text = "maximum"; }
    }
};

#endif
