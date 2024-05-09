#include "BMI160X.h"


bool BMI160X::init()
{
  if (_i2cAddress != 0x68 && _i2cAddress != 0x69) {
    Serial.println(F("Error BMI160 device use I2C address 0x68 or 0x69"));

    return false;
  }

  if (!CurieIMUClass::begin()) {
    Serial.println(F("Error initializing I2C BMI160 device"));

    return false;
  }

  // config
  setGyroRate(_GYROSCOPE_RATE);
  setGyroRange(_GYROSCOPE_RANGE);
  autoCalibrateGyroOffset();

  setAccelerometerRate(_ACCELEROMETER_RATE);
  setAccelerometerRange(_ACCELEROMETER_RANGE);
  // autoCalibrateAccelerometerOffset(X_AXIS, -1);
  // accelerometerOffsetEnabled(true);

  return true;
}

bool BMI160X::sleep()
{
  return true;
}

bool BMI160X::wake()
{
  return true;
}

bool BMI160X::check()
{
  return false;
}

bool BMI160X::update()
{
  int gxRaw, gyRaw, gzRaw, axRaw, ayRaw, azRaw; // return by reference
  readMotionSensor(gxRaw, gyRaw, gzRaw, axRaw, ayRaw, azRaw);
  _gx = _convertRawMotion(gxRaw, _GYROSCOPE_RANGE);
  _gy = _convertRawMotion(gyRaw, _GYROSCOPE_RANGE);
  _gz = _convertRawMotion(gzRaw, _GYROSCOPE_RANGE);
  _ax = _convertRawMotion(axRaw, _ACCELEROMETER_RANGE);
  _ay = _convertRawMotion(ayRaw, _ACCELEROMETER_RANGE);
  _az = _convertRawMotion(azRaw, _ACCELEROMETER_RANGE);

  _temperature = _convertToCelcius(readTemperature());

  return true;
}

void BMI160X::ss_init()
{
  Wire.beginTransmission(_i2cAddress);
  if( Wire.endTransmission() != 0 ) {
    Serial.println(F("Error initializing I2C BMI160 device"));
  }
}

int BMI160X::ss_xfer(uint8_t *buf, unsigned tx_cnt, unsigned rx_cnt)
{
  byte *p;

  Wire.beginTransmission(_i2cAddress);
  p = buf;
  while (0 < tx_cnt) {
    tx_cnt--;
    Wire.write(*p++);
  }
  if( Wire.endTransmission() != 0 ) {
      Serial.println(F("Error: Wire.endTransmission() for BMI160 failed."));
  }
  if (0 < rx_cnt) {
    Wire.requestFrom(_i2cAddress, rx_cnt);
    p = buf;
    while ( Wire.available() && 0 < rx_cnt) {
      rx_cnt--;
      *p++ = Wire.read();
    }
  }

  return 0;
}

float BMI160X::_convertRawMotion(int raw, int range)
{
  // if we are using 250 (unit) range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  return (raw / 32768.0) * range;
}

float BMI160X::_convertToCelcius(int raw)
{
  return (raw / 512.0) + 23;
}
