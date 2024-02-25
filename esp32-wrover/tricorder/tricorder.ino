#include "VSensors.h"
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
#include "VSensorPA1010D.h"

#define TRICORDER_REFRESH_RATE  3000

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
VSensorPA1010D    gps;
unsigned int      i;

field_data getSensor(sensors_code code)
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
  buffer[code].push(getSensor(code).value);
}

String getAllSensorsTable()
{
  field_data sensors[22];
  sensors[0] = air.getTemperature();
  sensors[1] = air.getPressure();
  sensors[2] = air.getHumidity();
  sensors[3] = air.getGasResistance();
  sensors[4] = air.getAirQuality();
  sensors[5] = air.getCo2Equivalent();
  sensors[6] = air.getVocEquivalent();
  sensors[7] = air.getGasPercentage();
  sensors[8] = emf.getMaxValue();
  sensors[9] = emf.getFrequency();
  sensors[10] = light.getUvIndex();
  sensors[11] = light.getVisible();
  sensors[12] = esp.getLoad();
  sensors[13] = esp.getMemory();
  sensors[14] = gps.getSatellites();
  sensors[15] = gps.getFixQuality();
  sensors[16] = gps.getLatitude();
  sensors[17] = gps.getLongitude();
  sensors[18] = gps.getAltitude();
  sensors[19] = gps.getSpeed();
  sensors[20] = gps.getDirectionAngle();
  sensors[21] = gps.getCompassAngle();

  return html.handleDataTable(sensors, 22);
}

void setup() 
{
  logger.begin();

  air.begin(0x77);
  emf.begin(36); 
  light.begin(0x53);
  esp.begin();
  //gps.begin(0x10);
  sound.begin(4);
    
  wifi.begin();

  for (int field = 0; field < VSENSORS_COUNT; field++) {
    buffer[field] = VDataBuffer();
    web.onHtml("/graph/" + String(field) + ".svg", [field](){ return html.handleHistorySvgGraph(buffer[field], getSensor((sensors_code) field)); });
  }
  web.onHtml("/logger", [](){ return wifi.getTime() + "  " + logger.dump(TRICORDER_REFRESH_RATE); });
  web.onHtml("/sensors", [](){ return getAllSensorsTable(); });
  web.onHtml("/", [](){ return html.handleHomePage(3000); });
  web.begin();
  
  sound.getOpen();
}

void loop() 
{
  if (web.update(10)) {

  }

  if (air.update(TRICORDER_REFRESH_RATE)) {
    setBuffer(TEMPERATURE);
    setBuffer(PRESSURE);
    setBuffer(HUMIDITY);
    setBuffer(AIR_QUALITY);
    setBuffer(GAS_PERCENTAGE);
  }

  if (emf.update(TRICORDER_REFRESH_RATE)) {
    setBuffer(EMF_FIELD);
  }

  if (light.update(TRICORDER_REFRESH_RATE)) {
    setBuffer(UV_INDEX);
    setBuffer(VISIBLE);
  }

  if (esp.update(TRICORDER_REFRESH_RATE)) {
    setBuffer(ESP_LOAD);
  }

  if (gps.update(TRICORDER_REFRESH_RATE)) {

  }

  if (logger.update(TRICORDER_REFRESH_RATE)) {
    for (i = 0; i < VSENSORS_COUNT; i++) {
      field_data value = getSensor((sensors_code) i);
      if (value.status >= JAUNE) {
        logger.println(" | " + value.label + ": " + value.text);
      }
    }
  }


}
