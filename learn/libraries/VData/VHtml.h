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
      return _getHtmlMeteoHomePage(delay);
    }

    // affiche un graphe svg de suivi
    String handleSvgGraph(int field, float* data, mind_graph_data info)
    {  
      String lines, grid, text, background;
      VLang lang;

      grid += _getHtmlSvgLine(50, 160, 410, 160, 0.5);
      grid += _getHtmlSvgLine(50, 60, 40, 60);
      grid += _getHtmlSvgLine(50, 160, 40, 160);

      text += _getHtmlSvgCircle(25, 20, info.color);
      text += _getHtmlSvgText(40, 25, 14, "white", lang.get(field) + ": " + lang.find(field, info.status));
      text += _getHtmlSvgText(40, 43, 12, "grey", info.comment);
      text += _getHtmlSvgBig(50, info.value);
      text += _getHtmlSvgText(390, 23, 14, "#555555", lang.unit(field));
      text += _getHtmlSvgText(20, 63, 10, "white", String(info.top));
      text += _getHtmlSvgText(20, 163, 10, "white", String(info.bottom));

      if (info.alert == 0) { background = "grey"; };
      if (info.alert == 1) { background = "yellow"; };
      if (info.alert == 2) { background = "red"; };

      for(int i = 0; i < 120 - 1; i++) 
      {
        int x1 = 410 - i * 3;
        int x2 = 410 - (i + 1) * 3;

        if (data[i] != 0 && data[i + 1] != 0) {
          int y1 = _isometric(data[i], info.top, info.bottom, 100, 60);
          int y2 = _isometric(data[i + 1], info.top, info.bottom, 100, 60);
          lines += _getHtmlSvgLine(x1, y1, x2, y2);
        }
        if (i == 0 && data[i] != 0) {
          int y3 = _isometric(data[i], info.top, info.bottom, 100, 60);
          text += _getHtmlSvgArrow(x1, y3, "grey");
        }
        if (i % 10 == 0) {
          grid += _getHtmlSvgLine(x1, 60, x1, 165, 0.5);
          text += _getHtmlLink("/graph/" + String(field) + "/reset/" + String(i), _getHtmlSvgText(x1 - 3, 173, 8, "grey", String(i)));
        }
      }

      float limit[3] = {info.maximum, info.minimum, info.average};
      for (int i = 0; i < 3; i++) {
        int pos = _isometric(limit[i], info.top, info.bottom, 100, 60);
        grid += _getHtmlSvgLine(50, pos, 410, pos);
        text += _getHtmlSvgText(20, pos + 3, 10, "grey", String(limit[i]));
      }
      
      return _getHtmlSvgCartouche(
        background,
        text,
        grid,
        lines
      );
    }

    // affiche un graphe svg oscilloscope
    String handleSvgGraph(String title, float* data, buffer_data_stat info)
    {  
      String lines, grid, text, meter;

      grid += _getHtmlSvgLine(50, 160, 410, 160);
      grid += _getHtmlSvgLine(50, 60, 40, 60);
      grid += _getHtmlSvgLine(50, 160, 40, 160);

      text += _getHtmlSvgText(20, 25, 14, "white", title + " oscilloscope");
      text += _getHtmlSvgBig(0, info.maximum);
      text += _getHtmlSvgText(20, 63, 10, "grey", String(info.maximum));
      text += _getHtmlSvgText(20, 163, 10, "grey", String(info.minimum));
      
      if (info.maximum > 100) { text += _getHtmlSvgRect(20, 35, 30, 5, "blue"); };
      if (info.maximum > 1000) { text += _getHtmlSvgRect(55, 35, 30, 5, "green"); };
      if (info.maximum > 10000) { text += _getHtmlSvgRect(90, 35, 30, 5, "yellow"); };
      if (info.maximum > 30000) { text += _getHtmlSvgRect(125, 35, 30, 5, "orange"); };
      if (info.maximum > 50000) { text += _getHtmlSvgRect(160, 35, 30, 5, "red"); };

      for(int i = 0; i <= 100; i++) 
      {
        int x1 = 410 - i * 3;
        int x2 = 410 - (i + 1) * 3;

        if (i != 100) {
          int y1 = _isometric(data[i], info.maximum, info.minimum, 100, 60);
          int y2 = _isometric(data[i + 1], info.maximum, info.minimum, 100, 60);
          lines += _getHtmlSvgLine(x1, y1, x2, y2); 
        }

        if (i % 10 == 0) {
          grid += _getHtmlSvgLine(x1, 60, x1, 165, 0.5);
          text += _getHtmlSvgText(x1 - 3, 173, 8, "grey", String(i));
        }
      }

      int pos = _isometric(info.average, info.maximum, info.minimum, 100, 60);
      grid += _getHtmlSvgLine(50, pos, 410, pos);
      text += _getHtmlSvgText(20, pos + 3, 10, "white", String(info.average));

      return _getHtmlSvgCartouche(
        "",
        text,
        grid,
        lines
      );
    }

    // Met a jour la map
    String handleOsmPoint(float latitude, float longitude, float angle) {
      return "go(" + String(latitude, 7) + ", " + String(longitude, 7) + ", " + String(360 - angle, 0) + ");";
    }

    // Affiche les infos GPS
    String handleGpsInfo(int satellites, String quality, float altitude, float speed) {
      return "Sat:<b style='font-size:16px; color:white'>" + String(satellites) + " " + quality + "</b> | Altitude: " + String(altitude, 2) + " m | Speed: " + String(speed, 2) + " km/h";
    }

    String handleNotification(String text) {
      return "<span class='material-icons' style='font-size:12px; color:red;'>warning</span>&nbsp;" + text + "<br>";
    }

  private:

    // necessaire pour la précision
    int _isometric(float value, float maximum, float minimum, int height, int offset) {
      return int((1 - ((value - minimum) / (maximum - minimum))) * height) + offset;
    }
    
    String _getHtmlSvgLine(int x1, int y1, int x2, int y2, float size = 1.0) {
      return "<line x1='" + String(x1) + "' y1='" + String(y1) + "' x2='" + String(x2) + "' y2='" + String(y2) + "' stroke-width='" + String(size) + "' />"; 
    }

    String _getHtmlSvgText(int x, int y, int size, String color, String text) {
      return "<text x='" + String(x) + "' y='" + String(y) + "' font-size='" + String(size) + "px' fill='" + String(color) + "'>" + text + "</text>";
    }

    String _getHtmlSvgBig(int offset, float value) {
      return "<text x='" + String((810 - offset) - (String(value).length()) * 24) + "' y='44' font-size='48px' font-weight='bold' fill='url(#shaded)' letter-spacing='-3' transform='scale(0.5, 1)'>" + String(value) + "</text>";
    }
    
    String _getHtmlSvgRect(int x, int y, int w, int h, String color) {
      return "<rect x='" + String(x) + "' y='" + String(y) + "' width='" + String(w) + "' height='" + String(h) + "' fill='" + color + "' />";;
    }

    String _getHtmlSvgArrow(int x, int y, String color) {
      return "<polygon points='" + String(x + 3) + " " + String(y) + ", " + String(x + 13) + " " + String(y + 5) + ", " + String(x + 13) + " " + String(y - 5) + "' fill='" + color + "' />";
    }

    String _getHtmlSvgCircle(int x, int y, String color) {
      return "<circle cx='" + String(x) + "' cy='" + String(y) +"' r='8' fill='" + color + "' />";
    }
    
    String _getHtmlLink(String href, String text) {
      return "<a href='" + href + "'>" + text + "</a>";
    }

    String _getHtmlMeteoHomePage(int delay)
    {
      return "\
<h1><span class='material-icons'>thunderstorm</span> ESP32-METEO</h1>\
<div id='alert' style='float:left; width:100%'></div>\
<div id='main'></div>\
<div id='div_0' class='simple bordered'></div>\
<div id='div_1' class='simple bordered'></div>\
<div id='div_2' class='simple bordered'></div>\
<div id='div_3' class='simple bordered'></div>\
<div id='div_4' class='simple bordered'></div>\
<div id='div_5' class='simple bordered'></div>\
<div class='double bordered'>\
  <div id='map' style='width:440px; height:365px;'></div>\
  <div id='gps' style='width:440px; text-align:center;'></div>\
</div>\
<div id='div_9' class='simple bordered'></div>\
<div id='emf' class='simple bordered'></div>\
<div id='div_7' class='simple bordered'></div>\
<div id='div_8' class='simple bordered'></div>\
<div id='div_6' class='simple bordered'></div>\
<div id='div_10' class='simple bordered'></div>\
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
  var box = ['/graph/0.svg', '/graph/1.svg'];\
  function id(path) {\
    var id = path.replaceAll('/', '_');\
    return id.replaceAll('.', '_');\
  }\
  function init() {\
    for (var i in box) {\
      var div = document.createElement('div');\
      div.id = id(box[i]);\
      getElementById('main').appendChild(div);\
      call(seq[i], id(box[i]));\
    }\
    refresh();\
  }\
  function refresh() {\
    var seq = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10'];\
    for (var i in seq) {\
      call('/graph/' + seq[i] + '.svg', 'div_' + seq[i]);\
    }\
    exec('/script/map.js');\
    call('/gps', 'gps');\
    call('/graph/emf.svg', 'emf');\
    call('/alert', 'alert');\
  }\
  init();\
  window.setInterval(refresh, " + String(delay) + ");\
</script>";
    }

    String _getHtmlSvgCartouche(String background, String text, String grid, String lines)
    {
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
  <rect width='440' height='190' fill='url(#" + background + ")' rx='20' ry='20' />\
  " + text + "\
  <g stroke='#777777'>\
    " + grid + "\
  </g>\
  <g stroke='#FFFFFF'>\
    " + lines + "\
  </g>\
</svg>";
    }
};

#endif