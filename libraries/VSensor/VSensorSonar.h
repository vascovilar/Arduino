/*
 * Read data on HC-SR04 ultrasonic sensor
 * Implementation:
 *
 *   #include <VSensorSonar.h>
 *
 *   const int TRIGPIN = 5;
 *   const int ECHOPIN = 18;
 *
 *   VSensorSonar sonar; 
 *
 *   void setup() {
 *     sonar.begin(TRIGPIN, ECHOPIN);
 *   }
 *   void loop() {
 *     if (sonar.update(100)) {
 *       float cm = sonar.getDistance();
 *     };
 *   }
 */

#ifndef VSensorSonar_h
#define VSensorSonar_h

#include "Arduino.h"

#define SOUND_SPEED 0.03436 // in cm/uS for 22°C

struct sonar_field_data {
  float    distance;    // in cm
};

class VSensorSonar
{
  public:
    void begin(int trigPin, int echoPin);
    bool update(int delay);

    sonar_field_data dump() { return _data; }
    float getDistance() { return _data.distance; }

  private:
    int _trigPin;
    int _echoPin;
    struct sonar_field_data _data;
    uint32_t _timer = millis();
    
    float _read();
};

#endif
