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

//#include "Wire.h"
//#include "SPI.h"


// instanciations come here

BUZ001        buzzer(4);
ST7789SD      tft(5, 25, 33, 32);
PIM447        track(0x0A);
BME680        air(0x77);
LTR390        light(0x53);
EMF001        emf(36);
SEN0487       ear(34);
PA1010D       gps(0x10);
BMI160X       imu(0x68);
AH49E         hall(35);
ESP32X        esp(2);

Container     container;
Mouse         mouse(track, tft);
Bitmap        bitmap(tft, mouse);

static const int screen_brightness = 60;
static const int screen_saver_delay = 30000;
int timer = millis();



typedef struct{
  int len;
  float buff[240];
} Duffer;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");

  //Wire.begin(21, 22); // for IMU

  // -----------------------------------------------------
  // INIT
  // -----------------------------------------------------

  Serial.println(String(ESP.getFreePsram()) + " / " + String(ESP.getFreeHeap()));

  

/*
Duffer *bufferA = (Duffer*)ps_malloc(sizeof(Duffer));

//bufferA = new Duffer();                      
bufferA->len = 3;
bufferA->buff[0] = 4.1;
*/

/**/
  float *tmp = (float*)ps_malloc(1000);

  for (int i = 0; i < 240; i++) {
    tmp[i] = random(0, 1000);
  }
  //tmp[0] = 9.9;

  Serial.println(String(ESP.getFreePsram()) + " / " + String(ESP.getFreeHeap()));





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
  if (container.begin(HIGH_REFRESH)) { // for all but already configured
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
  */

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
