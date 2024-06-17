#include <VLog.h>
#include <VScreenMini.h>
#include <VSensorSonar.h>

VLog          out;
VScreenMini   view;
VSensorSonar  sonar;

void setup()
{
  out.begin(&Serial);
  view.begin(0x3C);
  sonar.begin(5,18); 
}

void loop() 
{
  if (sonar.update(100)) {
    String output = "Distance: " + String(sonar.getDistance()) + " cm\n";
  
    view.clear();
    view.printAt(0, 0, output);
    view.display();  
  }
}
