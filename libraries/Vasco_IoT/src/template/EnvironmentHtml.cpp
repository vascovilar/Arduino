#include "EnvironmentHtml.h"

// Affiche la home
String EnvironmentHtml::handleHomePage(String title, String subtitle, vsensor* list, int length, int reloadDelay)
{
  String content = "";
  content += _drawTitleBloc(title, subtitle);
  content += _drawEnvironmentGraphs(list, length, reloadDelay);

  return _drawPageWrapper(content);
}

// affiche un graphe svg de suivi (width='430' height='190')
String EnvironmentHtml::handleHistorySvgGraphs(vfield data, Buffer buffer)
{
  float top = buffer.maximum + data.tolerance;
  float bottom = buffer.minimum - data.tolerance;
  float totalTime = VBUFFER_PUSH_DELAY * VBUFFER_MAX_SIZE / 1000.0;

  // upper left: rounded status
  circle(25, 20, 8, _convert(data.status));

  // upper left: title
  if (data.label != "") {
     text(40, 25, data.label, SIZE_TITLE, COLOR_WHITE);
  }

  // upper right: value and unit
  if (data.status != GRIS) {
    text(40, 43, data.text + " ±" + String(buffer.delta) + "(" + String(data.tolerance) + ")", SIZE_TEXT, COLOR_GREY);
    text(50, 0, String(data.value), SIZE_BIG, COLOR_TRANSPARENT);
    text(390, 23, data.unit, SIZE_TEXT, COLOR_GREY);
    String symbol = "";
    if (buffer.trend == 2) symbol += "▼";
    if (buffer.trend == 1) symbol += "▲";
    text(390, 45, symbol, SIZE_TEXT, COLOR_GREY_DARK);
  } else if (data.label != "") {
    text(40, 43, "En attente de données...", SIZE_TEXT, COLOR_GREY);
  }

  // use Screen class function
  drawValueHistory(22, 60, data, buffer, VSCREEN_WIDTH_WEB, VSCREEN_HEIGHT_WEB, COLOR_TRANSPARENT);

  return _drawSvgCartouche(popHtmlBuffer());
}

// Affiche un tableau de données
String EnvironmentHtml::handleDataTables(String title, String subtitle, vdatatable* chipsets, int chipsetLength, vfield* sensors, int sensorLength)
{
  String content = "";
  content += _drawTitleBloc(title, subtitle);
  content += _drawDataTable(chipsets, chipsetLength);
  content += _drawSensorDataTable(sensors, sensorLength);

  return _drawPageWrapper(content);
}

// Met a jour la map
String EnvironmentHtml::handleOsmPoint(float latitude, float longitude, float angle) {
  return "go(" + String(latitude, 7) + ", " + String(longitude, 7) + ", " + String(360 - angle, 0) + ");";
}

// Affiche les infos GPS
String EnvironmentHtml::handleGpsInfo(int satellites, String quality, float altitude, float speed) {
  return "Sat:<b style='font-size:16px; color:" + _getHtmlColor(COLOR_GREY) + "'>" + String(satellites) + " " + quality + "</b> | Altitude: " + String(altitude, 2) + " m | Speed: " + String(speed, 2) + " km/h";
}

String EnvironmentHtml::handleNotification(String content) {
  return "<span class='material-symbols-outlined' style='font-size:12px; color:red;'>warning</span>&nbsp;" + content + "<br>";
}

// affiche le bloc de divs a mettre a jour par ajax
String EnvironmentHtml::_drawEnvironmentGraphs(vsensor* list, int length, int reloadDelay)
{
  String htmlBuffer = "";
  String div = "";
  String js = "";

  for (int i = 0; i < length; i++) {
    div += "<div id='div_" + String(list[i]) + "' class='simple bordered' onclick='call(\"/sensor/" + String(list[i]) + ".svg\", \"div_" + String(list[i]) + "\")'></div>";
    js += "," + String(list[i]);
  }
  js = js.substring(1);

  htmlBuffer += div;
  htmlBuffer += F("\
<div style='width:100%; padding-left:10px;'>\
  <a href='/sensors'>view all sensors raw data</a>\
</div>\
<script>\
  function refresh() {\
    var seq = [");
  htmlBuffer += js;
  htmlBuffer += F("];\
    for (var i in seq) {\
      call('/sensor/' + seq[i] + '.svg', 'div_' + seq[i]);\
    };\
    /*window.setInterval(refresh, ");
  htmlBuffer += String(reloadDelay);
  htmlBuffer += F(");*/\
  };\
  refresh();\
</script>");

  return htmlBuffer;
}

// Affiche un tableau de données
String EnvironmentHtml::_drawSensorDataTable(vfield* sensors, int sensorLength)
{
  String htmlBuffer = "";

  htmlBuffer += "<table>";
  htmlBuffer += "<tr style='background-color:#333333'><td><b>Statut</b></td><td><b>Titre</b></td><td><b>Valeur</b></td><td><b>Tolerance</b></td><td><b>Commentaire</b></td></tr>";
  for (int i = 0; i < sensorLength; i++) {
    htmlBuffer += "<tr>";
    htmlBuffer += "<td><b style='color:" + _getHtmlColor(sensors[i].status) + "'>█</b></td>";
    htmlBuffer += "<td>" + sensors[i].label + "</td>";
    htmlBuffer += "<td>" + String(sensors[i].value) + sensors[i].unit + "</td>";
    htmlBuffer += "<td>" + String(sensors[i].tolerance) + sensors[i].unit + "</td>";
    htmlBuffer += "<td>" + sensors[i].text + "</td>";
    htmlBuffer += "</tr>";
  }
  htmlBuffer += "</table><br>";

  return htmlBuffer;
}
