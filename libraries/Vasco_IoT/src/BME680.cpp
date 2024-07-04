#include "BME680.h"

bool BME680::init()
{
  if (_i2cAddress != 0x76 && _i2cAddress != 0x77) {
    Serial.println(F("Error BME680 device use I2C address 0x76 or 0x77"));

    return false;
  }

  _iaq.begin(_i2cAddress, Wire);
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

  if (!_iaq.run()) {
    _checkIaqSensorStatus();
    Serial.println(F("Error initializing I2C BME680 device"));

    return false;
  }

  return true;
}

bool BME680::sleep()
{
  return true;
}

bool BME680::wake()
{
  return true;
}

bool BME680::check()
{
  // run every 3s or less (run function already includes delay condition), air mesure takes 5 min to be available.
  // BSEC_SAMPLE_RATE_LP is 3s. BSEC_SAMPLE_RATE_ULP is 300s
  _iaq.run();

  return false;
}

bool BME680::update()
{
  // sensor class values
  feed(_temperature, _iaq.temperature, _temperatures, 6);
  feed(_pressure, _convertToMilliBar(_iaq.pressure), _pressures, 11);
  feed(_humidity, _iaq.humidity, _humidities, 7);
  feed(_iaqAccuracy, _iaq.staticIaqAccuracy, _iaqAccuracies, 4);

  // if data available (air sensor need 5 min to start)
  if (_iaq.stabStatus == 1 && _iaq.staticIaqAccuracy > 0) {
    feed(_gasResistance, _convertToKiloOhm(_iaq.gasResistance), _gasResistances, 1);
    feed(_airQuality, _iaq.staticIaq, _airQualities, 8);
    feed(_co2Equivalent, _iaq.co2Equivalent, _co2Equivalents, 1);
    feed(_vocEquivalent, _iaq.breathVocEquivalent, _vocEquivalents, 1);
    feed(_gasPercentage, _iaq.gasPercentage, _gasPercentages, 5);
  }

  return true;
}

float BME680::read()
{
  return 0.0;
}

void BME680::_checkIaqSensorStatus()
{
  if (_iaq.bsecStatus != BSEC_OK) {
    if (_iaq.bsecStatus < BSEC_OK) {
      //Serial.println("BSEC error code : "+ String(_iaq.bsecStatus));
    } else {
      //Serial.println("BSEC warning code : "+ String(_iaq.bsecStatus));
    }
  }

  if (_iaq.bme68xStatus != BME68X_OK) {
    if (_iaq.bme68xStatus < BME68X_OK) {
      //Serial.println("BME68X error code : " + String(_iaq.bme68xStatus));
    } else {
      //Serial.println("BME68X warning code : " + String(_iaq.bme68xStatus));
    }
  }
}

float BME680::_convertToMilliBar(float pressure)
{
  return pressure / 100.0;
}

float BME680::_convertToKiloOhm(float resistance)
{
  return resistance / 1000.0;
}
