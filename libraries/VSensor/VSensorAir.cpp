#include "VSensorAir.h"

int VSensorAir::status()
{
  return _status;
}

void VSensorAir::begin(int addr)
{
  if (addr == 0x76 | addr == 0x77) {
    _iaq.begin(addr, Wire);
    
    bsec_virtual_sensor_t sensorList[10] = {
      BSEC_OUTPUT_RAW_TEMPERATURE,
      BSEC_OUTPUT_RAW_PRESSURE,
      BSEC_OUTPUT_RAW_HUMIDITY,
      BSEC_OUTPUT_RAW_GAS,
      BSEC_OUTPUT_IAQ,
      BSEC_OUTPUT_STATIC_IAQ,
      BSEC_OUTPUT_CO2_EQUIVALENT,
      BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
    };
    _iaq.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);

    _status = 1;
  }
}

bool VSensorAir::update(int delay)
{
  // always running
  if (! _iaq.run()) {
    // If no data is available
      _checkDeviceStatus(); 
  }

  if (millis() - _timer > delay) {
    _timer = millis(); // reset the timer

    _data.temperature = _iaq.temperature;
    _data.pressure = _convertToHPa(_iaq.pressure);
    _data.humidity = _iaq.humidity;
    _data.gasResistance = _convertToKOhm(_iaq.gasResistance);
    _data.airQuality = _iaq.staticIaq;
    _data.co2Equivalent = _iaq.co2Equivalent;
    _data.breathVocEquivalent = _iaq.breathVocEquivalent;
    _status = 2;

    return true;
  }

  return false;
}

temperature_status_code VSensorAir::getTemperatureStatus()
{
  temperature_status_code code;

  if (_data.temperature <= 0) code = V_FROZEN;
  else if (_data.temperature <= 19) code = V_COLD;
  else if (_data.temperature <= 25) code = V_CONFORT;
  else if (_data.temperature <= 35) code = V_HOT;
  else code = V_VERY_HOT;

  return code;
}

pressure_status_code VSensorAir::getPressureStatus()
{
  pressure_status_code code;

  if (_data.pressure <= 920) code = V_HURRICANE_5;
  else if (_data.pressure <= 944) code = V_HURRICANE_4;
  else if (_data.pressure <= 964) code = V_HURRICANE_3;
  else if (_data.pressure <= 980) code = V_HURRICANE_2;
  else if (_data.pressure <= 985) code = V_STORMY;
  else if (_data.pressure <= 1000) code = V_RAIN;
  else if (_data.pressure <= 1020) code = V_CHANGING;
  else if (_data.pressure <= 1035) code = V_FAIR;
  else if (_data.pressure <= 1050) code = V_VERY_DRY;
  else code = V_EXTREME_DRY;

  return code;
}

humidity_status_code VSensorAir::getHumidityStatus()
{
  humidity_status_code code;

  if (_data.humidity <= 20) code = V_NO_HUMID;
  else if (_data.humidity <= 50) code = V_CONFORT_HUMID;
  else if (_data.humidity <= 60) code = V_HUMID;
  else if (_data.humidity <= 90) code = V_VERY_HUMID;
  else code = V_TROPICAL_HUMID;

  return code;
}

air_status_code VSensorAir::getAirStatus()
{
  air_status_code code;

  if (_data.airQuality <= 20) code = V_PURE_AIR;
  else if (_data.airQuality <= 50) code = V_GOOD_AIR;
  else if (_data.airQuality <= 100) code = V_AVERAGE_AIR;
  else if (_data.airQuality <= 150) code = V_NOT_GOOD_AIR;
  else if (_data.airQuality <= 200) code = V_BAD_AIR;
  else if (_data.airQuality <= 300) code = V_WORSE_AIR;
  else if (_data.airQuality <= 400) code = V_VERY_BAD_AIR;
  else code = V_TOXIC_AIR;

  return code;
}

void VSensorAir::_checkDeviceStatus()
{
  if (_iaq.status != BSEC_OK) {
    if (_iaq.status < BSEC_OK) {
      Serial.println("BSEC error code : " + String(_iaq.status));
    } else {
      Serial.println("BSEC warning code : " + String(_iaq.status));
    }
    _status = 0;
  }

  if (_iaq.bme680Status != BME680_OK) {
    if (_iaq.bme680Status < BME680_OK) {
      Serial.println("BME680 error code : " + String(_iaq.bme680Status));
    } else {
      Serial.println("BME680 warning code : " + String(_iaq.bme680Status));
    }
    _status = 0;
  }
}

float VSensorAir::_convertToHPa(float pressure)
{
  return pressure / 100.0;
}

float VSensorAir::_convertToKOhm(float resistance)
{
  return resistance / 1000.0;
}
