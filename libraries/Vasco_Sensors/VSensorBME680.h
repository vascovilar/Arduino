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
#include "VEnum.h"
#include "bsec.h"

class VSensorBME680
{
  public:
    
    void  begin(int addr); // i2c must be 0x76 or 0x77
    bool  update(int delay); // 3000 ms minimum
    void  sync(); // read device registery data
    
    field_data  getTemperature() { return _data.temperature; }
    field_data  getPressure() { return _data.pressure; }
    field_data  getHumidity() { return _data.humidity; }
    field_data  getGasResistance() { return _data.gasResistance; }
    field_data  getAirQuality() { return _data.airQuality; }
    field_data  getCo2Equivalent() { return _data.co2Equivalent; }
    field_data  getVocEquivalent() { return _data.vocEquivalent; }
    field_data  getGasPercentage() { return _data.gasPercentage; }
    int         getMeasureTime() { return _data.measureTime; } // in milliseconds

  private:

    Bsec _iaq;
    unsigned int _timer = 0;
    
    void _checkIaqSensorStatus();
    void _errLeds();
    float _convertToMilliBar(float pressure);
    float _convertToKiloOhm(float resistance);

    struct fields {
      field_data  temperature = {"VDeviceWebTempérature", "°C", 0.5};
      field_data  pressure = {"Pression", "mBar", 1.0}; 
      field_data  humidity = {"Humidité", "%", 1.0};
      field_data  gasResistance = {"Resistivité de l'air", "kOhm", 10000.0};
      field_data  airQuality = {"Qualité de l'air", "", 10.0};
      field_data  co2Equivalent = {"Equivalent CO2", "ppm", 50.0};
      field_data  vocEquivalent = {"Equivalent VOC", "ppm", 0.5};
      field_data  gasPercentage = {"Particules / Solvants", "%", 5.0};
      int         measureTime = 0;
    };
    fields _data;

    field_data _setTemperature(field_data field, float value)
    {
      field.value = value;

      if (value <= 0) { field.status = ORANGE; field.text = "glacé"; } 
      else if (value <= 18) { field.status = JAUNE; field.text = "froid"; } 
      else if (value <= 25) { field.status = VERT; field.text = "confortable"; } 
      else if (value <= 30) { field.status = JAUNE; field.text = "chaud"; } 
      else if (value <= 35) { field.status = ORANGE; field.text = "très chaud"; } 
      else { field.status = ROUGE; field.text = "trop chaud"; }

      return field;
    }

    field_data _setPressure(field_data field, float value)
    {
      field.value = value;

      if (value <= 920) { field.status = VIOLET; field.text = "ouragan classe 5"; } 
      else if (value <= 944) { field.status = VIOLET; field.text = "ouragan classe 4"; } 
      else if (value <= 964) { field.status = ROUGE; field.text = "ouragan classe 2"; } 
      else if (value <= 980) { field.status = ROUGE; field.text = "ouragan classe 2"; } 
      else if (value <= 985) { field.status = ORANGE; field.text = "tempête"; } 
      else if (value <= 1000) { field.status = JAUNE; field.text = "pluie ou vent"; } 
      else if (value <= 1020) { field.status = VERT; field.text = "variable"; } 
      else if (value <= 1035) { field.status = VERT; field.text = "beau temps"; } 
      else if (value <= 1050) { field.status = ORANGE; field.text = "très sec"; } 
      else { field.status = ROUGE; field.text = "maximum"; }

      return field;
    }

    field_data _setHumidity(field_data field, float value)
    {
      field.value = value;

      if (value <= 10) { field.status = VIOLET; field.text = "dangereux"; } 
      else if (value <= 20) { field.status = ROUGE; field.text = "sec"; } 
      else if (value <= 50) { field.status = VERT; field.text = "confortable"; } 
      else if (value <= 60) { field.status = JAUNE; field.text = "humide"; } 
      else if (value <= 90) { field.status = ORANGE; field.text = "très humide"; } 
      else { field.status = ROUGE; field.text = "tropical"; }

      return field;
    }

    field_data _setGasResistance(field_data field, float value)
    {
      field.value = value;

      return field;
    }

    field_data _setAirQuality(field_data field, float value)
    {
      field.value = value;

      if (value <= 20) { field.status = VERT; field.text = "excellente"; } 
      else if (value <= 50) { field.status = VERT; field.text = "bonne"; } 
      else if (value <= 100) { field.status = JAUNE; field.text = "moyenne"; } 
      else if (value <= 150) { field.status = ORANGE; field.text = "passable"; } 
      else if (value <= 200) { field.status = ORANGE; field.text = "mauvaise"; } 
      else if (value <= 300) { field.status = ROUGE; field.text = "pire"; } 
      else if (value <= 400) { field.status = ROUGE; field.text = "très mauvaise"; } 
      else { field.status = VIOLET; field.text = "toxique"; }

      return field;
    }

    field_data _setCo2Equivalent(field_data field, float value)
    {
      field.value = value;

      return field;
    }

    field_data _setVocEquivalent(field_data field, float value)
    {
      field.value = value;

      return field;
    }

    field_data _setGasPercentage(field_data field, float value)
    {
      field.value = value;

      if (value <= 5) { field.status = VERT; field.text = "air pur"; } 
      else if (value <= 20) { field.status = JAUNE; field.text = "aérer"; } 
      else if (value <= 40) { field.status = ORANGE; field.text = "ventiler"; } 
      else if (value <= 80) { field.status = ROUGE; field.text = "attention"; } 
      else { field.status = VIOLET; field.text = "toxique"; }

      return field;
    }
};

#endif
