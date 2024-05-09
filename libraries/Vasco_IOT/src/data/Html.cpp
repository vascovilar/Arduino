#include "Html.h"

// Affiche la home
String Html::handleHomePage(int delay)
{
  String bloc = _getHtmlBlocEnvironment(
    COLOR_GREY,
    delay
  );
  return _getPageWrapper(
    bloc,
    COLOR_BLACK,
    COLOR_GREY
  );
}

// affiche un graphe svg de suivi
String Html::handleHistorySvgGraph(vfield data, Buffer buffer)
{
  String text, grid, graph, trend;

  float top = buffer.maximum + data.tolerance / 2;
  float bottom = buffer.minimum - data.tolerance / 2;
  float totalTime = VHISTORY_MAX_SIZE * VHISTORY_PUSH_DELAY / 1000.0;

  // upper left: title and text
  text += _getHtmlSvgCircle(25, 20, data.status);
  text += _getHtmlSvgText(40, 25, 14, COLOR_WHITE, data.label);
  text += _getHtmlSvgText(40, 43, 12, COLOR_GREY, data.text + " +/-" + String(buffer.delta) + " (" + String(data.tolerance) + ")");

  // upper right: value and unit
  if (data.status != GRIS) {
    text += _getHtmlSvgBig(50, data.value);
    text += _getHtmlSvgText(390, 23, 14, COLOR_GREY_DARK, data.unit);
    if (buffer.trend == 2) trend += "▼";
    if (buffer.trend == 1) trend += "▲";
    text += _getHtmlSvgText(390, 45, 18, COLOR_GREY_DARK, trend);
  }

  if (buffer.length > 0) {
    // iterate values to draw graph
    for (int i = 0; i < buffer.length; i++)
    {
      int relativeTime = buffer.timeline[0] - buffer.timeline[i];
      // do not show more than total time window
      if (relativeTime > totalTime) {
        break;
      }

      // calculate coords
      float x1 = 410 - 360 * relativeTime / totalTime;
      if (i != 0) {
        float x2 = 410 - 360 * (buffer.timeline[0] - buffer.timeline[i - 1]) / totalTime;
        float y1 = _isometric(buffer.history[i], top, bottom, 100, 60);
        float y2 = _isometric(buffer.history[i - 1], top, bottom, 100, 60);

        // draw all values
        graph += _getHtmlSvgLine(x1, y1, x2, y2, COLOR_WHITE);
      }

      // draw vertical lines : timeline in minuts (first value is bold line)
      if (i % 20 == 0) {
        grid += _getHtmlSvgLine(x1 , 60, x1, 165, COLOR_GREY_DARK, i == 0 ? 1.0: 0.5);
        text += _getHtmlSvgText(x1 - 3, 173, 8, COLOR_GREY, String(relativeTime / 60));
      }
    }

    // draw right arrow (current real time value)
    float y3 = _isometric(data.value, top, bottom, 100, 60);
    text += _getHtmlSvgArrow(410, y3, COLOR_GREY);

    // draw horizontal lines : values max, min, average
    float limits[3] = {buffer.maximum, buffer.minimum, buffer.average};
    for (int i = 0; i < 3; i++) {
      float y = _isometric(limits[i], top, bottom, 100, 60);
      bool isAverageLine = limits[i] == buffer.average;
      grid += _getHtmlSvgLine(50, y, 410, y, COLOR_GREY_DARK, isAverageLine ? 1: 0.5);
      text += _getHtmlSvgText(15, y + 3, 10, isAverageLine ? COLOR_GREY: COLOR_GREY_DARK, String(limits[i]));
    }
  }

  return _getHtmlBlocSvgCartouche(
    text,
    grid,
    graph
  );
}

// affiche la page html content le header avec CSS et JS
String Html::_getPageWrapper(String content, vcolor backgroundColor, vcolor fontColor)
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
      body{padding:10px;background:" + _getHtmlColor(backgroundColor) + ";font-family:Arial,Helvetica,Sans-Serif;font-size:12px;color:" + _getHtmlColor(fontColor) + ";}\
      h1{font-family:'Bebas Neue';font-size:42px;margin:0px;padding-right:20px;color:#333333;}\
      h1 .material-symbols-outlined{font-size:40px;}\
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

// affiche le bloc de divs a mettre a jour par ajax
String Html::_getHtmlBlocEnvironment(vcolor titleColor, int reloadDelay)
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
  <span style='color:" + _getHtmlColor(titleColor) + "'> \
    <span class='material-symbols-outlined'>guardian</span> ESP32 \
  </span> environnement \
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
    var seq = [" + js + "];\
    for (var i in seq) {\
      call('/sensor/' + seq[i] + '.svg', 'div_' + seq[i]);\
    }\
    call('/logger', 'div_logger');\
  }\
  window.setInterval(refresh, " + String(reloadDelay) + ");\
