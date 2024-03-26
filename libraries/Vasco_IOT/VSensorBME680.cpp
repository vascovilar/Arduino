#include "VSensorBME680.h"

bool VSensorBME680::init()
{
  _iaq.begin(_I2C_ADDRESS, Wire);
  
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
    Serial.println(F("Error initializing i2c BME680 device"));

    return false;
  }

  return true;
}

bool VSensorBME680::wake()
{
  return true;
}

bool VSensorBME680::sleep()
{ 
  return true;
}

bool VSensorBME680::sync()
{
  _feed(_data.temperature, _iaq.temperature, _temperatures, 6);
  _feed(_data.pressure, _convertToMilliBar(_iaq.pressure), _pressures, 10);
  _feed(_data.humidity, _iaq.humidity, _humidities, 6);
  _feed(_data.iaqAccuracy, _iaq.staticIaqAccuracy, _iaqAccuracies, 4);
  
  if (_iaq.stabStatus == 1 && _iaq.staticIaqAccuracy > 0) {
    _feed(_data.gasResistance, _convertToKiloOhm(_iaq.gasResistance), _gasResistances, 1);
    _feed(_data.airQuality, _iaq.staticIaq, _airQualities, 8);
    _feed(_data.co2Equivalent, _iaq.co2Equivalent, _co2Equivalents, 1);
    _feed(_data.vocEquivalent, _iaq.breathVocEquivalent, _vocEquivalents, 1);
    _feed(_data.gasPercentage, _iaq.gasPercentage, _gasPercentages, 5);
  }

  return true;
}

bool VSensorBME680::event()
{
  return false;
}

float VSensorBME680::check() 
{
  // must call run every 3s or less, else air mesure never works
  // BSEC_SAMPLE_RATE_LP is 3s
  // BSEC_SAMPLE_RATE_ULP is 300s
  _iaq.run(); 

  return 0.0;
}

void VSensorBME680::_checkIaqSensorStatus()
{
  if (_iaq.bsecStatus != BSEC_OK) {
    if (_iaq.bsecStatus < BSEC_OK) {
      Serial.println("BSEC error code : " + String(_iaq.bsecStatus));
    } else {
      Serial.println("BSEC warning code : " + String(_iaq.bsecStatus));
    }
  }

  if (_iaq.bme68xStatus != BME68X_OK) {
    if (_iaq.bme68xStatus < BME68X_OK) {
      Serial.println("BME68X error code : " + String(_iaq.bme68xStatus));
    } else {
      Serial.println("BME68X warning code : " + String(_iaq.bme68xStatus));
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
