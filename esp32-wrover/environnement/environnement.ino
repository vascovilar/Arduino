#include "Arduino.h"
#include "BUZ001.h"
#include "ST7789SD.h"
#include "PIM447.h"
#include "BME680.h"
#include "LTR390.h"
#include "EMF001.h"
#include "SEN0487.h"
#include "PA1010D.h"
#include "BMI160X.h"
#include "AH49E.h"
#include "ESP32X.h"
#include "interface/Chipset.h"
#include "run/Container.h"
#include "run/Sequencer.h"
#include "run/Webserver.h"
#include "data/Buffer.h"
#include "data/Html.h"

Container     container;
Webserver     server;
Html          html;
BUZ001        buzzer(4);
ST7789SD      tft(5, 25, 33, 32);
PIM447        pointer(0x0A);
BME680        air(0x77);
LTR390        light(0x53);
EMF001        emf(36);
SEN0487       ear(34);
PA1010D       gps(0x10);
BMI160X       imu(0x68);
AH49E         hall(35);
ESP32X        esp(2);


void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");

  // -----------------------------------------------------
  // Init: devices
  // -----------------------------------------------------

  Serial.print("Devices initialization... ");
  container.bind(buzzer);
  container.bind(tft);
  container.bind(pointer, CONTINUOUS); // to draw points on screen
  container.bind(air);
  container.bind(emf, HIGH_REFRESH); // realtime sensors
  container.bind(light);
  container.bind(ear, HIGH_REFRESH);
  container.bind(gps);
  container.bind(imu, HIGH_REFRESH);
  container.bind(hall);
  container.bind(esp);
  if (container.begin(LOW_REFRESH)) { // for all but already configured
    Serial.println("OK");
  }
  delay(500);

  // -----------------------------------------------------
  // Init: wifi
  // -----------------------------------------------------

  //int lenght = esp.getWifiAccessPoints();
  //Serial.println("bornes: " + String(lenght));
  //for (int i = 0; i < lenght; i++) {
  //  Serial.println(esp.getWifiAccessPointInfo(i));
  //}

  Serial.print("Connecting to Wifi... ");
  if (esp.connectWifi()) {
    Serial.println("OK");
    Serial.println("- sync dateTime from Internet: " + esp.getDateTime());
  } else {
    Serial.println("failed !");
  }
  delay(500);

  // -----------------------------------------------------
  // Init: webserver
  // -----------------------------------------------------

  Serial.print("Initializing webserver...");
  server.onHtml("/", [](){ return html.handleHomePage(3000); });
  for (int i = 0; i < VSENSOR_COUNT; i++) {
    server.onHtml("/sensor/" + String(i) + ".svg", [i](){
      return html.handleHistorySvgGraph(container.getField((vsensor)i), container.getBuffer((vsensor)i));
    });
  }
  //server.onHtml("/sensors", [](){ return getAllSensorsTable(); });
  //server.onHtml("/logger", [](){ return esp.getDateTime() + "  " + logger.dump(SHORT_REFRESH_RATE); });
  if (server.begin(CONTINUOUS)) {
    Serial.println("OK");
    Serial.println("- listening port 80, go: http://" + esp.getIP());
  } else {
    Serial.println("failed");
  }
  delay(500);

  // -----------------------------------------------------
  // TFT
  // -----------------------------------------------------
  if (container.isEnabled(TFT_SD_SCREEN)) {
    Serial.println("Screen: check diplay, fade out when program loops");
    tft.clear();
    tft.led(4095);
    tft.title("IOT #3", 0, 0, COLOR_GREY);
    tft.text("http://" + esp.getIP(), 3, 17, COLOR_WHITE);
    tft.text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat.", 0, 30, COLOR_GREY);
    delay(500);
  }

  // -----------------------------------------------------
  // Buzzer
  // -----------------------------------------------------

  if (container.isEnabled(BUZZER)) {
    Serial.println("Buzzer: beep once");
    buzzer.warning();
    delay(500);
  }

  Serial.println("");
}


void loop()
{
  if (server.run()) {
    //Serial.println("+Webserver: " + String(server.getProcessedTime()) + "ms");
  }

  if (container.run()) {
    for (int i = 0; i < container.updatedLength; i++) {
      switch(container.updatedChipsets[i]) {
        case POINTER:
          {
            // write pixel in screen
            vmouse mouse = pointer.getMouse();
            tft.point(mouse.x, mouse.y, COLOR_WHITE);
            // screen saver
            tft.led(4095, 0, 20000);
          }
          break;
        case GPS_NAVIGATOR:
          {
            /*
            // show some gps data
            tft.clear();
            tft.title("GPS", 0, 0, COLOR_GREY);
            tft.text(String(gps.getDateTime()), 0, 20, COLOR_WHITE);
            tft.text("sat: " + String(gps.getSatellite().value), 0, 30, COLOR_WHITE);
            tft.text("fix: " + String(gps.getFixQuality().text), 0, 40, COLOR_WHITE);
            tft.text("lat: " + String(gps.getLatitude(), 7), 0, 60, COLOR_WHITE);
            tft.text("lon: " + String(gps.getLongitude(), 7), 0, 50, COLOR_WHITE);
            */
          }
          break;
        case INERTIAL_UNIT:
          {
            // screen saver
            if (imu.get(MOVEMENT).value > 0.1) {
              tft.led(4095, 0, 20000);
            }
            /*
            // show some data from inertial device
            vcoord coord = imu.getGyroscope();
            Serial.print(String(coord.x) + ", " + String(coord.y) + ", " + String(coord.z) + " °   ");
            coord = imu.getAccelerometer();
            Serial.print(String(coord.x) + ", " + String(coord.y) + ", " + String(coord.z) + " g   ");
            Serial.print(String(imu.getTemperature()) + "°C    ");
            */
          }
          break;
        case MICRO_CONTROLLER:
          {
            // EVENT manager to do: set color on pointer if one status > limit
            vstatus limit = VERT;
            vstatus max = GRIS;
            for (int i = 0; i < VSENSOR_COUNT; i++) {
              vfield value = container.getField((vsensor) i);
              if (value.status > max) max = value.status;
            }
            if (max > limit) {
              pointer.led(max);
            } else {
              pointer.led(0, 0);
            }
          }
          break;
      }
    }
  }
}
