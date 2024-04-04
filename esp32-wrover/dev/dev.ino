
#include "Arduino.h"

// put includes here

#include "VDeviceESP32.h"
#include "VUsePins.h"
#include "VData.h"

// custom classes go there

class Test : public VData, public VUseAdcPin
{
  public:
    Test(byte pin) {
      _attachedPin = pin;
    }
    bool  init()    { return _initADC(_attachedPin, false, 4096, 0); }
    float read()    { return _readADC(); }

    float smooth(float value, int factor)     { return _smooth(value, factor); }
    float maximum(float value, int factor)    { return _maximum(value, factor); }
    float inertia(float value, int factor)    { return _inertia(value, factor); }



  private:
    byte  _attachedPin;
};

// put instanciations here

VDeviceESP32    esp(2);
Test            test(35);

void setup() 
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");  

  // put your setup code here, to run once:

  esp.init();
  esp.onboardedLed(true);

  test.init();

}

long timer;
int t;
float a,b,c,d;

void loop()
{
  // put your main code here, to run repeatedly:

  /*
  for (int i=0; i < 4096; i++) {
    esp.onboardedLed(i);
    Serial.println(i);
    delay(1);
  }
  esp.onboardedLed(false);
  */

  timer = micros();
  //********************************
  
  a = test.read() * 1000;
  b = test.smooth(a, 7);
  c = test.maximum(b, 10); 
  d = test.inertia(b,10); 

  //********************************
  t = micros() - timer;

  
  Serial.print("42000,");
  Serial.print(a); Serial.print(",");
  Serial.print(b); Serial.print(",");
  Serial.print(c); Serial.print(",");
  Serial.print(d); Serial.print(",");
  Serial.print("41755,");
  Serial.println("");
   
  //Serial.println(t);
 

// milieu 41.85
    
  //Serial.println("**********");
  //delay(1000);


}
