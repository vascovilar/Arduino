#include "VConfig.h"
#include "VDataBuffer.h"
#include "VDataHtml.h"
#include "VDataLogger.h"
#include "VToolWifi.h"
#include "VToolWebServer.h"
#include "VToolSound.h"
#include "VSensorBME680.h"
#include "VSensorEMF.h"
#include "VSensorLTR390.h"
#include "VSensorESP32.h"

VDataBuffer       buffer[VSENSORS_COUNT];
VDataHtml         html;
VDataLogger       logger;
VToolWifi         wifi;
VToolWebServer    web;
VToolSound        sound;
VSensorBME680     air;
VSensorEMF        emf;
VSensorLTR390     light;
VSensorESP32      esp;

field_data getValue(sensors_code code)
{
  field_data data = {};

  switch (code) {
    case TEMPERATURE:
      data = air.getTemperature();
      break;
    case PRESSURE:
      data = air.getPressure();
      break;
    case HUMIDITY:
      data = air.getHumidity();
      break;
    case AIR_QUALITY:
      data = air.getAirQuality();
      break;
    case GAS_PERCENTAGE:
      data = air.getGasPercentage();
      break;
    case EMF_FIELD:
      data = emf.getMaxValue();
      break;
    case UV_INDEX:
      data = light.getUvIndex();
      break;
    case VISIBLE:
      data = light.getVisible();
      break;
    case ESP_LOAD:
      data = esp.getLoad();
      break;
  }

  return data;
}


void setBuffer(sensors_code code)
{
  buffer[code].push(getValue(code).value);
}

void setLogger()
{
  for (int i = 0; i < VSENSORS_COUNT; i++) {
    field_data value = getValue((sensors_code) i);
    if (value.status >= JAUNE) {
      logger.println(" | " + value.label + ": " + value.text);
    }
  }
}

void setup() 
{
  logger.begin();

  air.begin(0x77);
  emf.begin(36); 
  light.begin(0x53);
  esp.begin();
  sound.begin(2);
    
  wifi.begin();

  for (int field = 0; field < VSENSORS_COUNT; field++) {
    buffer[field] = VDataBuffer();
    web.onHtml("/graph/" + String(field) + ".svg", [field](){ return html.handleHistorySvgGraph(buffer[field], getValue((sensors_code) field)); });
  }
  web.onHtml("/logger", [](){ return wifi.getTime() + "  " + logger.dump(); });
  web.onHtml("/", [](){ return html.handleHomePage(3000); });
  web.begin();
  
  sound.getOpen();
}

void loop() 
{
  if (web.update(10)) {
  }

  if (air.update(3000)) {
    setBuffer(TEMPERATURE);
    setBuffer(PRESSURE);
    setBuffer(HUMIDITY);
    setBuffer(AIR_QUALITY);
    setBuffer(GAS_PERCENTAGE);
  }

  if (emf.update(1000)) {
    setBuffer(EMF_FIELD);
  }

  if (light.update(1000)) {
    setBuffer(UV_INDEX);
    setBuffer(VISIBLE);
  }

  if (esp.update(1000)) {
    setBuffer(ESP_LOAD);
    setLogger();
    

    //sound.getNotice();
    //light.sync();
    //Serial.println("  " + wifi.getTime() + "  " + String(light.getVisible().value) + "  " + String(light.getProcessTime()) + "ms");
  }

  
}
