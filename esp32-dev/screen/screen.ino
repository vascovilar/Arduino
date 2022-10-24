#include "VComScreen.h"

VComScreen screen;

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
}

void loop()
{
  int up = touchRead(14);
  int go = touchRead(12);
  int down = touchRead(13);
  
  Serial.println(String(up) + " " + String(go) + " " + String(down));

  if (up < 25 && up > 20) {
    screen.next(-1);
  }
  if (go < 25 && go > 20) {
    screen.click(exec);
  }
  if (down < 25 && down > 20) {
    screen.next(1);
  }

  delay(100);
}