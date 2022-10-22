#include <VLog.h>
#include <VScreenMini.h>
#include <VSensorLight.h>

VLog         out;
VScreenMini  view;
VSensorLight light; 

void setup() 
{
  out.begin(&Serial);
  view.begin(0x3C);
  light.begin(0x60);
}

void loop() 
{
  if (light.update(100)) { 
    String output = "IndexUV:  " + String(light.getUVIndex()) + "\n"
                  + "Visible:  " + String(light.getVisible()) + " lux\n"
                  + "InfraRed: " + String(light.getIR()) + " lux\n";
  
    view.clear();
    view.printAt(0, 0, output);
    view.display();
  }
}
