#include "VComWifi.h"
#include "VComWebApi.h"
#include "VSensorGPS.h"

VComWifi net;
VComWebApi api;
VSensorGPS gps; 

// Affiche la map
String handleOsmHome() 
{
  String output = "\
<h1><span class='material-icons'>room</span> ESP32-GEO</h1>\
<div class='action'>\
  <div class='button' onclick='go(48.839197, 2.408810)'>Test</div>\
</div>\
<div id='map' class='double bordered'></div>\
<div id='info' class='simple bordered padded'></div>\
<script src='https://unpkg.com/leaflet@1.7.1/dist/leaflet.js'></script>\
<link rel='stylesheet' href='https://unpkg.com/leaflet@1.7.1/dist/leaflet.css' />\
<script>\
  var map = L.map('map').setView([0, 0], 19);\
  L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {attribution:'© OpenStreetMap', maxZoom:19}).addTo(map);\
  L.control.scale({imperial:false, metric:true}).addTo(map);\
  function go(lat, lon) {\
    map.panTo([lat, lon]);\
    L.circle([lat, lon], 0.1).addTo(map);\
  }\
  function refresh() {\
    exec('/map');\
    call('/info', 'info');\
  }\
  window.setInterval(refresh, 10000);\
</script>";

  return output;
}

// Met a jour la map
String handleOsmPoint() 
{
  String output = "\
go(" + String(gps.getLatitude(), 7) + ", " + String(gps.getLongitude(), 7) + ");";

  return output;
}

// Affiche les infos
String handleGpsInfo() 
{
  String output = "Sat:" + String(gps.getSatellites()) + "  " + gps.getFixQuality() + "<br>"
    + gps.getDateTime() + "<br>"
    + String(gps.getLatitude(), 7) + " " + String(gps.getLatCardinal()) + "<br>"
    + String(gps.getLongitude(), 7) + " " + String(gps.getLongCardinal()) + "<br>"
    + "Altitude:  " + String(gps.getAltitude(), 2) + " m<br>"
    + "Compass:   " + String(gps.getCompassAngle(), 2) + " deg " + String(gps.getComCardinal()) + "<br>"
    + "Speed:     " + String(gps.getSpeed(), 2) + " km/h<br>"
    + "Direction: " + String(gps.getDirectionAngle(), 2) + " deg<br>";

  return output;
}

void setup() 
{
  // global
  Serial.begin(115200);
  Wire.begin(21, 22);

  gps.begin(0x10);

  // wifi network
  net.begin();

  // web server
  api.onPage("/", []() { return handleOsmHome(); });
  api.onXhr("/map", []() { return handleOsmPoint(); });
  api.onXhr("/info", []() { return handleGpsInfo(); });
  api.begin();
}

void loop() 
{
  api.update(10);
  gps.update(1000);
}
