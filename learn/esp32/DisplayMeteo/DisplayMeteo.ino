
#include <VLog.h>
#include <VLang.h>
#include <VIcon.h>
#include <VScreenMini.h>
#include <VSensorMeteo.h>
#include <VBuffer.h>

VLog          out;
VLang         lang;
VIcon         icon; 
VScreenMini   view;
VSensorMeteo  meteo;
VBuffer       graphIAQ;

void setup()
{
  out.begin(&Serial);
  view.begin(0x3C);
  meteo.begin(0x76);
}

void loop() 
{  
  if (meteo.update(3000)) {  
    
    String output = "Temp: " + String(meteo.getTemperature()) + " " + char(9) + "C\n"
                  + "Pres: " + String(meteo.getPressure()) + " hPa\n"
                  + "Humi: " + String(meteo.getHumidity()) + " %\n"
                  //+ "Resi: " + String(meteo.getGasResistance()) + " kOhm\n"
                  + "IAQ:  " + String(int(meteo.getAirQuality())) + " " + lang.get(meteo.getAirStatus()) + "\n"
                  //+ "CO2:  " + String(meteo.getCo2Equivalent()) + " ppm\n"
                  //+ "VOC:  " + String(meteo.getBreathVocEquivalent()) + " ppm\n"
                  ;
    out.println(output);
    graphIAQ.push(meteo.getAirQuality());
    
    view.clear();
    view.printAt(0, 0, output);
    //view.numericBox(38, 0, "Temp", meteo.getTemperature(), 9);
    view.iconBox(0, 32, icon.get(Sky_SunAndCloud));    
    view.graphBox(36, 34, 30, graphIAQ.getMaximum(), graphIAQ.getMinimum(), graphIAQ.getAverage(), graphIAQ.data); 
    view.display();    
  }
}
