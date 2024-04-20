/*
 * Read BMI160 6DoF inertial sensor with I2C transport. Inherit from the Intel's CurieIMU driver
 * Ref: https://github.com/EmotiBit/EmotiBit_BMI160/tree/master
 * Pin: https://github.com/hanyazou/BMI160-Arduino/issues/2
 * Doc: https://docs.arduino.cc/retired/archived-libraries/CurieIMU/
 *
 * Implementation:
 *
 *   #include <BMI160X.h>
 *
 *   BMI160X gyro(0x68);
 *   int gxRaw, gyRaw, gzRaw;
 *
 *   void setup() {
 *     gyro.init();
 *   }
 *
 *   void loop() {
 *     gyro.readGyro(gxRaw, gyRaw, gzRaw);
 *     Serial.println(gxRaw);
 *   }
 */

#ifndef BMI160X_h
#define BMI160X_h

#include "Arduino.h"
#include "interface/Device.h"
#include "plugin/Pins.h"
#include "interface/Data.h"
#include "CurieIMU.h"
#include "Wire.h"

class BMI160X : public Device, public CurieIMUClass, public I2cPins
{
  public:

    static const int _GYROSCOPE_RATE = 100; // values: 25, 50, 100, 200, 400, 800, 1600, 3200 (Hz)
    static const int _ACCELEROMETER_RATE = 100; // values: 12.5, 25, 50, 100, 200, 400, 800, 1600 (Hz)
    static const int _GYROSCOPE_RANGE = 250; // values: 125, 250, 500, 1000, 2000 (degrees/second)
    static const int _ACCELEROMETER_RANGE = 2; // values: 2, 4, 8, 16 (G)

    BMI160X(byte addr) : Device(INERTIAL_UNIT) { _i2cAddress = addr; }
    // interfaces
    bool    init();
    bool    sleep();
    bool    wake();
    bool    check();
    bool    update();
    // curie class serial transport override
    void    ss_init(); // called in CurieIMUClass::begin() function
    int     ss_xfer(uint8_t *buf, unsigned tx_cnt, unsigned rx_cnt); // called for transport
    // data updated
    vcoord  getGyroscope() { return vcoord {_gx, _gy, _gz}; } // in milli-degree / second
    vcoord  getAccelerometer() { return vcoord {_ax, _ay, _az}; } // in milli G
    float   getTemperature() { return _temperature; }

    /*
    // TODO vasco: continue implemting CurieIMU
    bool    shockDetected(int axis, int direction); // X_AXIS Y_AXIS Z_AXIS
    bool    motionDetected(int axis, int direction); // X_AXIS Y_AXIS Z_AXIS
    bool    tapDetected(int axis, int direction); // X_AXIS Y_AXIS Z_AXIS
    bool    stepsDetected();
    */

  private:

    byte    _i2cAddress;
    float   _gx;
    float   _gy;
    float   _gz;
    float   _ax;
    float   _ay;
    float   _az;
    float   _temperature;
    float   _convertRawMotion(int raw, int range);
    float   _convertToCelcius(int raw);

};

#endif
