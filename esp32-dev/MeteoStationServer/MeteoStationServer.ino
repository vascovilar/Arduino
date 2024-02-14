#include "VMind.h"
#include "VBuffer.h"
#include "VLang.h"
#include "VHtml.h"
#include "VSensors.h"
#include "VSensorAir.h"
#include "VSensorLight.h"
#include "VSensorGPS.h"
#include "VSensorEMF.h"
#include "VSensorMemory.h"
#include "VGearWifi.h"
#include "VGearWebApi.h"
#include "VGearSound.h"

VBuffer        graph[SENSOR_COUNT];
VMind          mind;
VLang          lang;
VHtml          html;
VSensorAir     air;
VSensorLight   light;
VSensorGPS     gps; 
VSensorEMF     emf;
VSensorMemory  mem;
VGearWifi      net;
VGearWebApi    api;
VGearSound     sound;

void update(int field)
{
  switch (field) 
  {
    case TEMPERATURE:
      graph[TEMPERATURE].push(air.getTemperature());
      mind.analyse(TEMPERATURE, air.getTemperatureStatus(), air.getTemperatureTolerance(), graph[TEMPERATURE].dump());
      break;
    case PRESSURE:
      graph[PRESSURE].push(air.getPressure());
      mind.analyse(PRESSURE, air.getPressureStatus(), air.getPressureTolerance(), graph[PRESSURE].dump());
      break;
    case HUMIDITY:
      graph[HUMIDITY].push(air.getHumidity());
      mind.analyse(HUMIDITY, air.getHumidityStatus(), air.getHumidityTolerance(), graph[HUMIDITY].dump());
      break;
    case AIR_QUALITY:
      graph[AIR_QUALITY].push(air.getAirQuality());
      mind.analyse(AIR_QUALITY, air.getAirQualityStatus(), air.getAirQualityTolerance(), graph[AIR_QUALITY].dump());
      break;
    case CO2_EQUIVALENT:
      graph[CO2_EQUIVALENT].push(air.getCo2Equivalent());
      mind.analyse(CO2_EQUIVALENT, 0, air.getCo2EquivalentTolerance(), graph[CO2_EQUIVALENT].dump());
      break;
    case VOC_EQUIVALENT:
      graph[VOC_EQUIVALENT].push(air.getVocEquivalent());
      mind.analyse(VOC_EQUIVALENT, 0, air.getVocEquivalentTolerance(), graph[VOC_EQUIVALENT].dump());
      break;
    case UV_INDEX:
      graph[UV_INDEX].push(light.getUVIndex());
      mind.analyse(UV_INDEX, light.getUVIndexStatus(), light.getUVIndexTolerance(), graph[UV_INDEX].dump());
      break;
    case VISIBLE:
      graph[VISIBLE].push(light.getVisible());
      mind.analyse(VISIBLE, 0, light.getVisibleTolerance(), graph[VISIBLE].dump());
      break;
    case INFRARED:
      graph[INFRARED].push(light.getInfraRed());
      mind.analyse(INFRARED, 0, light.getInfraRedTolerance(), graph[INFRARED].dump());
      break;
    case EMF:
      graph[EMF].push(emf.getGauss());
      mind.analyse(EMF, 0, emf.getGaussTolerance(), graph[EMF].dump());
      break;    
    case MEMORY:
      graph[MEMORY].push(mem.getMemoryUsage());
      mind.analyse(MEMORY, 0, mem.getMemoryUsageTolerance(), graph[MEMORY].dump());
      break;
  } 

  if (field == AIR_QUALITY) {
    mind_graph_data info = mind.info(field);
    if (info.alert != 0) {
      sound.beep();
    } 
  } 
}

String handleAlert()
{
  String out = "";

  for (int field = 0; field < SENSOR_COUNT; field++) {
    mind_graph_data info = mind.info(field);
    if (info.alert != 0) {
      out += html.handleNotification(lang.get(field) + ": " + String(info.value) + lang.unit(field) + " " + info.comment);
    }
  }

  return out;  
}

void setup()
{
  Serial.begin(115200);
  
  Wire.begin(21, 22);
  air.begin(0x76);
  light.begin(0x60);
  gps.begin(0x10);
  
  emf.begin(36);
  mem.begin();
  sound.begin(4);
  
  // network
  net.begin();
  sound.open();
  
  // web server
  for (int field = 0; field < SENSOR_COUNT; field++) {
    graph[field] = VBuffer();
    api.onSvg("/graph/" + String(field) + ".svg", [field](){ return html.handleSvgGraph(field, graph[field].dump(), mind.info(field)); });
    api.onCommand("/graph/" + String(field) + "/reset/{}", [field](int arg){ graph[field].reset(arg); });
  }
  api.onSvg("/graph/emf.svg", [](){ float* buffer = emf.snap(); return html.handleSvgGraph("EMF", buffer, VBuffer::stat(buffer, 0, 100)); });
  api.onXhr("/script/map.js", []() { return html.handleOsmPoint(gps.getLatitude(), gps.getLongitude(), gps.getDirectionAngle()); });
  api.onXhr("/gps", []() { return html.handleGpsInfo(gps.getSatellites(), gps.getFixQuality(), gps.getAltitude(), gps.getSpeed()); });
  api.onXhr("/alert", []() { return handleAlert(); });
  api.onPage("/", [](){ return html.handleMeteoHome(3000); });
  api.begin();
}

void loop()
{
  api.update(10);
  
  //gps.update(1000);

  if (mem.update(1000)) {
    update(MEMORY);
  }

  if (light.update(1000)) {
    update(UV_INDEX);
    update(VISIBLE);
    update(INFRARED);
  }
  
  if (air.update(3000)) {
    update(TEMPERATURE);
    update(PRESSURE);
    update(HUMIDITY);
    update(AIR_QUALITY);
    update(CO2_EQUIVALENT);
    update(VOC_EQUIVALENT);
  }

  if (!Wire.busy()) {
    if (emf.update(6000)) {
      update(EMF);
    }
  }
  
 
}