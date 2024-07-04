#include "Html.h"


String Html::popHtmlBuffer()
{
  String tmp = _htmlBuffer;
  _htmlBuffer = "";

  return tmp;
}

void Html::text(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor, bool isFixedWidthFont)
{
  if (size == SIZE_BIG) {
    _htmlBuffer += "<text x='" + String((810 - x) - (String(content).length()) * 24) + "' y='44' font-size='" + _getHtmlSize(size) + "' font-weight='bold' fill='url(#shaded)' letter-spacing='-3' transform='scale(0.5, 1)'>" + content + "</text>";
  } else {
    _htmlBuffer += "<text x='" + String(x) + "' y='" + String(y) + "' font-size='" + _getHtmlSize(size) + "' fill='" + _getHtmlColor(color) + "'>" + content + "</text>";
  }
}

void Html::point(float x, float y, vcolor color)
{
  _htmlBuffer += "<line x1='" + String(x) + "' y1='" + String(y) + "' x2='" + String(x) + "' y2='" + String(y) + "' stroke='" + _getHtmlColor(color) + "' />";
}

void Html::line(float x1, float y1, float x2, float y2, vcolor color)
{
  _htmlBuffer += "<line x1='" + String(x1) + "' y1='" + String(y1) + "' x2='" + String(x2) + "' y2='" + String(y2) + "' stroke='" + _getHtmlColor(color) + "' />";
}

void Html::rect(float x, float y, int width, int height, vcolor color, int radius, bool isFilled)
{
  _htmlBuffer += "<rect x='" + String(x) + "' y='" + String(y) + "' width='" + String(width) + "' height='" + String(height) + "' fill='" + _getHtmlColor(color) + "' />";;
}

void Html::arrow(float x, float y, int width, int height, vcolor color, vdirection direction)
{
  // TODO vasco ** make other arrow directions
  _htmlBuffer += "<polygon points='" + String(x) + " " + String(y) + ", " + String(x + 6) + " " + String(y + 3) + ", " + String(x + 6) + " " + String(y - 3) + "' fill='" + _getHtmlColor(color) + "' />";
}

void Html::circle(float x, float y, int radius, vcolor color)
{
  _htmlBuffer += "<circle cx='" + String(x) + "' cy='" + String(y) +"' r='" + String(radius) + "' fill='" + _getHtmlColor(color) + "' />";
}

void Html::bitmap(float x, float y, const unsigned char* data, int width, int height, vcolor color, vcolor bgColor)
{
  // TODO vasco ** complete now
}

void Html::image(float x, float y, const uint16_t* data, int width, int height)
{
  // TODO vasco ** complete now
}

float Html::width(String content, vtextsize size, bool isFixedWidthFont)
{
  switch(size) {
    case  SIZE_SMALL:
      return 6;
    case SIZE_TEXT:
      return 8;
    case SIZE_TITLE:
      return 10;
    case SIZE_BIG:
      return 15;
  }

  return 0;
}

float Html::height(String content, vtextsize size)
{
  switch (size) {
    case  SIZE_SMALL:
      return 8;
    case SIZE_TEXT:
      return 12;
    case SIZE_TITLE:
      return 14;
    case SIZE_BIG:
      return 30;
  }

  return 0;
}

void Html::vertical(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor)
{
  // not used here
}

void Html::copy(float x, float y, int width, int height)
{
  // not used here
}
void Html::paste(float x, float y, int width, int height)
{
  // not used here
}

void Html::clear()
{
  // not used here
}

