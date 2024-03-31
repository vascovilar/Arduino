#include "Arduino.h"
#include "VData.h"
#include "VDataBuffer.h"
#include "VDataHtml.h"
#include "VDataWebServer.h"
#include "VDevice.h"
#include "VDeviceESP32.h"
#include "VDeviceBuzzer.h"
#include "VDeviceST7789SD.h"
#include "VSensor.h"
#include "VSensorEMF.h"
#include "VSensorLTR390.h"
#include "VSensorSEN0487.h"
#include "VSensorBME680.h"
#include "VSensorPA1010D.h"
#include "VRunSequencer.h"

VDeviceESP32      esp(2);
VDeviceBuzzer     buzzer(4);
VDeviceST7789SD   tft(5, 25, 33, 32);
VSensorEMF        emf(36);
VSensorLTR390     sun(0x53);
VSensorSEN0487    ear(34);
VSensorBME680     air(0x77);
VSensorPA1010D    gps(0x10);

VRunSequencer     deviceEMF(emf);
VRunSequencer     deviceSun(sun);
VRunSequencer     deviceEar(ear);
VRunSequencer     deviceAir(air);
VRunSequencer     deviceGPS(gps);
VDataBuffer       graph[VSENSOR_COUNT];
VDataHtml         html;
VDataWebServer    server;


void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");  

  // -----------------------------------------------------
  // devices
  // -----------------------------------------------------

  esp.init();
  buzzer.init();
  tft.init();

  deviceEMF.begin(LOW_REFRESH); // TODO vasco passer les messages d'erreur ici bool begin
  deviceSun.begin(LOW_REFRESH);
  deviceEar.begin(LOW_REFRESH);
  deviceAir.begin(LOW_REFRESH);
  deviceGPS.begin(LOW_REFRESH);
  
  Serial.println("Devices: initialized");

  for (int field = 0; field < VSENSOR_COUNT; field++) {
    graph[field] = VDataBuffer();
  }

  // -----------------------------------------------------
  // wifi 
  // -----------------------------------------------------
  
  /*
  int lenght = esp.getWifiAccessPoints();
  Serial.println("bornes: " + String(lenght));
  
  for (int i = 0; i < lenght; i++) {
    Serial.println(esp.getWifiAccessPointInfo(i));
  }
  */
  
  if (esp.connectWifi()) {
    esp.blueLed(true);
    Serial.println("Wifi: sync dateTime from web, " + esp.getDateTime());
  }

  /*if (esp.disconnectWifi()) {
    esp.blueLed(false);
  }*/

  // -----------------------------------------------------
  // webserver
  // -----------------------------------------------------

  server.onHtml("/", [](){ return html.handleHomePage(3000); });
  for (int field = 0; field < VSENSOR_COUNT; field++) {
    server.onHtml("/graph/" + String(field) + ".svg", [field](){ return html.handleHistorySvgGraph(graph[field], getSensor((vsensor_code) field)); });
  }
  //server.onHtml("/logger", [](){ return esp.getDateTime() + "  " + logger.dump(SHORT_REFRESH_RATE); });
  //server.onHtml("/sensors", [](){ return getAllSensorsTable(); });
  if (server.init()) {
    Serial.println("Webserver: listening on port 80, go http://" + esp.getDeviceIP());
  }
  
  // -----------------------------------------------------
  // tft
  // -----------------------------------------------------
  
  tft.light(127);
  tft.title("Bonjour !", 0, 0, COLOR_WHITE);
  tft.text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", 0, 20, COLOR_GREY);

  // -----------------------------------------------------
  // sd
  // -----------------------------------------------------

  tft.listFiles();

  // -----------------------------------------------------
  // buzzer
  // -----------------------------------------------------
  
  buzzer.warning();
  
  
  
  
  Serial.println();
}

void loop() 
{
  server.run();

  if (deviceEMF.update()) {
    setBuffer(EMF_LEVEL);
  }

  if (deviceSun.update()) {
    setBuffer(UV_INDEX);
    setBuffer(VISIBLE);
  }

  if (deviceEar.update()) {
    setBuffer(EAR_LEVEL);
  }

  if (deviceAir.update()) {
    setBuffer(TEMPERATURE);
    setBuffer(PRESSURE);
    setBuffer(HUMIDITY);
    setBuffer(AIR_QUALITY);
    setBuffer(GAS_PERCENTAGE);
  }

  if (deviceGPS.update()) {
    setBuffer(ALTITUDE);
    Serial.println(gps.getDateTime() + ", sat:" + String(gps.getSatellite().value) + " " + String(gps.getSatellite().text));
  }
}





// TODO vasco include in a sequencer array, warning: hérite de VRunSequencer pour pas pourrir la classe
field_data getSensor(vsensor_code code)
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
    case ALTITUDE:
      data = gps.getAltitude();
      break;  
  }

  return data;
}

void setBuffer(vsensor_code code)
{
  field_data sensor = getSensor(code);
  if (sensor.status > 0) {
    graph[code].push(sensor.value);
  }
}