</script>";
}

// affiche le bloc graphique
String Html::_getHtmlBlocSvgCartouche(String text, String grid, String graph)
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
      <stop offset='100%' stop-color='#999999' />\
    </linearGradient>\
  </defs>\
  <rect width='440' height='190' fill='url(#gradient)' rx='20' ry='20' />\
  <g>" + grid + "</g>\
  <g>" + graph + "</g>\
  " + text + "\
</svg>";
}

// Affiche un tableau de données
String Html::handleDataTable(vfield* sensors, int length)
{
  String output = "<tr><th>Statut</th><th>Titre</th><th>Valeur</th><th>Commentaire</th>";

  for (int i = 0; i < length; i++) {
    output += "<tr><td><b style='color:" + _getHtmlColor(sensors[i].status) + "'>█</b></td><td>" + sensors[i].label +"</td><td>" + sensors[i].value + sensors[i].unit + "</td><td>" + sensors[i].text + "</td></tr>";
  }

  return _getPageWrapper("<table>" + output + "</table>", COLOR_BLACK, COLOR_GREY);
}

// Met a jour la map
String Html::handleOsmPoint(float latitude, float longitude, float angle) {
  return "go(" + String(latitude, 7) + ", " + String(longitude, 7) + ", " + String(360 - angle, 0) + ");";
}

// Affiche les infos GPS
String Html::handleGpsInfo(int satellites, String quality, float altitude, float speed) {
  return "Sat:<b style='font-size:16px; color:" + _getHtmlColor(COLOR_GREY) + "'>" + String(satellites) + " " + quality + "</b> | Altitude: " + String(altitude, 2) + " m | Speed: " + String(speed, 2) + " km/h";
}

String Html::handleNotification(String text) {
  return "<span class='material-symbols-outlined' style='font-size:12px; color:red;'>warning</span>&nbsp;" + text + "<br>";
}

String Html::_getHtmlColor(vcolor code)
{
  String value = String(code, HEX);
  for (int i = value.length() + 1; i <= 6; i++) { // count without extra character for the null terminator
    value = "0" + value;
  }

  return "#" + value;
}

String Html::_getHtmlColor(vstatus code)
{
  switch (code) {
    case GRIS:
      return _getHtmlColor(COLOR_GREY_DARK);
    case VERT:
      return _getHtmlColor(COLOR_GREEN);
    case JAUNE:
      return _getHtmlColor(COLOR_YELLOW);
    case ORANGE:
      return _getHtmlColor(COLOR_ORANGE);
    case ROUGE:
      return _getHtmlColor(COLOR_RED);
    case VIOLET:
      return _getHtmlColor(COLOR_VIOLET);
  }

  return "";
}

String Html::_getHtmlSvgLine(float x1, float y1, float x2, float y2, vcolor color, float size)
{
  return "<line x1='" + String(x1) + "' y1='" + String(y1) + "' x2='" + String(x2) + "' y2='" + String(y2) + "' stroke='" + _getHtmlColor(color) + "' stroke-width='" + String(size) + "' />";
}

String Html::_getHtmlSvgText(float x, float y, int size, vcolor color, String text)
{
  return "<text x='" + String(x) + "' y='" + String(y) + "' font-size='" + String(size) + "px' fill='" + _getHtmlColor(color) + "'>" + text + "</text>";
}

String Html::_getHtmlSvgBig(int offset, float value)
{
  return "<text x='" + String((810 - offset) - (String(value).length()) * 24) + "' y='44' font-size='48px' font-weight='bold' fill='url(#shaded)' letter-spacing='-3' transform='scale(0.5, 1)'>" + String(value) + "</text>";
}

String Html::_getHtmlSvgRect(float x, float y, int w, int h, vcolor color)
{
  return "<rect x='" + String(x) + "' y='" + String(y) + "' width='" + String(w) + "' height='" + String(h) + "' fill='" + _getHtmlColor(color) + "' />";;
}

String Html::_getHtmlSvgArrow(float x, float y, vcolor color)
{
  return "<polygon points='" + String(x + 3) + " " + String(y) + ", " + String(x + 13) + " " + String(y + 5) + ", " + String(x + 13) + " " + String(y - 5) + "' fill='" + _getHtmlColor(color) + "' />";
}

String Html::_getHtmlSvgCircle(float x, float y, vstatus color)
{
  return "<circle cx='" + String(x) + "' cy='" + String(y) +"' r='8' fill='" + _getHtmlColor(color) + "' />";
}

String Html::_getHtmlLink(String href, String text)
{
  return "<a href='" + href + "'>" + text + "</a>";
}

float Html::_isometric(float value, float maximum, float minimum, int height, int offset)
{
  return (1 - ((value - minimum) / (float)(maximum - minimum))) * height + offset;
}