// affiche la page html, contient le header avec CSS et JS
String Html::_drawPageWrapper(String content)
{
  String htmlBuffer = "";

  htmlBuffer += F("\
<!DOCTYPE html>\
  <head>\
    <meta charset='UTF-8'>\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
    <link rel='preconnect' href='https://fonts.googleapis.com/'>\
    <link rel='preconnect' href='https://fonts.gstatic.com/' crossorigin=''>\
    <link href='https://fonts.googleapis.com/css2?family=Bebas+Neue&display=swap' rel='stylesheet'>\
    <link href='https://fonts.googleapis.com/icon?family=Material+Symbols+Outlined' rel='stylesheet'>\
    <style>\
      body{padding:10px;background:#000000;font-family:Arial,Helvetica,Sans-Serif;font-size:12px;color:#999999;}\
      h1{font-family:'Bebas Neue';font-size:42px;margin:0px;padding-right:20px;color:#333333;}\
      h1 .material-symbols-outlined{font-size:40px;}\
      td{padding-right:10px; border-bottom: 1px solid #333333;}\
      a:link,a:visited,a:hover,a:focus,a:active{color:#366899;text-decoration:none;}\
      a:hover{color:#4aa3fc;}\
      div{float:left;}\
      .bordered{border:1px solid #333333;border-radius:20px;margin:5px;overflow:hidden;}\
      .padded{padding:20px; padding-top:15px;}\
      .simple{width:430px; height:190px;cursor:pointer;}\
      \
      .double{width:430px; height:390px;}\
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
          };\
        };\
      };\
      function exec(uri) {\
        var xhr = new XMLHttpRequest();\
        xhr.open('GET', uri);\
        xhr.send();\
        xhr.onload = function() {\
          if (xhr.status == 200) {\
            eval(xhr.response);\
          };\
        };\
      };\
      function reload(obj) {\
        obj.style.display = 'none';\
        setTimeout(function(){location.reload();}, 1000);\
      };\
    </script>\
  </head>\
  <body>");
  htmlBuffer += content;
  htmlBuffer += F("</body>\
</html>");

  return htmlBuffer;
}

// affiche le titre
String Html::_drawTitleBloc(String title, String subtitle)
{
  String htmlBuffer = "";

  htmlBuffer += F("\
<h1>\
  <span style='color:#999999'> \
    <span class='material-symbols-outlined'>guardian</span> ");
  htmlBuffer += title;
  htmlBuffer += F(" \
  </span>");
  htmlBuffer += subtitle;
  htmlBuffer += F(" \
</h1>");

  return htmlBuffer;
}

// affiche le bloc graphique svg
String Html::_drawSvgCartouche(String content)
{
  String htmlBuffer = "";

  htmlBuffer += F("\
<svg xmlns='http://www.w3.org/2000/svg' version='1.1' width='430' height='190'>\
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
      <stop offset='0%' stop-color='#999999' />\
      <stop offset='100%' stop-color='#333333' />\
    </linearGradient>\
  </defs>\
  <rect width='430' height='190' fill='url(#gradient)' rx='20' ry='20' />\
  <g>");
  htmlBuffer += content;
  htmlBuffer += F("</g>\
</svg>");

  return htmlBuffer;
}

String Html::_drawDataTable(vdatatable* data, int length)
{
  String htmlBuffer = "";

  htmlBuffer += "<table>";
  for (int i = 0; i < length; i++) {
    htmlBuffer += "<tr"  + String(i == 0 ? " style='background-color:#333333'": "") + ">";
    htmlBuffer += "<td>" + String(i == 0 ? "<b>": "") + data[i].id + String(i == 0 ? "</b>": "") + "</td>";
    htmlBuffer += "<td>" + String(i == 0 ? "<b>": "") + data[i].title + String(i == 0 ? "</b>": "") + "</td>";
    htmlBuffer += "<td>" + String(i == 0 ? "<b>": "") + data[i].value + String(i == 0 ? "</b>": "") + "</td>";
    htmlBuffer += "<td>" + String(i == 0 ? "<b>": "") + data[i].description + String(i == 0 ? "</b>": "") + "</td>";
    htmlBuffer += "<td>" + String(i == 0 ? "<b>": "") + data[i].comment + String(i == 0 ? "</b>": "") + "</td>";
    htmlBuffer += "<tr>";
  }
  htmlBuffer += "</table><br>";

  return htmlBuffer;
}

String Html::_getHtmlSize(vtextsize size)
{
  switch (size) {
    case  SIZE_SMALL:
      return "8px";
    case SIZE_TEXT:
      return "12px";
    case SIZE_TITLE:
      return "14px";
    case SIZE_BIG:
      return "48px";
  }

  return "";
}

String Html::_getHtmlColor(vcolor color)
{
  String value = String(color, HEX);
  for (int i = value.length() + 1; i <= 6; i++) { // count without extra character for the null terminator
    value = "0" + value;
  }

  // example "#999999"
  return "#" + value;
}

String Html::_getHtmlColor(vstatus status)
{
  vcolor color = _convert(status);

  return _getHtmlColor(color);
}
