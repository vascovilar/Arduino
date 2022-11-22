/**
 * 
 * html lib
 * 
 */
 
#ifndef VHtml_h
#define VHtml_h

#include "Arduino.h"
#include "VLang.h"
#include "VMind.h"

class VHtml
{
  public:

    // Affiche la home
    String handleMeteoHome(int delay) 
    {
  return "\
<h1><span class='material-icons'>thunderstorm</span> ESP32-METEO</h1>\
<div id='alert' style='float:right'></div>\
<div id='div_0' class='simple bordered'></div>\
<div id='div_1' class='simple bordered'></div>\
<div id='div_2' class='simple bordered'></div>\
<div id='div_3' class='simple bordered'></div>\
<div id='div_4' class='simple bordered'></div>\
<div id='div_5' class='simple bordered'></div>\
<div class='double bordered'>\
  <div id='map' class='bordered' style='width:425px; height:360px;'></div>\
  <div id='gps' style='width:440px; text-align:center;'></div>\
</div>\
<div id='div_9' class='simple bordered'></div>\
<div id='emf' class='simple bordered'></div>\
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
    var seq = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9'];\
    for (var i in seq) {\
      call('/graph/' + seq[i] + '.svg', 'div_' + seq[i]);\
    }\
    exec('/map');\
    call('/alert', 'alert');\
    call('/gps', 'gps');\
    call('/graph/emf.svg', 'emf');\
  }\
  refresh();\
  window.setInterval(refresh, " + String(delay) + ");\
</script>";
    }

    // affiche un graphe svg
    String handleSvgGraph(int field, float* data, mind_graph_data info)
    {  
      String lines, grid, text, background;
      VLang lang;

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

      background = "#grey";
      if (info.alert == 1) {
        background = "#yellow";
      }
      if (info.alert == 2) {
        background = "#red";
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
    <radialGradient id='yellow' cx='50%' cy='50%' r='50%' fx='50%' fy='50%'>\
      <stop offset='0%' style='stop-color:#000000;stop-opacity:0' />\
      <stop offset='100%' style='stop-color:#222215;stop-opacity:1' />\
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
  <rect width='440' height='190' fill='url(" + background + ")' rx='20' ry='20' />\
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

    // affiche un graphe svg
    String handleSvgGraph(String title, float* data, buffer_data_stat info)
    {  
      String lines, grid, text, meter;

      for(int i = 0; i <= 100; i++) 
      {
        int x1 = 410 - i * 3;
        int x2 = 410 - (i + 1) * 3;

        if (i != 100) {
          int y1 = _isometric(data[i], info.maximum, info.minimum, 100, 60);
          int y2 = _isometric(data[i + 1], info.maximum, info.minimum, 100, 60);
          lines += "<line x1='" + String(x1) + "' y1='" + String(y1) + "' x2='" + String(x2) + "' y2='" + String(y2) + "' />";  
        }

        if (i % 10 == 0) {
          grid += "<line x1='" + String(x1) + "' y1='60' x2='" + String(x1) + "' y2='165' stroke-width='0.5' />";
          text += "<text x='" + String(x1 - 3) + "' y='173' font-size='8px' fill='#999999'>" + String(i) + "</text>";
        }
      }
      int pos = _isometric(info.average, info.maximum, info.minimum, 100, 60);
      grid += "<line x1='50' y1='" + String(pos) + "' x2='410' y2='" + String(pos) + "' />";
      text += "<text x='20' y='" + String(pos + 3) + "' font-size='10px' fill='white'>" + String(info.average) + "</text>";
      
      if (info.maximum > 100) {
        meter += "<rect x='20' y='35' width='30' height='5' fill='blue' />";
      }
      if (info.maximum > 1000) {
        meter += "<rect x='55' y='35' width='30' height='5' fill='green' />";
      }
      if (info.maximum > 10000) {
        meter += "<rect x='90' y='35' width='30' height='5' fill='yellow' />";
      }
      if (info.maximum > 30000) {
        meter += "<rect x='125' y='35' width='30' height='5' fill='orange' />";
      }
      if (info.maximum > 50000) {
        meter += "<rect x='160' y='35' width='30' height='5' fill='red' />";
      }

      return "\
<svg xmlns='http://www.w3.org/2000/svg' version='1.1' width='440' height='190'>\
  <style>\
    text { font-family:'Courier'; };\
    line { stroke-width:'1'; };\
  </style>\
  <defs>\
    <linearGradient id='shaded' x1='0' x2='0' y1='0' y2='1'>\
      <stop offset='0%' stop-color='#333333' />\
      <stop offset='100%' stop-color='#666666' />\
    </linearGradient>\
  </defs>\
  <rect width='440' height='190' rx='20' ry='20' />\
  <text x='20' y='25' font-size='14px' fill='white'>" + title + "</text>\
  " + meter + "\
  <text x='" + String(810 - (String(info.maximum).length()) * 24) + "' y='44' font-size='48px' font-weight='bold' fill='url(#shaded)' letter-spacing='-3' transform='scale(0.5, 1)'>" + String(info.maximum) + "</text>\
  <text x='20' y='63' font-size='10px' fill='#999999'>" + String(info.maximum) + "</text>\
  <text x='20' y='163' font-size='10px' fill='#999999'>" + String(info.minimum) + "</text>\
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

    // Met a jour la map
    String handleOsmPoint(float latitude, float longitude, float angle) 
    {
      return "\
go(" + String(latitude, 7) + ", " + String(longitude, 7) + ", " + String(360 - angle, 0) + ");";
    }

    // Affiche les infos GPS
    String handleGpsInfo(int satellites, String quality, float altitude, float speed) 
    {
      return "\
Sat:" + String(satellites) + "  " + quality + " | \
Altitude: " + String(altitude, 2) + " m | \
Speed: " + String(speed, 2) + " km/h";
    }

  private:

    // necessaire pour la précision
    int _isometric(float value, float maximum, float minimum, int height, int offset)
    {
      return int((1 - ((value - minimum) / (maximum - minimum))) * height) + offset;
    }

};

#endif