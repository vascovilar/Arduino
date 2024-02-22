#ifndef VDataHtml_h
#define VDataHtml_h

#include "Arduino.h"
#include "VConfig.h"
#include "VDataBuffer.h"

class VDataHtml
{
  public:

    String handleHomePage(int delay);
    String handleHistorySvgGraph(VDataBuffer buffer, field_data data);
    //String handleSvgGraph(String title, float* data, buffer_data_stat info):
    String handleOsmPoint(float latitude, float longitude, float angle);
    String handleGpsInfo(int satellites, String quality, float altitude, float speed);
    String handleNotification(String text);

  private:
    
    // necessaire pour la précision
    int _isometric(float value, float maximum, float minimum, int height, int offset);
    
    String _getHtmlColor(int code);
    String _getHtmlSvgLine(int x1, int y1, int x2, int y2, float size = 1.0);
    String _getHtmlSvgText(int x, int y, int size, String color, String text);
    String _getHtmlSvgBig(int offset, float value);
    String _getHtmlSvgRect(int x, int y, int w, int h, String color);
    String _getHtmlSvgArrow(int x, int y, String color);
    String _getHtmlSvgCircle(int x, int y, String color); 
    String _getHtmlLink(String href, String text);
     
    String _getHtmlWrapper(String content)
    {
      return "\
<!DOCTYPE html>\
  <head>\
    <meta charset='UTF-8'>\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
    <link rel='preconnect' href='https://fonts.googleapis.com/'>\
    <link rel='preconnect' href='https://fonts.gstatic.com/' crossorigin=''>\
    <link href='https://fonts.googleapis.com/css2?family=Bebas+Neue&display=swap' rel='stylesheet'>\
    <link href='https://fonts.googleapis.com/icon?family=Material+Icons' rel='stylesheet'>\
    <style>\
      body{padding:10px;background:#000000;font-family:Arial,Helvetica,Sans-Serif;font-size:12px;color:#999999;}\
      h1{font-family:'Bebas Neue';font-size:42px;margin:0px;color:#555555;}\
      h1 .material-icons{font-size:40px;}\
      h2{font-size:18px;margin-bottom: 0px;}\
      h2,h3{color:#DDDDDD;font-weight:bold;}\
      h3{font-family:'Courier';font-size:14px;font-weight:normal;margin:2px;}\
      p{margin-top:0px;}\
      a:link,a:visited,a:hover,a:focus,a:active{color:#366899;text-decoration:none;}\
      a:hover{color:#4aa3fc;}\
      div{float:left;}\
      .bordered{border:1px solid #444444;border-radius:20px;margin:5px;overflow:hidden;}\
      .padded{padding:20px; padding-top:15px;}\
      .simple{width:440px; height:190px;}\
      .double{width:440px; height:390px;}\
      .tile{margin:3px;padding:5px;border:1px solid #000000;border-radius:10px;background:#000000;text-align:center;}\
      .tile:hover{border-color:#366899;cursor:pointer;}\
      .button{background:#366899;color:#000000;font-size:14px;padding:10px 20px 10px 20px;margin-top:10px;border-radius:20px;font-weight:bold;}\
      .button:hover{background:#4aa3fc;cursor:pointer;}\
      .action{float:right;height:60px;}\
      .action .material-icons{color:#366899;font-size:32px;padding:10px;}\
    </style>\
    <script>\
      function call(uri, div) {\
        var xhr = new XMLHttpRequest();\
        xhr.open('GET', uri);\
        xhr.send();\
        xhr.onload = function() {\
          if (xhr.status == 200 && div) {\
            document.getElementById(div).innerHTML = xhr.response;\
          }\
        };\
      }\
      function exec(uri) {\
        var xhr = new XMLHttpRequest();\
        xhr.open('GET', uri);\
        xhr.send();\
        xhr.onload = function() {\
          if (xhr.status == 200) {\
            eval(xhr.response);\
          }\
        };\
      }\
      function reload(obj) {\
        obj.style.display = 'none';\
        setTimeout(function(){location.reload();}, 1000);\
      }\
    </script>\
  </head>\
  <body>\
    " + content + "\
  </body>\
</html>";
    }

    String _getHtmlGlobalEnvironmentPage(int delay)
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
<div id='div_6' class='simple bordered'></div>\
<div id='div_7' class='simple bordered'></div>\
<div id='div_8' class='simple bordered'></div>\
<script>\
  function refresh() {\
    var seq = ['0', '1', '2', '3', '4', '5', '6', '7', '8'];\
    for (var i in seq) {\
      call('/graph/' + seq[i] + '.svg', 'div_' + seq[i]);\
    }\
  }\
  window.setInterval(refresh, " + String(delay) + ");\
</script>";
    }

    /*String _getHtmlMeteoHomePage(int delay)
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
    }*/

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
      <stop offset='0%' style='stop-color:#000000;stop-opacity:1' />\
      <stop offset='100%' style='stop-color:#151515;stop-opacity:1' />\
    </radialGradient>\
    <radialGradient id='yellow' cx='50%' cy='50%' r='50%' fx='50%' fy='50%'>\
      <stop offset='0%' style='stop-color:#000000;stop-opacity:1' />\
      <stop offset='100%' style='stop-color:#222215;stop-opacity:1' />\
    </radialGradient>\
    <radialGradient id='red' cx='50%' cy='50%' r='50%' fx='50%' fy='50%'>\
      <stop offset='0%' style='stop-color:#000000;stop-opacity:1' />\
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