/*
 * WebServer abstraction
 * Implementation:
 *
 *   #include <VComWebApi.h>
 *
 *   VComWebApi api;
 *
 *   void setup() {
 *     api.onPage("/", [](){ return html.getHome(); });
 *     api.begin();
 *   }
 *   void loop() {
 *     api.update(10);
 *   }
 */

#ifndef VComWebApi_h
#define VComWebApi_h

#include "Arduino.h"
#include "WebServer.h"
#include "FS.h"

static WebServer _server(80);

class VComWebApi
{
  public:   
    
    void begin();
    bool update(int delay);

    void onPage(const String &uri, std::function<String()> callHtml);
    void onPage(const String &uri, std::function<String(int)> callHtml);
    void onXhr(const String &uri, std::function<String()> callHtml);
    void onXhr(const String &uri, std::function<String(int)> callHtml);
    void onJpg(const String &uri, std::function<File()> callFile);
    void onJpg(const String &uri, std::function<File(int)> callFile);
    void onCommand(const String &uri, std::function<void()> callCommand);
    void onCommand(const String &uri, std::function<void(int)> callCommand);
    
    // TODO remove with onXhr ?
    void onSvg(const String &uri, std::function<String()> callHtml);
    
  private:
    
    uint32_t _timer = millis();

};

static String getHtmlWrapper(const String &content) 
{
  return "\
<!DOCTYPE html>\
  <head>\
    <meta charset='UTF-8'>\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
    <link rel='preconnect' href='https://fonts.googleapis.com/'>\
    <link rel='preconnect' href='https://fonts.gstatic.com/' crossorigin=''>\
    <link href='https://fonts.googleapis.com/css2?family=Bebas+Neue&display=swap' rel='stylesheet'>\
    <link href='https://fonts.googleapis.com/icon?family=Material+Icons' rel='stylesheet'>\
    <style>\
      body{padding:10px;background:#000000;font-family:Arial,Helvetica,Sans-Serif;font-size:12px;color:#999999;}\
      h1{font-family:'Bebas Neue';font-size:52px;margin:0px;color:#555555;}\
      h1 .material-icons{font-size:40px;}\
      h2{font-size:18px;margin-bottom: 0px;}\
      h2,h3{color:#DDDDDD;font-weight:bold;}\
      h3{font-family:'Courier';font-size:14px;font-weight:normal;margin:2px;}\
      p{margin-top:0px;}\
      a:link,a:visited,a:hover,a:focus,a:active{color:#366899;text-decoration:none;}\
      a:hover{color:#4aa3fc;}\
      div{float:left;}\
      .bordered{border:1px solid #444444;border-radius:20px;margin:5px;}\
      .padded{padding:20px; padding-top:15px;}\
      .simple{width:440px; height:190px;}\
      .double{width:440px; height:390px;}\
      .tile{margin:3px;padding:5px;border:1px solid #000000;border-radius:10px;background:#000000;text-align:center;}\
      .tile:hover{border-color:#366899;cursor:pointer;}\
      .button{background:#366899;color:#000000;font-size:14px;padding:10px 20px 10px 20px;margin-top:10px;border-radius:20px;font-weight:bold;}\
      .button:hover{background:#4aa3fc;cursor:pointer;}\
      .action{float:right;height:60px;}\
      .action .material-icons{color:#366899;font-size:32px;padding:10px;}\
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
  <body>\
    " + content + "\
  </body>\
</html>";
}

#endif
