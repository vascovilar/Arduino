#include "VEnum.h"
#include "VDataBuffer.h"
#include "VDataHtml.h"
#include "VDeviceWifi.h"
#include "VDeviceWebServer.h"
#include "VSensorBME680.h"
#include "VSensorEMF.h"

VDataBuffer       buffer[VSENSORS_COUNT];
VDataHtml         html;
VDeviceWifi       net;
VDeviceWebServer  web;
VSensorBME680     air;
VSensorEMF        emf;

field_data getData(int code)
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
  }

  return data;
}

void setData(int code)
{
  buffer[code].push(getData(code).value);
}

void setup() 
{
  Serial.begin(115200);
  delay(1000);
  
  air.begin(0x77);
  emf.begin(36); 
  net.begin();

  for (int field = 0; field < VSENSORS_COUNT; field++) {
    buffer[field] = VDataBuffer();
    web.onHtml("/graph/" + String(field) + ".svg", [field](){ return html.handleSvgGraph(buffer[field], getData(field)); });
  }
  web.onHtml("/", [](){ return html.handleMeteoHome(3000); });
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
  
  web.update(10);

  if (air.update(3000)) {
    setData(TEMPERATURE);
    setData(PRESSURE);
    setData(HUMIDITY);
    setData(AIR_QUALITY);
    setData(GAS_PERCENTAGE);
  }

  if (emf.update(1000)) {
    setData(EMF_FIELD);
  }

}

