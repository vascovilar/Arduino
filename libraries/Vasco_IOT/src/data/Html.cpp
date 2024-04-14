#include "Html.h"

// Affiche la home
String Html::handleHomePage(int delay) 
{
  return _getHtmlWrapper(_getHtmlGlobalEnvironmentPage(delay));
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
      // minutes 
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
  
  return _getHtmlSvgCartouche(
    text,
    grid,
    graph
  );
}

// Affiche un tableau de données
String Html::handleDataTable(vfield* sensors, int length)
{
  String output = "<tr><th>Statut</th><th>Titre</th><th>Valeur</th><th>Commentaire</th>";

  for (int i = 0; i < length; i++) {
    output += "<tr><td><b style='color:" + _getHtmlColor(sensors[i].status) + "'>█</b></td><td>" + sensors[i].label +"</td><td>" + sensors[i].value + sensors[i].unit + "</td><td>" + sensors[i].text + "</td></tr>";
  }

  return _getHtmlWrapper("<table>" + output + "</table>");
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

