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

  // *****************************************************
  // NEEDED runtime setup
  // *****************************************************

  // -----------------------------------------------------
  // Init: devices
  // -----------------------------------------------------

  Serial.print("Devices initialization... ");
  container.bind(buzzer);
  container.bind(tft);
  container.bind(pointer, CONTINUOUS);
  container.bind(air);
  container.bind(emf, HIGH_REFRESH);
  container.bind(light);
  container.bind(ear, HIGH_REFRESH);
  container.bind(gps);
  container.bind(imu, HIGH_REFRESH);
  container.bind(hall);
  container.bind(esp);
  if (container.begin(LOW_REFRESH)) {
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
  for (int field = 0; field < VSENSOR_COUNT; field++) {
    server.onHtml("/sensor/" + String(field) + ".svg", [field](){
      return html.handleHistorySvgGraph(container.getField((vsensor)field), container.getBuffer((vsensor)field));
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

  // *****************************************************
  // TESTS <<<<<<start
  // *****************************************************

  // -----------------------------------------------------
  // Led
  // -----------------------------------------------------

  if (container.isEnabled(MICRO_CONTROLLER)) {
    esp.led(true);
    Serial.println("Led: check esp blue led");
    delay(500);
  }

  // -----------------------------------------------------
  // Pointer
  // -----------------------------------------------------

  if (container.isEnabled(POINTER)) {
    Serial.println("Pointer: check led colors and screen cursor");
    pointer.setBoundary(240, 135);
    pointer.led(COLOR_GREEN, 0); Serial.print("- green"); delay(1000);
    pointer.led(COLOR_YELLOW, 0); Serial.print(" yellow"); delay(1000);
    pointer.led(COLOR_ORANGE, 0); Serial.print(" orange"); delay(1000);
    pointer.led(COLOR_RED, 0); Serial.print(" red"); delay(1000);
    pointer.led(COLOR_VIOLET, 0); Serial.println(" violet"); delay(1000);
    pointer.led(COLOR_BLACK, 0);
    delay(500);
  }

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
  // SD
  // -----------------------------------------------------

  if (container.isEnabled(TFT_SD_SCREEN)) {
    //tft.listFiles();
    //delay(500);
  }

  // -----------------------------------------------------
  // EEPROM & PSRAM
  // -----------------------------------------------------

  //esp.getEepromTest(); // warning destroys rom in using it, keep commented
  esp.getPsramTest();
  delay(500);

  // -----------------------------------------------------
  // Realtime sensors calibration
  // -----------------------------------------------------

  Serial.println("Calibration:");
  Serial.print("- EMF zero: ");
  calibrate(EMF_LEVEL);
  Serial.print("- Hall zero: ");
  calibrate(GAUSS_LEVEL);
  Serial.print("- Sound zero: ");
  calibrate(EAR_LEVEL);
  delay(500);

  // -----------------------------------------------------
  // Buzzer
  // -----------------------------------------------------

  Serial.println("Buzzer: beep once");
  buzzer.warning();
  delay(500);

  // *****************************************************
  // TESTS >>>>>end
  // *****************************************************

  esp.led(4095, 0, 5000);
  tft.led(4095, 0, 5000);
  Serial.println("");
}


void loop()
{
  // *****************************************************
  // NEEDED runtime loop
  // *****************************************************

  if (server.run()) {
    //Serial.println("+Webserver: " + String(server.getProcessedTime()) + "ms");
  }

  if (container.run()) {
    for (int i = 0; i < container.length; i++) {
      switch(container.updated[i]) {
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
            // show data from inertial device
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
            // count rounds as a sensor
            container.setField(RUN_CYCLES, (vfield) {"Check cycles ", "/s", 10.0, container.getProcessedChecksPerSecond(), VERT});
            /*
            // EVENT manager to do: set color on pointer if one status > JAUNE
            vstatus limit = GRIS;
            for (int i = 0; i < VSENSOR_COUNT; i++) {
              vfield value = getSensor((vsensor) i);
              if ((int) value.status > limit) limit = value.status;
            }
            if (limit > JAUNE) {
              pointer.led(limit);
            } else {
              pointer.led(0, 0);
            }
            */
          }
          break;
      }
    }
  }
}


void calibrate(vsensor code)
{
  long value = 0;
  long total = 0;
  int  length = 100;

  for (int i = 0; i < length; i++) {
    switch (code) {
      case EMF_LEVEL:
        value = emf.read();
        break;
      case GAUSS_LEVEL:
        value = hall.read();
        break;
      case EAR_LEVEL:
        value = ear.read();
        break;
      default:

        return;
    }
    total += value;
    delay(10);
  }

  Serial.println(String(total / length));
}
