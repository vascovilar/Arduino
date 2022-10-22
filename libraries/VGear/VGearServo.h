/*
 * Move servo rotule
 * Implementation:
 *
 *   #include <VGearServo.h>
 *
 *   const int SERVOPINH = 4;
 *   const int SERVOPINV = 2;
 *
 *   VGearServo servo;
 *
 *   void setup() {
 *     servo.begin(SERVOPINH, SERVOPINV);
 *   }
 *   void loop() {
 *     servo.left();
 *     delay(100);
 *   }
 */

#ifndef VGearServo_h
#define VGearServo_h

#include "Arduino.h"
#include "ESP32Servo.h"

// old micro 9g model
#define SERVO_FREQUENCY   50
#define SERVO_MIN_PULSE   500
#define SERVO_MAX_PULSE   2500
#define SERVO_H_CENTER    90
#define SERVO_V_CENTER    60
#define SERVO_COOLNESS    10
#define SERVO_INCREMENT   5


class VGearServo
{
  public:
    void begin(int pinH, int pinV);

    void center();
    void left();
    void right();
    void up();
    void down();
    
  private:
    Servo _servoH;
    Servo _servoV;
    
    int pos(Servo* servo);
    void inc(Servo* servo, int dx);
    bool go(Servo* servo, int dest);
};

#endif
