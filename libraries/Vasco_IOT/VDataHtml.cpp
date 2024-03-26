#include "VDataHtml.h"

// Affiche la home
String VDataHtml::handleHomePage(int delay) 
{
  return _getHtmlWrapper(_getHtmlGlobalEnvironmentPage(delay));
}

// affiche un graphe svg de suivi
String VDataHtml::handleHistorySvgGraph(VDataBuffer buffer, field_data data)
{
  String background, text, grid, lines, trend;
  float top = buffer.getMaximum() + data.tolerance / 2;
  float bottom = buffer.getMinimum() - data.tolerance / 2;
  float average = buffer.getAverage();
  float maximum = buffer.getMaximum();
  float minimum = buffer.getMinimum();
  float* history = buffer.getHistory();
  
  byte length = buffer.getLenght();
  if (length > 240) {
    length = 240;
  }

  background = "grey";
  // titre à gauche
  text += _getHtmlSvgCircle(25, 20, _getHtmlColor(data.status));
  text += _getHtmlSvgText(40, 25, 14, "white", data.label);
  text += _getHtmlSvgText(40, 43, 12, "grey", data.text + " +/-" + String(buffer.getDelta()) + " (" + String(data.tolerance) + ")");
  // titre à droite
  if (data.status != GRIS) {
    text += _getHtmlSvgBig(50, data.value);
    text += _getHtmlSvgText(390, 23, 14, "#555555", data.unit);
    if (buffer.getTrend() == 2) {
      trend += "▼";
    }
    if (buffer.getTrend() == 1) {
      trend += "▲";
    }
    text += _getHtmlSvgText(390, 45, 18, "#555555", trend);
  }

  if (length > 0) {    
    for (int i = 0; i < length; i++) 
    {
      int x1 = 410 - i * 1.5;
      int x2 = 410 - (i + 1) * 1.5;

      // graphe
      if (i != 0) {
        int y1 = _isometric(history[i - 1], top, bottom, 100, 60);
        int y2 = _isometric(history[i], top, bottom, 100, 60);
        lines += _getHtmlSvgLine(x1, y1, x2, y2);
      } else {
        int y2 = _isometric(history[i], top, bottom, 100, 60);
        int y3 = _isometric(data.value, top, bottom, 100, 60);
        lines += _getHtmlSvgLine(x1, y2, x2, y2);
        // fleche droite (valeur en cours)
        text += _getHtmlSvgArrow(x1, y3, "grey");
      }
      // minutes 
      if (i % 20 == 0) {
        grid += _getHtmlSvgLine(x1, 60, x1, 165, 0.5);
        text += _getHtmlSvgText(x1 - 3, 173, 8, "grey", String((i * VDataBuffer::_DELAY_TO_PUSH) / 1000 / 60));
      }
    }
    
    // lignes horizontales en fct des valeurs max, min, moyenne
    float limit[3] = {maximum, minimum, average};
    for (int i = 0; i < 3; i++) {
      int pos = _isometric(limit[i], top, bottom, 100, 60);
      grid += _getHtmlSvgLine(50, pos, 410, pos);
      text += _getHtmlSvgText(20, pos + 3, 10, "grey", String(limit[i]));
    }
  }
  
  return _getHtmlSvgCartouche(
    background,
    text,
    grid,
    lines
  );
}

String VDataHtml::handleDataTable(field_data* sensors, byte length)
{
  String output = "<tr><th>Statut</th><th>Titre</th><th>Valeur</th><th>Commentaire</th>";

  for (int i = 0; i < length; i++) {
    output += "<tr><td><b style='color:" + _getHtmlColor(sensors[i].status) + "'>█</b></td><td>" + sensors[i].label +"</td><td>" + sensors[i].value + sensors[i].unit + "</td><td>" + sensors[i].text + "</td></tr>";
  }

  return _getHtmlWrapper("<table>" + output + "</table>");
}


/*// affiche un graphe svg oscilloscope
String VDataHtml::handleSvgGraph(String title, float* data, buffer_data_stat info)
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

    for (int i = 0; i <= 100; i++) 
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
}*/

// Met a jour la map
String VDataHtml::handleOsmPoint(float latitude, float longitude, float angle) {
  return "go(" + String(latitude, 7) + ", " + String(longitude, 7) + ", " + String(360 - angle, 0) + ");";
}

// Affiche les infos GPS
String VDataHtml::handleGpsInfo(int satellites, String quality, float altitude, float speed) {
  return "Sat:<b style='font-size:16px; color:white'>" + String(satellites) + " " + quality + "</b> | Altitude: " + String(altitude, 2) + " m | Speed: " + String(speed, 2) + " km/h";
}

String VDataHtml::handleNotification(String text) {
  return "<span class='material-icons' style='font-size:12px; color:red;'>warning</span>&nbsp;" + text + "<br>";
}

int VDataHtml::_isometric(float value, float maximum, float minimum, int height, int offset) {
  return int((1 - ((value - minimum) / (maximum - minimum))) * height) + offset;
}

String VDataHtml::_getHtmlColor(byte code) 
{
  switch (code) {
    case GRIS:
      return "#444444";
    case VERT:
      return "#4eb400";
    case JAUNE:
      return "#f7e400";
    case ORANGE:
      return "#f85900";
    case ROUGE:
      return "#d8001d";
    case VIOLET:
      return "#6B49C8";          
  }

  return "";
}

String VDataHtml::_getHtmlSvgLine(int x1, int y1, int x2, int y2, float size) 
{
  return "<line x1='" + String(x1) + "' y1='" + String(y1) + "' x2='" + String(x2) + "' y2='" + String(y2) + "' stroke-width='" + String(size) + "' />"; 
}

String VDataHtml::_getHtmlSvgText(int x, int y, int size, String color, String text) 
{
  return "<text x='" + String(x) + "' y='" + String(y) + "' font-size='" + String(size) + "px' fill='" + String(color) + "'>" + text + "</text>";
}

String VDataHtml::_getHtmlSvgBig(int offset, float value) 
{
  return "<text x='" + String((810 - offset) - (String(value).length()) * 24) + "' y='44' font-size='48px' font-weight='bold' fill='url(#shaded)' letter-spacing='-3' transform='scale(0.5, 1)'>" + String(value) + "</text>";
}

String VDataHtml::_getHtmlSvgRect(int x, int y, int w, int h, String color) 
{
  return "<rect x='" + String(x) + "' y='" + String(y) + "' width='" + String(w) + "' height='" + String(h) + "' fill='" + color + "' />";;
}

String VDataHtml::_getHtmlSvgArrow(int x, int y, String color) 
{
  return "<polygon points='" + String(x + 3) + " " + String(y) + ", " + String(x + 13) + " " + String(y + 5) + ", " + String(x + 13) + " " + String(y - 5) + "' fill='" + color + "' />";
}

String VDataHtml::_getHtmlSvgCircle(int x, int y, String color) 
{
  return "<circle cx='" + String(x) + "' cy='" + String(y) +"' r='8' fill='" + color + "' />";
}

String VDataHtml::_getHtmlLink(String href, String text) 
{
  return "<a href='" + href + "'>" + text + "</a>";
}

