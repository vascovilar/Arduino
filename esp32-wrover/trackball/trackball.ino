
#include "Arduino.h"

// put includes here
#include "PIM447.h"
#include "interface/Data.h"
#include "interface/Device.h"


// custom classes go there



// put instanciations here
PIM447 trackball(0x0A);


void setup() 
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");  

  // put your setup code here, to run once:
  
  Wire.begin(21, 22);

  trackball.init();
  trackball.setBoundary(240, 135);
  
  trackball.led(COLOR_GREEN, 0); Serial.println("green"); delay(500);
  trackball.led(COLOR_YELLOW, 0); Serial.println("yellow"); delay(500);
  trackball.led(COLOR_ORANGE, 0); Serial.println("orange"); delay(500);
  trackball.led(COLOR_RED, 0); Serial.println("red"); delay(500);
  trackball.led(COLOR_VIOLET, 0); Serial.println("violet"); delay(500);
  trackball.led(COLOR_BLACK, 0); 

   Serial.println("\nMove pointer: ");
}

long timer;
float a,b,c,d;
int t;

void loop()
{
  timer = micros();
  // put your main code here, to run repeatedly:

  if (trackball.update()) {
    vpointer pointer = trackball.getPointer();

    Serial.print(String(pointer.x) + "," + String(pointer.y) + " ");
    if (pointer.left) Serial.print("L");
    if (pointer.right) Serial.print("R");
    if (pointer.up) Serial.print("U");
    if (pointer.down) Serial.print("D");
    Serial.print(String((pointer.focus ? " [focus]": "")));
    Serial.print(String((pointer.click ? " [click]": "")));
    Serial.println(" " + String(micros() - timer) + "uS");
  }

/*
  t = 0;
  timer = 2000;
  trackball.led(0x000000, 0); delay(200);
  trackball.led(0xFF0000, t); delay(timer);
  trackball.led(0x00FF00, t); delay(timer);
  trackball.led(0x0000FF, t); delay(timer);
  trackball.led(0xFFFFFF, t); delay(timer);
  trackball.led(0xFF00FF, 0); delay(timer);
  trackball.led(0x00FFFF, 0); delay(timer);
  trackball.led(0xFFFF00, 0); delay(timer);
*/  

  //Serial.println("********** " + String(micros() - timer) + "uS");
  delay(100);
}
