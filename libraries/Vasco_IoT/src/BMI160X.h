/*
 * Read BMI160 6DoF inertial sensor with I2C transport. component from the Intel's CurieIMU driver
 * Ref: https://github.com/EmotiBit/EmotiBit_BMI160/tree/master
 * Pin: https://github.com/hanyazou/BMI160-Arduino/issues/2
 * Doc: https://docs.arduino.cc/retired/archived-libraries/CurieIMU/
 */

#ifndef BMI160X_h
#define BMI160X_h

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Sensor.h"
#include "component/Pins.h"
#include "component/Timer.h"
#include "Wire.h"
#include "CurieIMU.h"


class BMI160X : public Sensor, public I2cPins, public CurieIMUClass, public Timer
{
  public:

    static const int _GYROSCOPE_RATE = 100; // reads/second. values: 25, 50, 100, 200, 400, 800, 1600, 3200 (Hz)
    static const int _GYROSCOPE_RANGE = 1000; // max - min value. values: 125, 250, 500, 1000, 2000 (degrees/second)
    static const int _ACCELEROMETER_RATE = 100; // values: 12.5, 25, 50, 100, 200, 400, 800, 1600 (Hz)
    static const int _ACCELEROMETER_RANGE = 2; // values: 2, 4, 8, 16 (G)

    BMI160X(byte addr) : Sensor(INERTIAL_UNIT, true) { _i2cAddress = addr; }

    // interfaces
    bool    init();
    bool    sleep();
    bool    wake();
    bool    check();
    bool    update();
    float   read();
    vfield  get(vsensor code)
    {
      switch (code) {
        case MOVEMENT:
          return _maxValue;
      }

      return (vfield){};
    }

    // other data updated
    vcoord  getGyroscope() { return vcoord {_gx, _gy, _gz}; } // in degree
    vcoord  getAccelerometer() { return vcoord {_ax, _ay, _az}; } // in G
    float   getTemperature() { return _temperature; }

    // curie class serial transport override
    void    ss_init(); // called in CurieIMUClass::begin() function
    int     ss_xfer(uint8_t *buf, unsigned tx_cnt, unsigned rx_cnt); // called for transport

    /*
    // TODO vasco: continue implemting CurieIMU
    bool    shockDetected(int axis, int direction); // X_AXIS Y_AXIS Z_AXIS
    bool    motionDetected(int axis, int direction); // X_AXIS Y_AXIS Z_AXIS
    bool    tapDetected(int axis, int direction); // X_AXIS Y_AXIS Z_AXIS
    bool    stepsDetected();
    */

  private:

    byte    _i2cAddress;
    float   _maxValueBuffer = 0;
    vfield  _maxValue = {"Quantité mouvement", "mG", 100};
    float   _gx = 0;
    float   _gy = 0;
    float   _gz = 0;
    float   _ax = 0;
    float   _ay = 0;
    float   _az = 0;
    float   _temperature = 0;

    float   _convertRawGyro(int gRaw); // convert to °
    float   _convertRawAcceleration(int aRaw); // convert to G
    float   _convertToCelcius(int raw);

    vlegend _maxValues[1] = {
      {0, VERT, "ok"},
    };
};

#endif
