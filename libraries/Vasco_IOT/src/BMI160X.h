/*
 * Read BMI160 6DoF inertial sensor with I2C transport. Inherit from the Intel's CurieIMU driver
 * Ref: https://github.com/EmotiBit/EmotiBit_BMI160/tree/master
 * Pin: https://github.com/hanyazou/BMI160-Arduino/issues/2
 * Doc: https://docs.arduino.cc/retired/archived-libraries/CurieIMU/
 *
 * Implementation:
 *
 *   #include <BMI160X.h>
 *   #include <interface/data.h>
 *
 *   BMI160X imu(0x68);
 *   int gxRaw, gyRaw, gzRaw;
 *
 *   void setup() {
 *     imu.init();
 *   }
 *
 *   void loop() {
 *     if (imu.update()) {
 *       vcoord coord = imu.getGyroscope();
 *       Serial.print(String(coord.x) + ", " + String(coord.y) + ", " + String(coord.z) + " °   ");
 *     }
 *   }
 */

#ifndef BMI160X_h
#define BMI160X_h

#include "Arduino.h"
#include "interface/Device.h"
#include "interface/Sensor.h"
#include "plugin/Pins.h"
#include "interface/Data.h"
#include "CurieIMU.h"
#include "Wire.h"

class BMI160X : public Device,  public Sensor, public I2cPins, public CurieIMUClass
{
  public:

    static const int _GYROSCOPE_RATE = 100; // reads/second. values: 25, 50, 100, 200, 400, 800, 1600, 3200 (Hz)
    static const int _GYROSCOPE_RANGE = 1000; // max - min value. values: 125, 250, 500, 1000, 2000 (degrees/second)
    static const int _ACCELEROMETER_RATE = 100; // values: 12.5, 25, 50, 100, 200, 400, 800, 1600 (Hz)
    static const int _ACCELEROMETER_RANGE = 2; // values: 2, 4, 8, 16 (G)

    BMI160X(byte addr) : Device(INERTIAL_UNIT), Sensor(true) { _i2cAddress = addr; }

    // interfaces
    bool    init();
    bool    sleep();
    bool    wake();
    bool    check();
    bool    update();
    float   read();

    // data updated
    vfield  getMaxValue() { return _data.maxValue; }

    // curie class serial transport override
    void    ss_init(); // called in CurieIMUClass::begin() function
    int     ss_xfer(uint8_t *buf, unsigned tx_cnt, unsigned rx_cnt); // called for transport

    // data updated
    vcoord  getGyroscope() { return vcoord {_gx, _gy, _gz}; } // in degree
    vcoord  getAccelerometer() { return vcoord {_ax, _ay, _az}; } // in G
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
    float   _maxValueBuffer = 0;
    long    _timer = 0;
    float   _gx;
    float   _gy;
    float   _gz;
    float   _ax;
    float   _ay;
    float   _az;
    float   _temperature;
    float   _convertRawGyro(int gRaw); // convert to °
    float   _convertRawAcceleration(int aRaw); // convert to G
    float   _convertToCelcius(int raw);

    // human readable buffer. Updated by udpate function
    struct fields {
      vfield   maxValue = {"Quantité mouvement", "g", 0.1};
    };
    fields _data;

    vlegend _maxValues[1] = {
      {0, VERT, "ok"},
    };

};

#endif
