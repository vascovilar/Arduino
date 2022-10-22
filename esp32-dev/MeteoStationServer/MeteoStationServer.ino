#include "VMind.h"
#include "VBuffer.h"
#include "VLang.h"
#include "VSensorMeteo.h"
#include "VSensorLight.h"
#include "VSensorGPS.h"
#include "VComWifi.h"
#include "VComWebApi.h"
#include "VGearSound.h"

VMind          mind;
VBuffer        graph[9];
VLang          lang;
VSensorMeteo   meteo;
VSensorLight   light;
VSensorGPS     gps; 
VComWifi       net;
VComWebApi     api;
VGearSound     sound;

// Affiche la home
String handleMeteoHome() 
{
  return "\
<h1><span class='material-icons'>thunderstorm</span> ESP32-METEO</h1>\
<div id='div_0' class='simple bordered'></div>\
<div id='div_1' class='simple bordered'></div>\
<div id='div_2' class='simple bordered'></div>\
<div id='div_3' class='simple bordered'></div>\
<div id='div_4' class='simple bordered'></div>\
<div id='div_5' class='simple bordered'></div>\
<div id='map' class='double bordered'></div>\
<div class='simple bordered'>\
  <div id='gps' class='padded'>Loading ...</div>\
  <div id='meteo' class='padded'></div>\
</div>\
<div id='div_6' class='simple bordered'></div>\
<div id='div_7' class='simple bordered'></div>\
<div id='div_8' class='simple bordered'></div>\
<script src='https://unpkg.com/leaflet@1.7.1/dist/leaflet.js'></script>\
<script src='https://unpkg.com/leaflet-rotate@0.1.4/dist/leaflet-rotate-src.js'></script>\
<link rel='stylesheet' href='https://unpkg.com/leaflet@1.7.1/dist/leaflet.css' />\
<script>\
  var esri = L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}',{maxZoom:24,attribution:'&copy; Esri'});\
  var osm = L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png',{maxZoom:24,attribution:'&copy; OpenStreetMap'});\
  var map = L.map('map',{center:[0, 0],zoom:18,layers:[osm],rotate:true,bearing:0});\
  L.control.scale({imperial:false, metric:true}).addTo(map);\
  function go(lat, lon, angle) {\
    L.circle([lat, lon], 0.1).addTo(map);\
    map.panTo([lat, lon]);\
    map.setBearing(angle);\
  }\
  function refresh() {\
    var seq = ['0', '1', '2', '3', '4', '5', '6', '7', '8'];\
    for (var i in seq) {\
      call('/graph/' + seq[i] + '.svg', 'div_' + seq[i]);\
    }\
    exec('/map');\
    call('/meteo', 'meteo');\
    call('/gps', 'gps');\
  }\
  refresh();\
  window.setInterval(refresh, 5000);\
</script>";
}

