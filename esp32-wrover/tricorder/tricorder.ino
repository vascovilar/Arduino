#include "VConfig.h"
#include "VDataBuffer.h"
#include "VDataHtml.h"
#include "VDeviceWifi.h"
#include "VDeviceWebServer.h"
#include "VSensorBME680.h"
#include "VSensorEMF.h"
#include "VSensorLTR390.h"
#include "VSensorESP32.h"

VDataBuffer       buffer[VSENSORS_COUNT];
VDataHtml         html;
VDeviceWifi       net;
VDeviceWebServer  web;
VSensorBME680     air;
VSensorEMF        emf;
VSensorLTR390     light;
VSensorESP32      esp;

field_data getValue(int code)
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

void setBuffer(int code)
{
  buffer[code].push(getValue(code).value);
}

void setup() 
{
  Serial.begin(115200);
  delay(1000);
  
  air.begin(0x77);
  emf.begin(36); 
  light.begin(0x53);
  esp.begin();
  net.begin();


  for (int field = 0; field < VSENSORS_COUNT; field++) {
    buffer[field] = VDataBuffer();
    web.onHtml("/graph/" + String(field) + ".svg", [field](){ return html.handleHistorySvgGraph(buffer[field], getData(field)); });
  }
  web.onHtml("/", [](){ return html.handleHomePage(3000); });
  web.begin();
}

void loop() 
{
  /*
  if (emf.update(1000)) {
    float* buffer = emf.getBuffer();
    for (int i = 0; i<100; i++) {
      Serial.println(buffer[i]);
    } 
  }
  */
  
  if (web.update(10)) {
    esp.addLoad(web.getProcessTime());
  }

  if (air.update(3000)) {
    setBuffer(TEMPERATURE);
    setBuffer(PRESSURE);
    setBuffer(HUMIDITY);
    setBuffer(AIR_QUALITY);
    setBuffer(GAS_PERCENTAGE);
    esp.addLoad(air.getProcessTime());
  }

  if (emf.update(1000)) {
    setBuffer(EMF_FIELD);
    esp.addLoad(emf.getProcessTime());
  }

  if (light.update(1000)) {
    setBuffer(UV_INDEX);
    setBuffer(VISIBLE);
    esp.addLoad(light.getProcessTime());
  }

  if (esp.update(1000)) {
    setBuffer(ESP_LOAD);
  }
}

