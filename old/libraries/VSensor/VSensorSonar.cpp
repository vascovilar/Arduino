#include "VSensorSonar.h"

void VSensorSonar::begin(int trigPin, int echoPin)
{
  _trigPin = trigPin;
  _echoPin = echoPin;
   
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
}

bool VSensorSonar::update(int delay)
{
  // get data every 100 ms
  if (millis() - _timer > delay) {
    _timer = millis(); // reset the timer

    _data.distance = _read();

    return true;
  }

  return false;
}

float VSensorSonar::_read()
{
  // Clears the trigPin
  digitalWrite(_trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(_echoPin, HIGH);
  
  // Calculate the distance in cm
  return duration * SOUND_SPEED / 2.0;
}
