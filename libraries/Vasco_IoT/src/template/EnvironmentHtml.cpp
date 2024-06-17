#include "EnvironmentHtml.h"


String EnvironmentHtml::makeHtmlScript()
{
  String tmp = _htmlScript;
  _htmlScript = "";

  return tmp;
}

// Affiche la home
String EnvironmentHtml::handleHomePage(int delay)
{
  String bloc = _getHtmlBlocEnvironment(delay);

  return _getPageWrapper(bloc);
}

// affiche un graphe svg de suivi (width='430' height='190')
String EnvironmentHtml::handleHistorySvgGraph(vfield data, Buffer buffer)
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

  return _getHtmlBlocSvgCartouche(makeHtmlScript());
}

// Affiche un tableau de données
String EnvironmentHtml::handleDataTable(vfield* sensors, int length)
{
  String output = "<tr><th>Statut</th><th>Titre</th><th>Valeur</th><th>Commentaire</th>";

  for (int i = 0; i < length; i++) {
    output += "<tr><td><b style='color:" + _getHtmlColor(sensors[i].status) + "'>█</b></td><td>" + sensors[i].label +"</td><td>" + sensors[i].value + sensors[i].unit + "</td><td>" + sensors[i].text + "</td></tr>";
  }

  return _getPageWrapper("<table>" + output + "</table>");
}

// Met a jour la map
String EnvironmentHtml::handleOsmPoint(float latitude, float longitude, float angle) {
  return "go(" + String(latitude, 7) + ", " + String(longitude, 7) + ", " + String(360 - angle, 0) + ");";
}

// Affiche les infos GPS
String EnvironmentHtml::handleGpsInfo(int satellites, String quality, float altitude, float speed) {
  return "Sat:<b style='font-size:16px; color:" + _getHtmlColor(COLOR_GREY) + "'>" + String(satellites) + " " + quality + "</b> | Altitude: " + String(altitude, 2) + " m | Speed: " + String(speed, 2) + " km/h";
}

String EnvironmentHtml::handleNotification(String text) {
  return "<span class='material-symbols-outlined' style='font-size:12px; color:red;'>warning</span>&nbsp;" + text + "<br>";
}
