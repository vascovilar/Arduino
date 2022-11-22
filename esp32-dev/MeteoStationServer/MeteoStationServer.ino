#include "VMind.h"
#include "VBuffer.h"
#include "VLang.h"
#include "VHtml.h"
#include "VSensorAir.h"
#include "VSensorLight.h"
#include "VSensorGPS.h"
#include "VSensorEMF.h"
#include "VGearWifi.h"
#include "VGearWebApi.h"
#include "VGearSound.h"

VBuffer        graph[10];
VMind          mind;
VLang          lang;
VHtml          html;
VSensorAir     air;
VSensorLight   light;
VSensorGPS     gps; 
VSensorEMF     emf;
VGearWifi      net;
VGearWebApi    api;
VGearSound     sound;


void update(int field)
{
  switch (field) 
  {
    case TEMPERATURE:
      graph[TEMPERATURE].push(air.getTemperature());
      mind.analyse(TEMPERATURE, air.getTemperatureStatus(), graph[TEMPERATURE].dump());
      break;
    case PRESSURE:
      graph[PRESSURE].push(air.getPressure());
      mind.analyse(PRESSURE, air.getPressureStatus(), graph[PRESSURE].dump());
      break;
    case HUMIDITY:
      graph[HUMIDITY].push(air.getHumidity());
      mind.analyse(HUMIDITY, air.getHumidityStatus(), graph[HUMIDITY].dump());
      break;
    case AIR_QUALITY:
      graph[AIR_QUALITY].push(air.getAirQuality());
      mind.analyse(AIR_QUALITY, air.getAirStatus(), graph[AIR_QUALITY].dump());
      break;
    case CO2_EQUIVALENT:
      graph[CO2_EQUIVALENT].push(air.getCo2Equivalent());
      mind.analyse(CO2_EQUIVALENT, 0, graph[CO2_EQUIVALENT].dump());
      break;
    case VOC_EQUIVALENT:
      graph[VOC_EQUIVALENT].push(air.getBreathVocEquivalent());
      mind.analyse(VOC_EQUIVALENT, 0, graph[VOC_EQUIVALENT].dump());
      break;
    case UV_INDEX:
      graph[UV_INDEX].push(light.getUVIndex());
      mind.analyse(UV_INDEX, light.getUvIndexStatus(), graph[UV_INDEX].dump());
      break;
    case VISIBLE:
      graph[VISIBLE].push(light.getVisible());
      mind.analyse(VISIBLE, 0, graph[VISIBLE].dump());
      break;
    case INFRARED:
      graph[INFRARED].push(light.getInfraRed());
      mind.analyse(INFRARED, 0, graph[INFRARED].dump());
      break;
    case EMF:
      graph[EMF].push(emf.getGauss());
      mind.analyse(EMF, 0, graph[EMF].dump());
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

  for (int field = 0; field < 10; field++) {
    mind_graph_data info = mind.info(field);
    if (info.alert != 0) {
      out += lang.get(field) + ": " + String(info.value) + lang.unit(field) + " " + info.comment + "<br>";
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
  sound.begin(4);
  
  // network
  net.begin();
  sound.open();
  
  // web server
  api.onPage("/", [](){ return html.handleMeteoHome(3000); });
  for (int field = 0; field < 10; field++) {
    graph[field] = VBuffer();
    api.onSvg("/graph/" + String(field) + ".svg", [field](){ return html.handleSvgGraph(field, graph[field].dump(), mind.info(field)); });
    api.onCommand("/graph/" + String(field) + "/reset", [field](){ graph[field].reset(); });
    api.onCommand("/graph/" + String(field) + "/slice/{}", [field](int arg){ graph[field].slice(arg); });
  } 
  api.onXhr("/map", []() { return html.handleOsmPoint(gps.getLatitude(), gps.getLongitude(), gps.getDirectionAngle()); });
  api.onXhr("/gps", []() { return html.handleGpsInfo(gps.getSatellites(), gps.getFixQuality(), gps.getAltitude(), gps.getSpeed()); });
  api.onSvg("/graph/emf.svg", [](){ float* buffer = emf.snap(); return html.handleSvgGraph("EMF oscilloscope", buffer, graph[0].stat(buffer, 0, 100)); });
  api.onXhr("/alert", []() { return handleAlert(); });
  api.begin();
}

void loop()
{
  api.update(10);
  
  //gps.update(1000);
 
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