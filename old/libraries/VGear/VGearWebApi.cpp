#include "VGearWebApi.h"
#include "uri/UriBraces.h"


void VGearWebApi::begin()
{
  _server.onNotFound([](){ 
    String out = "\
      File Not Found\n\n\
      URI: " + String(_server.uri()) + "\n\
      Method: " + String((_server.method() == HTTP_GET) ? "GET" : "POST") + "\n\
      Arguments: ";
    for (uint8_t i = 0; i < _server.args(); i++) {
      out += "\n " + _server.argName(i) + ": " + _server.arg(i);
    }
    _server.send(404, "text/plain", out); 
  });

  _server.begin();
}

bool VGearWebApi::update(int delay)
{
  if (millis() - _timer > delay) {
    _timer = millis(); // reset timer
    
    _server.handleClient();

    return true;
  }

  return false;
}

void VGearWebApi::onPage(const String &uri, std::function<String()> callHtml)
{
  _server.on(Uri(uri), [&, callHtml]() { 
    _server.send(200, "text/html", getHtmlWrapper(callHtml())); 
  }); 
}

void VGearWebApi::onPage(const String &uri, std::function<String(int)> callHtml)
{
  _server.on(UriBraces(uri), [&, callHtml]() {
    int id = _server.pathArg(0).toInt(); 
    _server.send(200, "text/html", getHtmlWrapper(callHtml(id))); 
  }); 
}

void VGearWebApi::onXhr(const String &uri, std::function<String()> callHtml)
{
  _server.on(UriBraces(uri), [&, callHtml]() {
    _server.send(200, "text/html", callHtml()); 
  }); 
}

void VGearWebApi::onXhr(const String &uri, std::function<String(int)> callHtml)
{
  _server.on(UriBraces(uri), [&, callHtml]() {
    int id = _server.pathArg(0).toInt(); 
    _server.send(200, "text/html", callHtml(id)); 
  }); 
}

void VGearWebApi::onJpg(const String &uri, std::function<File()> callFile)
{
  _server.on(UriBraces(uri), [&, callFile]() {
    File file = callFile();
    _server.sendHeader("Cache-Control", "max-age=31536000");
    _server.streamFile(file, "image/jpeg");

    file.close();
  }); 
}

void VGearWebApi::onJpg(const String &uri, std::function<File(int)> callFile)
{
  _server.on(UriBraces(uri), [&, callFile]() {
    int id = _server.pathArg(0).toInt(); 
    File file = callFile(id);
    _server.sendHeader("Cache-Control", "max-age=31536000");
    _server.streamFile(file, "image/jpeg");

    file.close();
  }); 
}

void VGearWebApi::onCommand(const String &uri, std::function<void()> callCommand)
{
  _server.on(Uri(uri), [&, callCommand]() { 
    callCommand();
    _server.send(204, "text/plain", "");
  }); 
}

void VGearWebApi::onCommand(const String &uri, std::function<void(int)> callCommand)
{
  _server.on(UriBraces(uri), [&, callCommand]() { 
    int id = _server.pathArg(0).toInt(); 
    callCommand(id);
    _server.send(204, "text/plain", "");
  });
}

void VGearWebApi::onSvg(const String &uri, std::function<String()> callHtml)
{
  _server.on(Uri(uri), [&, callHtml]() { 
    _server.send(200, "image/svg+xml", callHtml()); 
  }); 
}