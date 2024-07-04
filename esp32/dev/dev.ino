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
#include "interface/Pointer.h"
#include "run/Container.h"
#include "run/Webserver.h"
#include "data/Html.h"
#include "data/Bitmap.h"
#include "data/Mouse.h"
#include "bin/earthImage.h"


//#include "Wire.h"
//#include "SPI.h"


// instanciations come here
//
//BUZ001        buzzer(4);
//ST7789SD      tft(5, 25, 33, 32);
//PIM447        track(0x0A);
//BME680        air(0x77);
//LTR390        light(0x53);
//EMF001        emf(36);
//SEN0487       ear(34);
//PA1010D       gps(0x10);
//BMI160X       imu(0x68);
//AH49E         hall(35);
ESP32X        esp(2);

//Container     container;
//Mouse         mouse(track, tft);
//Bitmap        bitmap(tft, mouse);

static const int screen_brightness = 60;
static const int screen_saver_delay = 30000;
int timer = millis();


void setup()//
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");

  //Wire.begin(21, 22); // for IMU

  // -----------------------------------------------------
  // INIT
  // -----------------------------------------------------

  // list wifi AP
  esp.init();

  int length = esp.getAccessPointsCount();
  for (int i = 0; i < length; i++) {
    Serial.println(esp.getAccessPointInfo(i));
  }

  /*
  tft.init();
  tft.text(100, 60, "test", SIZE_BIG, COLOR_WHITE);
  tft.led(0,10,5);
  //tft.led(100);
  //bitmap.iconTestPage();
  //tft.image(183, 20, VIMAGE_earth, 57, 120);
  */

  /*
  buzzer.init();
  buzzer.warning();
  */

  /*
  tft.line(0, 5, VSCREEN_WIDTH, 5, COLOR_GREY);
  tft.text(0, 5, "custom text jp1", SIZE_TEXT, COLOR_WHITE_DARK, COLOR_MAGENTA, false);
  tft.line(0, 20, VSCREEN_WIDTH, 20, COLOR_GREY);
  tft.text(0, 20, "system text jp1", SIZE_TEXT, COLOR_WHITE_DARK, COLOR_MAGENTA, true);

  tft.line(0, 40, VSCREEN_WIDTH, 40, COLOR_GREY);
  tft.text(0, 40, "custom text jp1", SIZE_TITLE, COLOR_WHITE_DARK, COLOR_MAGENTA, false);
  tft.line(0, 60, VSCREEN_WIDTH, 60, COLOR_GREY);
  tft.text(0, 60, "system text jp1", SIZE_TITLE, COLOR_WHITE_DARK, COLOR_MAGENTA, true);

  tft.text(0, 90, "custom iiiiiiiiiiiiiiiiiiiiii", SIZE_TEXT, COLOR_WHITE_DARK, COLOR_BLUE, false);
  tft.text(0, 90 + VBITMAP_LINESPACING, "system iiiiiiiiiiiiiiiiiiiiii", SIZE_TEXT, COLOR_WHITE_DARK, COLOR_MAGENTA, true);
  */

  /*
  tft.init();
  tft.led(100);
  tft.rect(0, 0, 100, 100, COLOR_GREEN);
  */

  /*
  tft.init();
  tft.setRotation(1);
  delay(500);
  tft.fillScreen(TFT_BLACK);
  Serial.println(tft.readPixel(1, 1));
  */

  /*
  float *tmp = (float*)ps_malloc(1000);

  for (int i = 0; i < 240; i++) {
    tmp[i] = random(0, 1000);
  }
  //tmp[0] = 9.9;

  Serial.println(String(ESP.getFreePsram()) + " / " + String(ESP.getFreeHeap()));
  */

  /*
  Serial.print("Devices initialization... ");

  // needed hardware 25000 turns/S
  container.bind(buzzer);
  container.bind(tft);
  container.bind(track, AWARE); // to draw points on screen
  container.bind(esp, EACH_SECOND);

  // ADC consumers, realtime sensors
  container.bind(hall); // curently disconnected use nothing
  container.bind(emf); // big load fpm to 1500
  container.bind(ear); // big load fpm to 1500

  // I2C consumers
  container.bind(gps); // curently disconnected use nothing
  container.bind(air);
  container.bind(imu);
  container.bind(light); // big load fpm to 3000

  // let's run
  if (container.begin(HIGH_FREQUENCY)) { // for all but already configured
    Serial.println("OK");
  }

  // light screen
  tft.led(screen_brightness);
  */
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
  // put your main code here, to run repeatedly:








  //Serial.println("*** " + String(esp.getDateTime()));
  //delay(1000);

  /*
  ear.update();
  Serial.println(String("1700," + String(ear.read()) + ",1740"));
  */

  /*
  imu.update();
  vcoord coord = imu.getTest();
  Serial.println(String(coord.x) + "," + String(coord.y) + "," + String(coord.z));
  */

  /*
  emf.update();
  Serial.println(String("0," + String(emf.getMaxValue().value) + ",100"));
  *

  /*
  if (track.update()) {
    vpointer pointer = track.get();
    isFreezed = pointer.click;
  }
  if (!isFreezed) {
    bitmap.handleRealtimeUpdate(ear, 240, 25);
  }
  */

  /*
  track.led(status);
  */

  // -----------------------------------------------------
  //Serial.print(String((millis() - timer)) + "ms");
  //Serial.println("");
  //timer = millis();
  //delay(1000);
}
