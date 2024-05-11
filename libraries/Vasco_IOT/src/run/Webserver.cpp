#include "Webserver.h"
#include "uri/UriBraces.h"


bool Webserver::begin(vrun mode)
{
  // first add 404 page
  _server.onNotFound([](){
    String out = "\
      File Not Found\n\n\
      URI: " + String(_server.uri()) + "\n\
      Method: " + String((_server.method() == HTTP_GET) ? "GET" : "POST") + "\n\
      Arguments: ";
    for (int i = 0; i < _server.args(); i++) {
      out += "\n " + _server.argName(i) + ": " + _server.arg(i);
    }
    _server.send(404, "text/plain", out);
  });
  // if wifi connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("Wifi must be connected to begin webserver"));

    return false;
  }
  // init
  _server.begin();
  _processMode = mode;
  _enabled = true;

  return true;
}

bool Webserver::run()
{
  long time = esp_timer_get_time();
  int delay;

  switch (_processMode) {
    case LOW_REFRESH:
      delay = _LAZY_REFRESH_RATE;
      break;
    case HIGH_REFRESH:
    case EVENT_TRIG:
    case CONTINUOUS:
      delay = _AWARE_REFRESH_RATE;
      break;
    default:

      return false;
  }

  if (millis() - _timer > delay) { // TODO vasco count callbacks too, howto
    _timer = millis(); // reset timer
    _server.handleClient();
    _processedTime = _timeBuffer / 1000;
    _timeBuffer = 0;

    return true;
  }

  _timeBuffer += esp_timer_get_time() - time;


  return false;
}

void Webserver::onHtml(const String &uri, std::function<String()> callHtml)
{
  _server.on(Uri(uri), [&, callHtml]() {
    _server.send(200, "text/html", callHtml());
  });
}

void Webserver::onHtml(const String &uri, std::function<String(int)> callHtml)
{
  _server.on(UriBraces(uri), [&, callHtml]() {
    int id = _server.pathArg(0).toInt();
    _server.send(200, "text/html", callHtml(id));
  });
}

void Webserver::onSvg(const String &uri, std::function<String()> callSvg)
{
  _server.on(Uri(uri), [&, callSvg]() {
    _server.send(200, "image/svg+xml", callSvg());
  });
}

void Webserver::onSvg(const String &uri, std::function<String(int)> callSvg)
{
  _server.on(UriBraces(uri), [&, callSvg]() {
    int id = _server.pathArg(0).toInt();
    _server.send(200, "image/svg+xml", callSvg(id));
  });
}

void Webserver::onJpg(const String &uri, std::function<File()> callFile)
{
  _server.on(Uri(uri), [&, callFile]() {
    File file = callFile();
    _server.sendHeader("Cache-Control", "max-age=31536000");
    _server.streamFile(file, "image/jpeg");

    file.close();
  });
}

void Webserver::onJpg(const String &uri, std::function<File(int)> callFile)
{
  _server.on(UriBraces(uri), [&, callFile]() {
    int id = _server.pathArg(0).toInt();
    File file = callFile(id);
    _server.sendHeader("Cache-Control", "max-age=31536000");
    _server.streamFile(file, "image/jpeg");

    file.close();
  });
}

void Webserver::onCommand(const String &uri, std::function<void()> callCommand)
{
  _server.on(Uri(uri), [&, callCommand]() {
    callCommand();
    _server.send(204, "text/plain", "");
  });
}

void Webserver::onCommand(const String &uri, std::function<void(int)> callCommand)
{
  _server.on(UriBraces(uri), [&, callCommand]() {
    int id = _server.pathArg(0).toInt();
    callCommand(id);
    _server.send(204, "text/plain", "");
  });
}
