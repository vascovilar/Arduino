
#include <ESP32Servo.h>
#include <math.h>

Servo servoH;
Servo servoV;

const int SERVOPINH = 12;
const int SERVOPINV = 13;

void setup() 
{
  Serial.begin(115200);
  delay(1000);

  servoH.setPeriodHertz(50);
  servoH.attach(SERVOPINH);
  servoV.setPeriodHertz(50);
  servoV.attach(SERVOPINV, 500, 2500);  
}

bool go(Servo* servo, int dest) 
{
  int pos = servo->read();

  if (pos == dest) {
    return false;
  }
    
  float dx = (dest - pos) / 1000.0;  
  if (dx > 0) dx++;
  if (dx < 0) dx--;
  
  servo->write(pos + int(dx));  
  Serial.println(pos + int(dx));
  return true; 
}



void loop() 
{
  int x = random (1, 180);
  int y = random (1, 180);

  //x = 89;
  //y = 60;

  Serial.println("*********************************************");
  Serial.println("Destination : " + String(x) + "," + String(y));

  bool dx;
  bool dy;
  
  do {
    dx = go(&servoH, x);
    dy = go(&servoV, y);
    //delay(5);
  } 
  while (dx || dy);
  
  Serial.println("Done !");
  
  delay(1000);
}
