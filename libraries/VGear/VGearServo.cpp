#include "VGearServo.h"


void VGearServo::begin(int pinH, int pinV)
{
  _servoH.setPeriodHertz(SERVO_FREQUENCY);
  _servoV.setPeriodHertz(SERVO_FREQUENCY);

  _servoH.attach(pinH, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
  _servoV.attach(pinV, SERVO_MIN_PULSE, SERVO_MAX_PULSE);

  _servoH.write(SERVO_H_CENTER);
  _servoV.write(SERVO_V_CENTER);
}


void VGearServo::center()
{
  bool dx, dy;
  do {
    dx = go(&_servoH, SERVO_H_CENTER);
    dy = go(&_servoV, SERVO_V_CENTER);

    delay(5);
  } 
  while (dx || dy);
}

void VGearServo::left()
{
  inc(&_servoH, SERVO_INCREMENT);
}

void VGearServo::right()
{
  inc(&_servoH, -SERVO_INCREMENT);
}

void VGearServo::up()
{
  inc(&_servoV, SERVO_INCREMENT);
}

void VGearServo::down()
{
  inc(&_servoV, -SERVO_INCREMENT);
}


int VGearServo::pos(Servo* servo)
{
  return servo->read();
}

void VGearServo::inc(Servo* servo, int dx)
{
  servo->write(pos(servo) + dx);  
}

bool VGearServo::go(Servo* servo, int dest) 
{
  int p = pos(servo);

  if (p == dest) {
    return false;
  }
    
  float dx = (dest - p) / (float) SERVO_COOLNESS;  
  if (dx > 0) dx++;
  if (dx < 0) dx--;
  servo->write(p + int(dx));

  return true; 
}