// affiche un graphe svg
String handleSvgGraph(int field, float* data, mind_graph_data info)
{  
  String lines, grid, text;

  for(int i = 0; i < 120 - 1; i++) 
  {
    int x1 = 410 - i * 3;
    int x2 = 410 - (i + 1) * 3;

    if (data[i] != 0 && data[i + 1] != 0) {
      int y1 = _isometric(data[i], info.top, info.bottom, 100, 60);
      int y2 = _isometric(data[i + 1], info.top, info.bottom, 100, 60);
      lines += "<line x1='" + String(x1) + "' y1='" + String(y1) + "' x2='" + String(x2) + "' y2='" + String(y2) + "' />";  
    }
    if (i == 0 && data[i] != 0) {
      int y3 = _isometric(data[i], info.top, info.bottom, 100, 60);
      text += "<polygon points='" + String(x1 + 3) + " " + String(y3) + ", " + String(x1 + 13) + " " + String(y3 + 5) + ", " + String(x1 + 13) + " " + String(y3 - 5) + "' fill='#777777' />";
    }
    if (i % 10 == 0) {
      grid += "<line x1='" + String(x1) + "' y1='60' x2='" + String(x1) + "' y2='165' stroke-width='0.5' />";
      text += "<a href='/graph/" + String(field) + "/slice/" + String(i) + "'><text x='" + String(x1 - 3) + "' y='173' font-size='8px' fill='#999999'>" + String(i) + "</text></a>";
    }
  }
  float limit[3] = {info.maximum, info.minimum, info.average};
  for (int i = 0; i < 3; i++) {
    int pos = _isometric(limit[i], info.top, info.bottom, 100, 60);
    grid += "<line x1='50' y1='" + String(pos) + "' x2='410' y2='" + String(pos) + "' />";
    text += "<text x='20' y='" + String(pos + 3) + "' font-size='10px' fill='#AAAAAA'>" + String(limit[i]) + "</text>";
  }
  
  return "\
<svg xmlns='http://www.w3.org/2000/svg' version='1.1' width='440' height='190'>\
  <style>\
    text { font-family:'Courier'; };\
    line { stroke-width:'1'; };\
  </style>\
  <defs>\
    <radialGradient id='grey' cx='50%' cy='50%' r='50%' fx='50%' fy='50%'>\
      <stop offset='0%' style='stop-color:#000000;stop-opacity:0' />\
      <stop offset='100%' style='stop-color:#151515;stop-opacity:1' />\
    </radialGradient>\
    <radialGradient id='red' cx='50%' cy='50%' r='50%' fx='50%' fy='50%'>\
      <stop offset='0%' style='stop-color:#000000;stop-opacity:0' />\
      <stop offset='100%' style='stop-color:#221515;stop-opacity:1' />\
    </radialGradient>\
    <linearGradient id='shaded' x1='0' x2='0' y1='0' y2='1'>\
      <stop offset='0%' stop-color='#333333' />\
      <stop offset='100%' stop-color='#666666' />\
    </linearGradient>\
  </defs>\
  <rect width='440' height='190' fill='url(" + String(info.background) + ")' rx='20' ry='20' />\
  <circle cx='25' cy='20' r='8' fill='" + info.color + "' />\
  <text x='40' y='25' font-size='14px' fill='white'>" + lang.get(field) + ": " + lang.find(field, info.status) + "</text>\
  <text x='40' y='43' font-size='12px' fill='#999999'>" + info.comment + "</text>\
  <text x='" + String(740 - (String(info.value).length()) * 24) + "' y='44' font-size='48px' font-weight='bold' fill='url(#shaded)' letter-spacing='-3' transform='scale(0.5, 1)'>" + String(info.value) + "</text>\
  <text x='380' y='23' font-size='14px' font-weight='bold' fill='#555555'>" + lang.unit(field) + "</text>\
  <text x='380' y='43' font-size='10px' fill='#999999'>~" + String(info.delta) + "</text>\
  <text x='20' y='63' font-size='10px' fill='white'>" + String(info.top) + "</text>\
  <text x='20' y='163' font-size='10px' fill='white'>" + String(info.bottom) + "</text>\
  " + text + "\
  <g stroke='#777777'>\
    <line x1='50' y1='160' x2='410' y2='160' />\
    <line x1='50' y1='60' x2='40' y2='60' />\
    <line x1='50' y1='160' x2='40' y2='160' />\
    " + grid + "\
  </g>\
  <g stroke='#FFFFFF'>\
    " + lines + "\
  </g>\
</svg>";
}
int _isometric(float value, float maximum, float minimum, int height, int offset)
{
  return int((1 - ((value - minimum) / (maximum - minimum))) * height) + offset;
}

// Met a jour la map
String handleOsmPoint() 
{
  return "\
go(" + String(gps.getLatitude(), 7) + ", " + String(gps.getLongitude(), 7) + ", " + String(360 - gps.getDirectionAngle(), 0) + ");";
}

// Affiche les info meteo
String handleMeteoInfo() 
{
  return "\
<h3>Meteo:</h3>\
" + net.getTime() + "<br>\
Temperature: " + meteo.getTemperature() + " °C<br>\
Pressure:    " + meteo.getPressure() + " hPa<br>\
Humidity:    " + meteo.getHumidity() + " %<br>\
Air quality: " + meteo.getAirQuality() + "<br>\
CO2 equiv.:  " + meteo.getCo2Equivalent() + " ppm<br>\
VOC equiv.:  " + meteo.getBreathVocEquivalent() + " ppm<br>\
UV Index:    " + light.getUVIndex() + "<br>\
Visible:     " + light.getVisible() + " lux<br>\
Infrared:    " + light.getInfraRed() + " lux<br>";
}

