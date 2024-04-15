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
    COLOR_GREY_DARK, 
    COLOR_GREY
  );
}

// affiche un graphe svg de suivi
String Html::handleHistorySvgGraph(vfield data, Buffer buffer)
{
  String text, grid, graph, trend;

  float top = buffer.maximum + data.tolerance / 2;
  float bottom = buffer.minimum - data.tolerance / 2;

  // titre à gauche
  text += _getHtmlSvgCircle(25, 20, _getHtmlColor(data.status));
  text += _getHtmlSvgText(40, 25, 14, _getHtmlColor(COLOR_WHITE), data.label);
  text += _getHtmlSvgText(40, 43, 12, _getHtmlColor(COLOR_GREY), data.text + " +/-" + String(buffer.delta) + " (" + String(data.tolerance) + ")");
  // titre à droite
  if (data.status != GRIS) {
    text += _getHtmlSvgBig(50, data.value);
    text += _getHtmlSvgText(390, 23, 14, _getHtmlColor(COLOR_GREY_DARK), data.unit);
    if (buffer.trend == 2) {
      trend += "▼";
    }
    if (buffer.trend == 1) {
      trend += "▲";
    }
    text += _getHtmlSvgText(390, 45, 18, _getHtmlColor(COLOR_GREY_DARK), trend);
  }

  if (buffer.length > 0) {    
    for (int i = 0; i < buffer.length; i++) 
    {
      int x1 = 410 - i * 1.5;
      int x2 = 410 - (i + 1) * 1.5;

      // graphe
      if (i != 0) {
        int y1 = _isometric(buffer.history[i - 1], top, bottom, 100, 60);
        int y2 = _isometric(buffer.history[i], top, bottom, 100, 60);
        graph += _getHtmlSvgLine(x1, y1, x2, y2);
      } else {
        int y2 = _isometric(buffer.history[i], top, bottom, 100, 60);
        int y3 = _isometric(data.value, top, bottom, 100, 60);
        graph += _getHtmlSvgLine(x1, y2, x2, y2);
        // fleche droite (valeur en cours)
        text += _getHtmlSvgArrow(x1, y3, _getHtmlColor(COLOR_GREY));
      }
      // minutes  // + 1 eof
      if (i % 20 == 0) {
        grid += _getHtmlSvgLine(x1, 60, x1, 165, 0.5);
        text += _getHtmlSvgText(x1 - 3, 173, 8, _getHtmlColor(COLOR_GREY), String((i * buffer.delay) / 1000 / 60));
      }
    }
    
    // lignes horizontales en fct des valeurs max, min, moyenne
    float limit[3] = {buffer.maximum, buffer.minimum, buffer.average};
    for (int i = 0; i < 3; i++) {
      int pos = _isometric(limit[i], top, bottom, 100, 60);
      grid += _getHtmlSvgLine(50, pos, 410, pos, limit[i] != buffer.average ? 0.5: 1);
      text += _getHtmlSvgText(20, pos + 3, 10, _getHtmlColor(COLOR_GREY), String(limit[i]));
    }
  }
  
  return _getHtmlBlocSvgCartouche(
    text,
    COLOR_GREY_DARK,
    grid,
    COLOR_WHITE,
    graph
  );
}

// affiche la page html content le header avec CSS et JS
String Html::_getPageWrapper(String content, vcolor backgroundColor, vcolor fontColor, vcolor titleColor)
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
      h1 .material-symbols-outlined{font-size:40px;color:" + _getHtmlColor(titleColor) + ";}\
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
  <span class='material-symbols-outlined'>guardian</span> <span style='color:" + _getHtmlColor(titleColor) + "'>ESP32</span> environnement \
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
String Html::_getHtmlBlocSvgCartouche(String text, vcolor gridColor, String grid, vcolor graphColor, String graph)
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
  " + text + "\
  <g stroke='" + _getHtmlColor(gridColor) + "'>" + grid + "</g>\
  <g stroke='" + _getHtmlColor(graphColor) + "'>" + graph + "</g>\
</svg>";
}

// Affiche un tableau de données
String Html::handleDataTable(vfield* sensors, int length)
{
  String output = "<tr><th>Statut</th><th>Titre</th><th>Valeur</th><th>Commentaire</th>";

  for (int i = 0; i < length; i++) {
    output += "<tr><td><b style='color:" + _getHtmlColor(sensors[i].status) + "'>█</b></td><td>" + sensors[i].label +"</td><td>" + sensors[i].value + sensors[i].unit + "</td><td>" + sensors[i].text + "</td></tr>";
  }

  return _getPageWrapper("<table>" + output + "</table>", COLOR_BLACK, COLOR_GREY_DARK, COLOR_GREY);
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

String Html::_getHtmlSvgLine(int x1, int y1, int x2, int y2, float size) 
{
  return "<line x1='" + String(x1) + "' y1='" + String(y1) + "' x2='" + String(x2) + "' y2='" + String(y2) + "' stroke-width='" + String(size) + "' />"; 
}

String Html::_getHtmlSvgText(int x, int y, int size, String color, String text) 
{
  return "<text x='" + String(x) + "' y='" + String(y) + "' font-size='" + String(size) + "px' fill='" + color + "'>" + text + "</text>";
}

String Html::_getHtmlSvgBig(int offset, float value) 
{
  return "<text x='" + String((810 - offset) - (String(value).length()) * 24) + "' y='44' font-size='48px' font-weight='bold' fill='url(#shaded)' letter-spacing='-3' transform='scale(0.5, 1)'>" + String(value) + "</text>";
}

String Html::_getHtmlSvgRect(int x, int y, int w, int h, String color) 
{
  return "<rect x='" + String(x) + "' y='" + String(y) + "' width='" + String(w) + "' height='" + String(h) + "' fill='" + color + "' />";;
}

String Html::_getHtmlSvgArrow(int x, int y, String color) 
{
  return "<polygon points='" + String(x + 3) + " " + String(y) + ", " + String(x + 13) + " " + String(y + 5) + ", " + String(x + 13) + " " + String(y - 5) + "' fill='" + color + "' />";
}

String Html::_getHtmlSvgCircle(int x, int y, String color) 
{
  return "<circle cx='" + String(x) + "' cy='" + String(y) +"' r='8' fill='" + color + "' />";
}

String Html::_getHtmlLink(String href, String text) 
{
  return "<a href='" + href + "'>" + text + "</a>";
}

