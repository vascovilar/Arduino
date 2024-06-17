#include <VModel.h>
#include <VBuffer.h>
#include <VLang.h>
#include <VHtml.h>
#include <VMind.h>
#include <VComWifi.h>
#include <VComWebApi.h>
#include <VSensorMeteo.h>
#include <VSensorLight.h>
#include <VMotorHardware.h>

// fix I2C without this
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

VModel         model;
VLang          lang;
VHtml          html;
VComWifi       net;
VComWebApi     api;
VSensorMeteo   meteo;
VSensorLight   light;
VMotorHardware hard;
VMind          comfort;
VBuffer graph[12] = { VBuffer(),VBuffer(), VBuffer(), VBuffer(), VBuffer(), VBuffer(), VBuffer(), VBuffer(), VBuffer(), VBuffer(), VBuffer(), VBuffer() };

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  model.begin(&Serial);
  
  meteo.begin(0x76);
  light.begin(0x60);
  hard.begin(23);
  
  net.begin();

  api.onPage("/", [](){ return html.getHome(5000); });
  api.onPage("/uptime", [](){ return html.getUptime(); });

  for (int i = 0; i < 12; i++) {
    api.onSvg("/graph/" + String(i) + ".svg", [i](){ return html.getGraph(i, graph[i].data, comfort.info(i)); });
    api.onCommand("/graph/" + String(i) + "/reset", [i](){ graph[i].reset(); });
    api.onCommand("/graph/" + String(i) + "/slice/{}", [i](int arg){ graph[i].slice(arg); });
  }
  api.begin();
}

void loop()
{
  if (api.update(5000)) {
    Serial.println("update server usage");
    if (graph[SERVER].push(api.getServerUsage())) {
      comfort.analyse(SERVER, 0, graph[SERVER].data);
    }
  }

  if (hard.update(1000)) {
    Serial.println("update memory usage");
    if (graph[MEMORY].push(hard.getMemoryUsage())) {
      comfort.analyse(MEMORY, 0, graph[MEMORY].data);
    }
  }
  
  if (meteo.update(3000)) {
    Serial.println("update meteo sensor");
    if (graph[TEMPERATURE].push(meteo.getTemperature())) {
      comfort.analyse(TEMPERATURE, 0, graph[TEMPERATURE].data);
    } 
    if (graph[PRESSURE].push(meteo.getPressure())) {
      comfort.analyse(PRESSURE, meteo.getPressureStatus(), graph[PRESSURE].data);
    } 
    if (graph[HUMIDITY].push(meteo.getHumidity())) {
      comfort.analyse(HUMIDITY, 0, graph[HUMIDITY].data);
    } 
    if (graph[GAS_RESISTANCE].push(meteo.getGasResistance())) {
      comfort.analyse(GAS_RESISTANCE, 0, graph[GAS_RESISTANCE].data);
    } 
    if (graph[AIR_QUALITY].push(meteo.getAirQuality())) {
      comfort.analyse(AIR_QUALITY, meteo.getAirStatus(), graph[AIR_QUALITY].data);
    }
    if (graph[CO2_EQUIVALENT].push(meteo.getCo2Equivalent())) {
      comfort.analyse(CO2_EQUIVALENT, 0, graph[CO2_EQUIVALENT].data);
    } 
    if (graph[VOC_EQUIVALENT].push(meteo.getBreathVocEquivalent())) {
      comfort.analyse(VOC_EQUIVALENT, 0, graph[VOC_EQUIVALENT].data);
    }
  }

  if (light.update(1000)) {
    Serial.println("update light sensor");
    if (graph[UV_INDEX].push(light.getUVIndex())) {
      comfort.analyse(UV_INDEX, 0, graph[UV_INDEX].data);
    }
    if (graph[VISIBLE].push(light.getVisible())) {
      comfort.analyse(VISIBLE, 0, graph[VISIBLE].data);
    }
    if (graph[INFRARED].push(light.getInfraRed())) {
      comfort.analyse(INFRARED, 0, graph[INFRARED].data);
    }
  }
}
