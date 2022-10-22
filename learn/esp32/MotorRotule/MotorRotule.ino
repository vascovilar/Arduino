#include <VLog.h>
#include <VMotorServo.h>

VLog        out;
VMotorServo servo;

void setup() {
  out.begin(&Serial);
  servo.begin(4, 14);
}

void loop() {
  servo.left();
  delay(100);
}
