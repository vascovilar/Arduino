#include "VSensors.h"
#include "VDataBuffer.h"
#include "VDataHtml.h"
#include "VDataLogger.h"
#include "VDataWebServer.h"
#include "VDeviceESP32.h"
#include "VDeviceBuzzer.h"
#include "VSensorBME680.h"
#include "VSensorEMF.h"
#include "VSensorLTR390.h"
#include "VSensorESP32.h"
#include "VSensorPA1010D.h"
#include "VSensorSEN0487.h"

#define SHORT_REFRESH_RATE  3000
#define LONG_REFRESH_RATE  10000


VDataBuffer       buffer[VSENSOR_COUNT];
VDataHtml         html;
VDataLogger       logger;
VDataWebServer    server;
VDeviceESP32      esp(2);
VDeviceBuzzer     sound(4);
VSensorBME680     air(0);
VSensorEMF        emf(36);
VSensorLTR390     sun(0);
VSensorPA1010D    gps(0);
VSensorSEN0487    ear(34);
unsigned int      i;

field_data getSensor(sensor_code code)
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
    case EMF_LEVEL:
      data = emf.getMaxValue();
      break;
    case UV_INDEX:
      data = sun.getUvIndex();
      break;
    case VISIBLE:
      data = sun.getVisible();
      break;
    case EAR_LEVEL:
      data = ear.getMaxValue();
      break;
  }

  return data;
}

void setBuffer(sensor_code code)
{
  buffer[code].push(getSensor(code).value);
}

String getAllSensorsTable()
{
  field_data sensors[100];

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
  sensors[10] = sun.getUvIndex();
  sensors[11] = sun.getVisible();
  sensors[12] = esp.getMemoryUsage();
  sensors[13] = gps.getSatellite();
  sensors[14] = gps.getFixQuality();
  sensors[15] = gps.getLatitude();
  sensors[16] = gps.getLongitude();
  sensors[17] = gps.getAltitude();
  sensors[18] = gps.getSpeed();
  sensors[19] = gps.getDirectionAngle();
  sensors[20] = gps.getCompassAngle();
  sensors[21] = ear.getMaxValue();
  
  field_data airTime = {"Air : Durée mesure", "ms", 100, air.getProcessTime(), air.isEnabled() ? VERT: GRIS, "-"};
  sensors[22] = airTime;
  field_data emfTime = {"EMF : Durée mesure", "ms", 100, emf.getProcessTime(), emf.isEnabled() ? VERT: GRIS, "-"};
  sensors[23] = emfTime;
  field_data sunTime = {"Luminosité : Durée mesure", "ms", 100, sun.getProcessTime(), sun.isEnabled() ? VERT: GRIS, "-"};
  sensors[24] = sunTime;
  field_data espTime = {"ESP32 : Durée mesure", "ms", 100, esp.getProcessTime(), esp.isEnabled() ? VERT: GRIS, "-"};
  sensors[25] = espTime;
  field_data gpsTime = {"GPS : Durée mesure", "ms", 100, gps.getProcessTime(), gps.isEnabled() ? VERT: GRIS, "-"};
  sensors[26] = gpsTime;
  field_data earTime = {"Son : Durée mesure", "ms", 100, ear.getProcessTime(), ear.isEnabled() ? VERT: GRIS, "-"};
  sensors[27] = earTime;

  return html.handleDataTable(sensors, 28);
}

void setup() 
{
  air.begin(0x77);
  sun.begin(0x53);
  //gps.begin(0x10);
  emf.begin(36); 
  ear.begin(35);
  esp.begin();

  logger.start();
  
  esp.init();
  sound.init();

  //esp.connectWifi();

  for (int field = 0; field < VSENSOR_COUNT; field++) {
    buffer[field] = VDataBuffer();
    server.onHtml("/graph/" + String(field) + ".svg", [field](){ return html.handleHistorySvgGraph(buffer[field], getSensor((sensor_code) field)); });
  }
  server.onHtml("/logger", [](){ return esp.getDateTime() + "  " + logger.dump(SHORT_REFRESH_RATE); });
  server.onHtml("/sensors", [](){ return getAllSensorsTable(); });
  server.onHtml("/", [](){ return html.handleHomePage(3000); });
  server.init();
  
  sound.open();
}

void loop() 
{
  server.run();

  if (air.update(SHORT_REFRESH_RATE)) {
    setBuffer(TEMPERATURE);
    setBuffer(PRESSURE);
    setBuffer(HUMIDITY);
    setBuffer(AIR_QUALITY);
    setBuffer(GAS_PERCENTAGE);
  }

  if (emf.update(SHORT_REFRESH_RATE)) {
    setBuffer(EMF_LEVEL);
  }

  if (sun.update(SHORT_REFRESH_RATE)) {
    setBuffer(UV_INDEX);
    setBuffer(VISIBLE);
  }

  if (esp.update(LONG_REFRESH_RATE)) {

  }

  if (gps.update(LONG_REFRESH_RATE)) {

  }

  if (ear.update(SHORT_REFRESH_RATE)) {
    setBuffer(AUDIO_LEVEL);
  }

  if (logger.update(SHORT_REFRESH_RATE)) {
    for (i = 0; i < VSENSOR_COUNT; i++) {
      field_data value = getSensor((sensor_code) i);
      if (value.status >= JAUNE) {
        logger.println(" | " + value.label + ": " + value.text);
      }
    }
  }

}
