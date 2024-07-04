/*
 * Environment project
 *
 * Outdoor chipset
 * Including BME680 (meteo), SI1145 (light) and SEN0487 (sound)
 * esp32s3 (https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/)
 *
 * board: Select XIAO ESP32S3
 * port: /dev/ttyACM1 or ACM0
 * src: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
 *
 * modified with https://github.com/boschsensortec/BSEC-Arduino-library/pull/57 content
 */

#include "Arduino.h"
#include "ESP32X.h"
#include "SI1145.h"
#include "BME680.h"
#include "SEN0487.h"
#include "BAT001.h"
#include "run/Webserver.h"
#include "run/Container.h"
#include "template/EnvironmentHtml.h"

ESP32X            esp(BUILTIN_LED);
SI1145            light(0x60);
BME680            air(0x77);
SEN0487           ear(1);
BAT001            bat(9);

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
  ledDemo(300);
}

void loop()
{
  server.run();

  if (container.run()) {
    vfield worst = container.getWorstField();
    showStatus(worst.status);

    if (isTouched()) {
      ledDemo(100);
    }

    //printData();
  }
}


void initDevices()
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(4, HIGH);

  Serial.print("Initialize devices... ");
  container.bind(light); // 2
  container.bind(air); // 1
  container.bind(ear); // 5
  container.bind(bat); // 12
  container.bind(esp, EACH_SECOND); // 0 this is the IoT metronom
  // let's run
  if (container.begin(HIGH_FREQUENCY)) {
    Serial.println("OK");
  } else {
    Serial.println("failed");
  }
}

void startServer()
{
  bool isSuccess = true;
  digitalWrite(4, LOW);
  digitalWrite(3, HIGH);

  // Init: wifi
  Serial.print("Connecting to Wifi... ");
  if (esp.connectWifi()) {
    Serial.println("OK");
    Serial.println("Sync dateTime from web: " + esp.getDateTime());
  } else {
    Serial.println("failed");
    isSuccess = false;
  }
  delay(500);

  // Init: webserver
  Serial.print("Initializing webserver...");

  server.onHtml("/", [](){
    vcodes list = container.getBindedCodes();
    return html.handleHomePage("IoT4", "environement", list.sensorCodes, list.sensorLength, javascript_reload_delay);
  });

  vcodes list = container.getBindedCodes();
  for (int i = 0; i < list.sensorLength; i++) {
    server.onSvg("/sensor/" + String(list.sensorCodes[i]) + ".svg", [i](){
      vcodes list = container.getBindedCodes();
      return html.handleHistorySvgGraphs(container.getField(list.sensorCodes[i]), container.getBuffer(list.sensorCodes[i]));
    });
  }

  server.onHtml("/sensors", [](){
    vfield  fields[VSENSOR_COUNT];
    vcodes  list = container.getBindedCodes();
    for (int i = 0; i < list.sensorLength; i++) fields[i] = container.getField(list.sensorCodes[i]);
    vdatatables chipsetTable = container.getChipsetDataTable(list.chipsetCodes, list.chipsetLength);
    vdatatables sensorTable = container.getSensorDataTable(list.sensorCodes, list.sensorLength);
    return html.handleDataTables("IoT4", "snapshot", chipsetTable.lines, chipsetTable.length, fields, list.sensorLength);
  });

  if (server.begin(AWARE)) {
    Serial.println("OK");
    Serial.println("Listening to port 80, go: http://" + esp.getIp());
  } else {
    Serial.println("failed");
    isSuccess = false;
  }
  delay(500);

  digitalWrite(3, LOW);
  digitalWrite(2, HIGH);

  if (isSuccess) {
    Serial.println("OK");
  } else {
    Serial.println("KO");
  }
}

bool isTouched()
{
  return touchRead(8) > 300000;
}

void showStatus(vstatus status)
{
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  switch (status) {
    case VERT:
    case JAUNE:
      digitalWrite(2, HIGH); // LED GREEN
      break;
    case ORANGE:
      digitalWrite(3, HIGH); // LED ORANGE
      break;
    case ROUGE:
      digitalWrite(4, HIGH); // LED RED
      break;
  }
}

void ledDemo(int time)
{
  digitalWrite(2, HIGH);
  delay(time*2);
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  delay(time);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  delay(time);
  digitalWrite(4, LOW);
}

void printData()
{
  vcodes list = container.getBindedCodes();
  Serial.println("\n");
  Serial.println("---------------------------------------------");
  for (int i = 0; i < list.sensorLength; i++) {
    vfield field = container.getField(list.sensorCodes[i]);
    Serial.print(complete(field.label) + "\t| " + String(field.value) + " " + field.unit + "\n");
    Serial.println("---------------------------------------------");
  }
  Serial.println(complete("Touch") + "\t| " + String(touchRead(8)));
  Serial.println("---------------------------------------------");
}

String complete(String content)
{
  while (content.length() < 20) {
    content += " ";
  }

  return content;
}
