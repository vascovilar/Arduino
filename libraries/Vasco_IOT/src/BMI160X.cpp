#include "BMI160X.h"


bool BMI160X::init()
{
  if (_i2cAddress != 0x68 && _i2cAddress != 0x69) {
    Serial.println(F("Error BMI160 device use I2C address 0x68 or 0x69"));

    return false;
  }

  Wire.begin(); // need to initialise I2C
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
  autoCalibrateAccelerometerOffset(X_AXIS, 0);
  autoCalibrateAccelerometerOffset(Y_AXIS, 0);
  autoCalibrateAccelerometerOffset(Z_AXIS, 1);

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
  // check 10 time per second only
  if (millis() - _timer >= 100) {
    _timer = millis(); // reset timer
    float value = read();

    // store max
    if (value > _maxValueBuffer) {
      _maxValueBuffer = value;
    }
  }

  // detecting anything
  return _maxValueBuffer > 0;
}

bool BMI160X::update()
{
  // sensor class values
  feed(_maxValue, _maxValueBuffer, _maxValues, 1);

  // reset max value buffer for another round
  _maxValueBuffer = 0;

  // update local variables
  int gxRaw, gyRaw, gzRaw, axRaw, ayRaw, azRaw; // return by reference
  readMotionSensor(axRaw, ayRaw, azRaw, gxRaw, gyRaw, gzRaw);
  _gx = _convertRawGyro(gxRaw);
  _gy = _convertRawGyro(gyRaw);
  _gz = _convertRawGyro(gzRaw);
  _ax = _convertRawAcceleration(axRaw);
  _ay = _convertRawAcceleration(ayRaw);
  _az = _convertRawAcceleration(azRaw);
  _temperature = _convertToCelcius(readTemperature());

  return true;
}

float BMI160X::read()
{
  float ax, ay;
  int axRaw, ayRaw, azRaw;

  readAccelerometer(axRaw, ayRaw, azRaw);
  ax = _convertRawAcceleration(axRaw);
  ay = _convertRawAcceleration(ayRaw);

  // return max of X or Y
  return ax > ay ? ax: ay;
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
      Serial.println(F("Error: BMI160 I2C transmission failed."));
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

float BMI160X::_convertRawGyro(int gRaw)
{
  // since we are using 250 degrees range
  // -125 (-90°) maps to a raw value of -16384
  // +125 (+90°) maps to a raw value of 16384

  return (gRaw / 32768.0) * 180;
}

float BMI160X::_convertRawAcceleration(int aRaw)
{
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767

  return (aRaw / 32768.0) * _ACCELEROMETER_RANGE;
}

float BMI160X::_convertToCelcius(int traw)
{
  return (traw / 512.0) + 23;
}