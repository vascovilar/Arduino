/*
 * Read data on BME680
 * Implementation:
 *
 *   #include <VSensorMeteo.h>
 *
 *   VSensorMeteo meteo; 
 *
 *   void setup() {
 *     meteo.begin(0x76);
 *   }
 *   void loop() {
 *     if (meteo.update(1000)) {
 *       float temp = meteo.getTemperature();
 *     };
 *   }
 */

#ifndef VSensorMeteo_h
#define VSensorMeteo_h

#include "Arduino.h"
#include "Wire.h"
#include "bsec.h"

struct meteo_field_data {
  float    temperature;    // in °C
  float    pressure;       // in hPa 
  float    humidity;       // in %
  float    gasResistance;  // in kOhm
  float    airQuality;     // index
  float    co2Equivalent;  // in ppm
  float    breathVocEquivalent;// in ppm
};

enum temperature_status_code {
  V_FROZEN = 0,
  V_COLD = 1,
  V_CONFORT = 2,
  V_HOT = 3,
  V_VERY_HOT = 4
};

enum pressure_status_code {
  V_HURRICANE_5 = 0,
  V_HURRICANE_4 = 1,
  V_HURRICANE_3 = 2,
  V_HURRICANE_2 = 3,
  V_STORMY = 4,
  V_RAIN = 5,
  V_CHANGING = 6,
  V_FAIR = 7,
  V_VERY_DRY = 8,
  V_EXTREME_DRY = 9
};

enum humidity_status_code {
  V_NO_HUMID = 0,
  V_CONFORT_HUMID = 1,
  V_HUMID = 2,
  V_VERY_HUMID = 3,
  V_TROPICAL_HUMID = 4
};

enum air_status_code {
  V_PURE_AIR = 0,
  V_GOOD_AIR = 1,
  V_AVERAGE_AIR = 2,
  V_NOT_GOOD_AIR = 3,
  V_BAD_AIR = 4,
  V_WORSE_AIR = 5,
  V_VERY_BAD_AIR = 6,
  V_TOXIC_AIR = 7
};

class VSensorMeteo
{
  public:
    
    int   status();
    void  begin(int addr);
    bool  update(int delay); // 3000 ms minimum
    meteo_field_data dump() { return _data; }

    float getTemperature() { return _data.temperature; }
    float getPressure() { return _data.pressure; }
    float getHumidity() { return _data.humidity; }
    float getGasResistance() { return _data.gasResistance; }
    float getAirQuality() { return _data.airQuality; }
    float getCo2Equivalent() { return _data.co2Equivalent; }
    float getBreathVocEquivalent() { return _data.breathVocEquivalent; }
    
    temperature_status_code getTemperatureStatus();
    pressure_status_code getPressureStatus();
    humidity_status_code getHumidityStatus();
    air_status_code getAirStatus();
  
  private:
    
    Bsec _iaq;
    meteo_field_data _data;
    uint32_t _timer = millis();
    int _status = 0;
    
    void _checkDeviceStatus();
    float _convertToHPa(float pressure);
    float _convertToKOhm(float resistance);
};

#endif