// Affiche les infos GPS
String handleGpsInfo() 
{
  return "\
<h3>GPS:</h3>\
Uptime: " + net.getUpTime() + "<br>\
Sat:" + String(gps.getSatellites()) + "  " + gps.getFixQuality() + "<br>\
" + gps.getDateTime() + "<br>\
" + String(gps.getLatitude(), 7) + " " + String(gps.getLatCardinal()) + "<br>\
" + String(gps.getLongitude(), 7) + " " + String(gps.getLongCardinal()) + "<br>\
Altitude:  " + String(gps.getAltitude(), 2) + " m<br>\
Compass:   " + String(gps.getCompassAngle(), 2) + " deg " + String(gps.getComCardinal()) + "<br>\
Speed:     " + String(gps.getSpeed(), 2) + " km/h<br>\
Direction: " + String(gps.getDirectionAngle(), 2) + " deg<br>";
}

void setup()
{
  // global
  Serial.begin(115200);
  Wire.begin(21, 22);
  
  // sensors
  meteo.begin(0x76);
  light.begin(0x60);
  gps.begin(0x10);
  sound.begin(19);

  // network
  net.begin();
  sound.open();
  
  // web server
  api.onPage("/", [](){ return handleMeteoHome(); });
  for (int field = 0; field < 9; field++) {
    graph[field] = VBuffer();
    api.onSvg("/graph/" + String(field) + ".svg", [field](){ return handleSvgGraph(field, graph[field].data, mind.info(field)); });
    api.onCommand("/graph/" + String(field) + "/reset", [field](){ graph[field].reset(); });
    api.onCommand("/graph/" + String(field) + "/slice/{}", [field](int arg){ graph[field].slice(arg); });
  } 
  api.onXhr("/map", []() { return handleOsmPoint(); });
  api.onXhr("/meteo", []() { return handleMeteoInfo(); });
  api.onXhr("/gps", []() { return handleGpsInfo(); });
  api.begin();
}

void loop()
{
  api.update(10);
  gps.update(5000);
  light.update(5000);
  
  if (meteo.update(5000)) {
    Serial.println("update graph sensor");

    if (graph[TEMPERATURE].push(meteo.getTemperature())) {
      mind.analyse(TEMPERATURE, meteo.getTemperatureStatus(), graph[TEMPERATURE].data);
    } 
    if (graph[PRESSURE].push(meteo.getPressure())) {
      mind.analyse(PRESSURE, meteo.getPressureStatus(), graph[PRESSURE].data);
    } 
    if (graph[HUMIDITY].push(meteo.getHumidity())) {
      mind.analyse(HUMIDITY, meteo.getHumidityStatus(), graph[HUMIDITY].data);
    }
    if (graph[AIR_QUALITY].push(meteo.getAirQuality())) {
      mind.analyse(AIR_QUALITY, meteo.getAirStatus(), graph[AIR_QUALITY].data);
    }
    if (graph[CO2_EQUIVALENT].push(meteo.getCo2Equivalent())) {
      mind.analyse(CO2_EQUIVALENT, 0, graph[CO2_EQUIVALENT].data);
    } 
    if (graph[VOC_EQUIVALENT].push(meteo.getBreathVocEquivalent())) {
      mind.analyse(VOC_EQUIVALENT, 0, graph[VOC_EQUIVALENT].data);
    }
    if (graph[UV_INDEX].push(light.getUVIndex())) {
      mind.analyse(UV_INDEX, light.getUvIndexStatus(), graph[UV_INDEX].data);
    }
    if (graph[VISIBLE].push(light.getVisible())) {
      mind.analyse(VISIBLE, 0, graph[VISIBLE].data);
    }
    if (graph[INFRARED].push(light.getInfraRed())) {
      mind.analyse(INFRARED, 0, graph[INFRARED].data);
    }
  }
}
