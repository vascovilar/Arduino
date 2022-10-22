#include <VModel.h>
#include <VScreenMini.h>

VModel        out;
VScreenMini view;

int col;
int row;

void reset()
{
  view.display(); 
  delay(5000);
  view.clear();
  col = 0;
  row = 0;
}

void setup()   
{ 
  out.begin(&Serial);
  view.begin(0x3C); 
}

void loop() 
{
  reset();
  
  for (int i = 0; i < 256; i++) {
      if (i != 0) {
        row = row + 8;
        if ((i % 8) == 0) {
          col = col + 32;
          row = 0;
        }
        if ((i % 32) == 0) {
          reset();
        }
      }
      view.charAt(col, row, i);
      Serial.println(String(i) + " " + char(i));
  }
}
