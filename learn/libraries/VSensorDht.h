/*
 * Read data on DHT22
 * Implementation:
 *
 *   #include <VSensorDht.h>
 *
 *   const int DHTPIN = 4;
 *
 *   VSensorDht dht; 
 *
 *   void setup() {
 *     dht.begin(DHTPIN);
 *   }
 *   void loop() {
 *     float temp = dht.getTemperature();
 *     delay(2000);
 *   }
 */

#ifndef VSensorDht_h
#define VSensorDht_h

#include "Arduino.h"
#include "DHTesp.h"

class VSensorDht
{
  public:
    void begin(int pin);
    float getTemperature();
    float getHumidity();
    ComfortState getComfort();
    // computeHeatIndex
    // computeDewPoint
    // computePerception
    // computeAbsoluteHumidity
  private:
    DHTesp _dht;
};

#endif
