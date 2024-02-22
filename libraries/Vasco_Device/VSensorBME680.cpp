#include "VSensorBME680.h"

void VSensorBME680::begin(int addr)
{
  if (addr == 0x76 | addr == 0x77) {
    _iaq.begin(addr, Wire);
    
    bsec_virtual_sensor_t sensorList[13] = {
      BSEC_OUTPUT_IAQ,
      BSEC_OUTPUT_STATIC_IAQ,
      BSEC_OUTPUT_CO2_EQUIVALENT,
      BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
      BSEC_OUTPUT_RAW_TEMPERATURE,
      BSEC_OUTPUT_RAW_PRESSURE,
      BSEC_OUTPUT_RAW_HUMIDITY,
      BSEC_OUTPUT_RAW_GAS,
      BSEC_OUTPUT_STABILIZATION_STATUS,
      BSEC_OUTPUT_RUN_IN_STATUS,
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
      BSEC_OUTPUT_GAS_PERCENTAGE
    };
    _iaq.updateSubscription(sensorList, 13, BSEC_SAMPLE_RATE_LP);
  }
}

bool VSensorBME680::update(int delay)
{
  if (millis() - _timer > delay) {
    _timer = millis(); // reset the timer
    
    sync();

    return true;
  }

  return false;
}

void VSensorBME680::sync()
{ 
  unsigned int time = millis();

  if (! _iaq.run()) {
    _checkIaqSensorStatus();
  }

  _setTemperature(_iaq.temperature);
  _setPressure(_convertToMilliBar(_iaq.pressure));
  _setHumidity(_iaq.humidity);
  _setGasResistance(_convertToKiloOhm(_iaq.gasResistance));
  _setAirQuality(_iaq.staticIaq);
  _setCo2Equivalent(_iaq.co2Equivalent);
  _setVocEquivalent(_iaq.breathVocEquivalent);
  _setGasPercentage(_iaq.gasPercentage);

  _processTime = millis() - time;
}

void VSensorBME680::_checkIaqSensorStatus()
{
  String output;

  if (_iaq.bsecStatus != BSEC_OK) {
    if (_iaq.bsecStatus < BSEC_OK) {
      output = "BSEC error code : " + String(_iaq.bsecStatus);
      Serial.println(output);
      for (;;); /* Halt in case of failure */
    } else {
      output = "BSEC warning code : " + String(_iaq.bsecStatus);
      Serial.println(output);
    }
  }

  if (_iaq.bme68xStatus != BME68X_OK) {
    if (_iaq.bme68xStatus < BME68X_OK) {
      output = "BME68X error code : " + String(_iaq.bme68xStatus);
      Serial.println(output);
      for (;;); /* Halt in case of failure */
    } else {
      output = "BME68X warning code : " + String(_iaq.bme68xStatus);
      Serial.println(output);
    }
  }
}

float VSensorBME680::_convertToMilliBar(float pressure)
{
  return pressure / 100.0;
}

float VSensorBME680::_convertToKiloOhm(float resistance)
{
  return resistance / 1000.0;
}
