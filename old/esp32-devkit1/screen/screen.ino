#include "VGearScreen.h"

VGearScreen screen;

int len = 10;
String menu[] = {
  "Menu > LCD > Off",
  "Menu > LED > Noop",
  "Menu > LED > Off",
  "Menu > LED > On",
  "Menu > LED > Fade",
  "Menu > LED > Blink",
  "Menu > LCD > light",
  "Menu > Nothing1",  
  "Menu > Nothing2",
  "Menu > Nothing3"
};

// lambdas

bool exec(String line)
{
  if (line == "Menu > LED > Off") {
      screen.clear();
      screen.title("OK", 0, 0);
      return true;
  } 
   
  return false;
}

void setup(void) 
{
  Serial.begin(115200);
  screen.begin(menu, len);
  pinMode(39, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
}

void loop()
{
  bool up = !digitalRead(39);
  int go = !digitalRead(34);
  int down = !digitalRead(35);

  if (up) {
    screen.next(-1);
  }
  if (go) {
    screen.click(exec);
  }
  if (down) {
    screen.next(1);
  }

  delay(100);
}