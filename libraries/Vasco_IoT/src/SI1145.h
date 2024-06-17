/*
 * Read uv index, infrared and visible light with Adafruit SI1145
 * Ref: https://learn.adafruit.com/adafruit-si1145-breakout-board-uv-ir-visible-sensor
 * Doc: https://www.silabs.com/documents/public/data-sheets/Si1145-46-47.pdf
 */

#ifndef SI1145_h
#define SI1145_h

#define ARDUINO 100

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Sensor.h"
#include "component/Pins.h"
#include "Wire.h"
#include "Adafruit_SI1145.h"


class SI1145 : public Sensor, public I2cPins
{
  public:

    SI1145(byte addr) : Sensor(LIGHT_SENSOR, false) { _i2cAddress = addr; }

    // interfaces
    bool    init();
    bool    sleep();
    bool    wake();
    bool    check();
    bool    update();
    float   read();
    vfield  get(vsensor code)
    {
      switch (code) {
        case INFRARED:
          return _infrared;
        case VISIBLE:
          return _visible;
        case UV_INDEX:
          return _uvIndex;
      }

      return (vfield){};
    }

  private:

    Adafruit_SI1145 _uv = Adafruit_SI1145();
    byte            _i2cAddress;
    vfield          _infrared = {"Infrarouge", "", 50};
    vfield          _visible = {"Lumière ambiante", "lux", 50};
    vfield          _uvIndex = {"Index UV", "", 1};

    float _convertToUVIndex(float uv);

    vlegend _infrareds[1] = {
      {0, VERT, "ok"}
    };

    vlegend _visibles[10] = {
      {1, VERT, "obscurité"},
      {20, VERT, "faible"},
      {100, VERT, "tamisée"},
      {500, VERT, "confortable"},
      {1000, VERT, "bureau"},
      {10000, VERT, "ciel couvert"},
      {50000, VERT, "ciel nuageux"},
      {100000, JAUNE, "ciel d'été"},
      {500000, ORANGE, "trop lumineux"},
    };

    vlegend _uvIndexes[5] = {
      {2, VERT, "risque faible"},
      {5, JAUNE, "risque modéré"},
      {7, ORANGE, "risque élevé"},
      {10, ROUGE, "risque très élevé"},
      {12, ROUGE, "risque extrême"},
    };
};

#endif
