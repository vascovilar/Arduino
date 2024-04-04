#ifndef VDataHtml_h
#define VDataHtml_h

#include "Arduino.h"
#include "VData.h"
#include "VSensor.h"
#include "VDataBuffer.h"
#include "VDataFilter.h"

class VDataHtml : public VDataFilter
{
  public:

    String    handleHomePage(int delay);
    String    handleHistorySvgGraph(vfield_data data, VDataBuffer buffer);
    String    handleDataTable(vfield_data* sensors, int length);
    String    handleOsmPoint(float latitude, float longitude, float angle);
    String    handleGpsInfo(int satellites, String quality, float altitude, float speed);
    String    handleNotification(String text);

  private:
    
    String    _getHtmlColor(vcolor_code code);
    String    _getHtmlColor(vstatus_code code);
    String    _getHtmlSvgLine(int x1, int y1, int x2, int y2, float size = 1.0);
    String    _getHtmlSvgText(int x, int y, int size, String color, String text);
    String    _getHtmlSvgBig(int offset, float value);
    String    _getHtmlSvgRect(int x, int y, int w, int h, String color);
    String    _getHtmlSvgArrow(int x, int y, String color);
    String    _getHtmlSvgCircle(int x, int y, String color); 
    String    _getHtmlLink(String href, String text);
     
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
    <link href='https://fonts.googleapis.com/icon?family=Material+Symbols+Outlined' rel='stylesheet'>\
    <style>\
      body{padding:10px;background:" + _getHtmlColor(COLOR_BLACK) + ";font-family:Arial,Helvetica,Sans-Serif;font-size:12px;color:" + _getHtmlColor(COLOR_GREY_DARK) + ";}\
      h1{font-family:'Bebas Neue';font-size:42px;margin:0px;padding-right:20px;color:#333333;}\
      h1 .material-symbols-outlined{font-size:40px;color:" + _getHtmlColor(COLOR_GREY) + ";}\
      td{padding-right:10px;}\
      a:link,a:visited,a:hover,a:focus,a:active{color:#366899;text-decoration:none;}\
      a:hover{color:#4aa3fc;}\
      div{float:left;}\
      .bordered{border:1px solid #333333;border-radius:20px;margin:5px;overflow:hidden;}\
      .padded{padding:20px; padding-top:15px;}\
      .simple{width:440px; height:190px;}\
      .double{width:440px; height:390px;}\
      .tile{margin:3px;padding:5px;border:1px solid #000000;border-radius:10px;background:#000000;text-align:center;}\
      .tile:hover{border-color:#366899;cursor:pointer;}\
      .button{background:#366899;color:#000000;font-size:14px;padding:10px 20px 10px 20px;margin-top:10px;border-radius:20px;font-weight:bold;}\
      .button:hover{background:#4aa3fc;cursor:pointer;}\
      .action{float:right;height:60px;}\
      .action .material-symbols-outlined{color:#366899;font-size:32px;padding:10px;}\
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
  <body>" + content + "</body>\
</html>";
    }

    String _getHtmlGlobalEnvironmentPage(int delay)
    {
      String div = "";
      String js = "";
      for (int field = 0; field < VSENSOR_COUNT; field++) {
        div += "<div id='div_" + String(field) + "' class='simple bordered'></div>";
        js += "," + String(field);
      }
      js = js.substring(1);

      return "\
<h1>\
  <span class='material-symbols-outlined'>guardian</span> <span style='color:" + _getHtmlColor(COLOR_GREY) + "'>ESP32</span> environnement \
</h1>\
<div id='div_logger' style='width:100%; padding-left:10px; padding-bottom:10px;'></div>\
</div>\
<div id='main'></div>\
" + div + "\
<div style='width:100%; padding-left:10px;'>\
  <a href='/sensors'>view all sensors raw data</a>\
</div>\
<script>\
  function refresh() {\
    var seq = [ "+ js + "];\
    for (var i in seq) {\
      call('/sensor/' + seq[i] + '.svg', 'div_' + seq[i]);\
    }\
    call('/logger', 'div_logger');\
  }\
  window.setInterval(refresh, " + String(delay) + ");\
</script>";
    }

    String _getHtmlSvgCartouche(String text, String grid, String graph)
    {
      return "\
<svg xmlns='http://www.w3.org/2000/svg' version='1.1' width='440' height='190'>\
  <style>\
    text { font-family:'Courier'; };\
    line { stroke-width:'1'; };\
  </style>\
  <defs>\
    <radialGradient id='gradient' cx='50%' cy='50%' r='50%' fx='50%' fy='50%'>\
      <stop offset='0%' style='stop-color:#000000;stop-opacity:1' />\
      <stop offset='100%' style='stop-color:#151515;stop-opacity:1' />\
    </radialGradient>\
    <linearGradient id='shaded' x1='0' x2='0' y1='0' y2='1'>\
      <stop offset='0%' stop-color='#333333' />\
      <stop offset='100%' stop-color='" + _getHtmlColor(COLOR_GREY) + "' />\
    </linearGradient>\
  </defs>\
  <rect width='440' height='190' fill='url(#gradient)' rx='20' ry='20' />\
  " + text + "\
  <g stroke='" + _getHtmlColor(COLOR_GREY_DARK) + "'>" + grid + "</g>\
  <g stroke='" + _getHtmlColor(COLOR_WHITE) + "'>" + graph + "</g>\
</svg>";
    }

};

#endif