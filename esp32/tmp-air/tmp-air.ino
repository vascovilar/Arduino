/*
 * Environment project
 *
 *
 *  --------------------------
 *  |                        |
 *  |    ONLY BME680 TEST    |
 *  |                        |
 *  --------------------------
 */

#include "Arduino.h"
#include "ESP32X.h"
#include "BAT001.h"
#include "BME680.h"
#include "run/Webserver.h"
#include "run/Container.h"
#include "template/EnvironmentHtml.h"

ESP32X            esp(BUILTIN_LED);
BME680            air(0x77);

Webserver         server;
Container         container;
EnvironmentHtml   html;

static const int  javascript_reload_delay = 10000;


void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");

  initDevices();
  startServer();
}

void loop()
{
  server.run();

  if (container.run()) {
    printData();
  }
}

void initDevices()
{
  Serial.print("Initialize devices... ");
  container.bind(air);
  container.bind(esp, EACH_SECOND); // the IoT metronom
  // let's run
  if (container.begin(HIGH_FREQUENCY)) {
    Serial.println("OK");
  } else {
    Serial.println("failed");
  }
}

void startServer()
{
  // Init: wifi
  Serial.print("Connecting to Wifi... ");
  if (esp.connectWifi()) {
    Serial.println("OK");
    Serial.println("- sync dateTime from web: " + esp.getDateTime());
  } else {
    Serial.println("failed");
  }
  delay(500);

  // Init: webserver
  Serial.print("Initializing webserver...");
  server.onHtml("/", [](){
    return html.handleHomePage("IoT4", "environment graph", container.list, container.length, javascript_reload_delay);
  });
  for (int i = 0; i < container.length; i++) {
    server.onSvg("/sensor/" + String(container.list[i]) + ".svg", [i](){
      return html.handleHistorySvgGraphs(container.getField(container.list[i]), container.getBuffer(container.list[i]));
    });
  }
  if (server.begin(AWARE)) {
    Serial.println("OK");
    Serial.println("- listening to port 80, go: http://" + esp.getIp());
  } else {
    Serial.println("failed");
  }
  delay(500);
}

void printData()
{
  Serial.println("\n");
  Serial.println("---------------------------------------------");
  for (int i = 0; i < container.length; i++) {
    vfield field = container.getField(container.list[i]);
    Serial.print(complete(field.label) + "\t| " + String(field.value) + " " + field.unit + "\n");
    Serial.println("---------------------------------------------");
  }
}

String complete(String content)
{
  while (content.length() < 20) {
    content += " ";
  }

  return content;
}
