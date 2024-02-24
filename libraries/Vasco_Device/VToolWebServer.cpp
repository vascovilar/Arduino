#include "VToolWebServer.h"
#include "uri/UriBraces.h"


void VToolWebServer::begin()
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

bool VToolWebServer::update(int delay)
{
  if (millis() - _timer > delay) {
    _timer = millis(); // reset timer
    
    sync();

    return true;
  }

  return false;
}

void VToolWebServer::sync()
{
  _server.handleClient();
}

void VToolWebServer::onHtml(const String &uri, std::function<String()> callHtml)
{
  _server.on(Uri(uri), [&, callHtml]() { 
    _server.send(200, "text/html", callHtml()); 
  }); 
}

void VToolWebServer::onHtml(const String &uri, std::function<String(int)> callHtml)
{
  _server.on(UriBraces(uri), [&, callHtml]() {
    int id = _server.pathArg(0).toInt(); 
    _server.send(200, "text/html", callHtml(id)); 
  }); 
}

void VToolWebServer::onSvg(const String &uri, std::function<String()> callSvg)
{
  _server.on(Uri(uri), [&, callSvg]() { 
    _server.send(200, "image/svg+xml", callSvg()); 
  }); 
}

void VToolWebServer::onSvg(const String &uri, std::function<String(int)> callSvg)
{
  _server.on(UriBraces(uri), [&, callSvg]() { 
    int id = _server.pathArg(0).toInt(); 
    _server.send(200, "image/svg+xml", callSvg(id)); 
  }); 
}

void VToolWebServer::onJpg(const String &uri, std::function<File()> callFile)
{
  _server.on(Uri(uri), [&, callFile]() {
    File file = callFile();
    _server.sendHeader("Cache-Control", "max-age=31536000");
    _server.streamFile(file, "image/jpeg");

    file.close();
  }); 
}

void VToolWebServer::onJpg(const String &uri, std::function<File(int)> callFile)
{
  _server.on(UriBraces(uri), [&, callFile]() {
    int id = _server.pathArg(0).toInt(); 
    File file = callFile(id);
    _server.sendHeader("Cache-Control", "max-age=31536000");
    _server.streamFile(file, "image/jpeg");

    file.close();
  }); 
}

void VToolWebServer::onCommand(const String &uri, std::function<void()> callCommand)
{
  _server.on(Uri(uri), [&, callCommand]() { 
    callCommand();
    _server.send(204, "text/plain", "");
  }); 
}

void VToolWebServer::onCommand(const String &uri, std::function<void(int)> callCommand)
{
  _server.on(UriBraces(uri), [&, callCommand]() { 
    int id = _server.pathArg(0).toInt(); 
    callCommand(id);
    _server.send(204, "text/plain", "");
  });
}
