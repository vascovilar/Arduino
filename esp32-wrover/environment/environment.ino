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
#include "component/Wifi.h"
#include "run/Webserver.h"
#include "run/Container.h"
#include "data/Html.h"
#include "data/Bitmap.h"
#include "data/Mouse.h"
#include "template/EnvironmentBmp.h"


BUZ001            buzzer(4);
ST7789SD          tft(5, 25, 33, 32);
PIM447            track(0x0A);
BME680            air(0x77);
LTR390            light(0x53);
EMF001            emf(36);
SEN0487           ear(34);
PA1010D           gps(0x10);
BMI160X           imu(0x68);
AH49E             hall(35);
ESP32X            esp(2);
Webserver         server;

Container         container;
Mouse             mouse(track, tft);
EnvironmentBmp    bitmap(tft, mouse);
Html              html(server);

static const int  screen_brightness = 100;
static                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 const int  screen_transition = 300;
static const int  screen_saver_delay = 300000;
long              _lastActivityTime = 0;


void initialize()
{
  // Init: devices
  Serial.print("Devices initialization... ");

  // needed hardware 25000 fps
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
  tft.led(10);
  tft.show("initialized");
  delay(1000);

/**/
  // Init: wifi
  tft.show("connecting to WIFI");
  Serial.print("Connecting to Wifi... ");
  if (esp.connectWifi()) {
    Serial.println("OK");
    Serial.println("- sync dateTime from web: " + esp.getDateTime());
  } else {
    Serial.println("failed !");
  }
  delay(500);

  // Init: webserver
  Serial.print("Initializing webserver...");
  server.onHtml("/", [](){ return html.handleHomePage(3000); });
  for (int i = 0; i < VSENSOR_COUNT; i++) {
    server.onHtml("/sensor/" + String(i) + ".svg", [i](){
      return html.handleHistorySvgGraph(container.getField((vsensor)i), container.getBuffer((vsensor)i));
    });
  }
  if (server.begin(AWARE)) {
    Serial.println("OK");
    Serial.println("- listening port 80, go: http://" + esp.getIp());
  } else {
    Serial.println("failed");
  }
  delay(500);

}


void onClick(vpage force = NONE)
{
  vpage triggered = (force == NONE) ? mouse.click(): force;

  switch (triggered)
  {
    case HOME_PAGE:
      tft.led(0);
      container.setCurrentPage(HOME_PAGE); // TODO vasco *** reintegrer ca dans la classe container
      bitmap.handleHomePage("IoT", esp.getIp(), esp.getNetwork(), VSCREEN_OFFSET_HEADER + 18);
      bitmap.handleHomeUpdate(esp.getClockWatch(), esp.get(RUN_CYCLES), esp.getNetwork() != "", 0);
      container.resume();
      tft.led(0, screen_brightness, screen_transition);
      break;
    case CURRENT_EVENT:
      {
        tft.led(0);
        vsensor code = container.getCurrentEvent();
        container.setCurrentPage(CURRENT_EVENT);
        bitmap.handleHistoryPage("EVENT", container.getField(code), container.getCurrentDelay(code), COLOR_BLACK, VSCREEN_OFFSET_HEADER);
        bitmap.handleHistoryUpdate(code, container.getField(code), container.getBuffer(code), COLOR_BLACK, VSCREEN_OFFSET_HEADER);
        tft.led(0, screen_brightness, screen_transition);
      }
      break;
    case BUFFER_UP:
    case BUFFER_DOWN:
      {
        //rules to select page
        int id = (int)container.getCurrentSensor();
        if (triggered == BUFFER_UP) id++;
        if (triggered == BUFFER_DOWN) id--;
        if (id < 0) id = VSENSOR_COUNT - 1;
        if (id >= VSENSOR_COUNT) id = 0;
        vsensor code = (vsensor)id;
        container.setCurrentSensor(code);

        container.setCurrentPage(triggered);
        bitmap.handleHistoryPage("PROBE", container.getField(code), container.getCurrentDelay(code), COLOR_BLACK, VSCREEN_OFFSET_HEADER);
        bitmap.handleHistoryUpdate(code, container.getField(code), container.getBuffer(code), COLOR_BLACK, VSCREEN_OFFSET_HEADER);
      }
      break;
    case EAR_REALTIME:
      tft.led(0);
      container.setCurrentPage(EAR_REALTIME);
      bitmap.handleRealtimePage("EAR", VSCREEN_OFFSET_HEADER); // TODO vasco *** à variabiliser
      //container.pause();
      tft.led(0, screen_brightness, screen_transition);
      break;
    case EMF_REALTIME:
      tft.led(0);
      container.setCurrentPage(EMF_REALTIME);
      bitmap.handleRealtimePage("EMF", VSCREEN_OFFSET_HEADER);
      container.pause();
      tft.led(0, screen_brightness, screen_transition);
      break;
    case SAT_TABLE:
      tft.led(0);
      container.setCurrentPage(SAT_TABLE);
      bitmap.handleSatellitePage("SAT", VSCREEN_OFFSET_HEADER + 10);
      bitmap.handleSatelliteUpdate(gps.getDateTime(), gps.getSatellite(), gps.getLatitude(), gps.getLongitude(), gps.getSpeed(),
        container.getField(ALTITUDE), gps.getCompassAngle(), gps.getFixQuality(), VSCREEN_OFFSET_HEADER + 10);
      tft.led(0, screen_brightness, screen_transition);
      break;
  }
}


void onUpdate()
{
  bool activity = false;

  if (container.changed(POINTER)) {
    // screen saver
    if (tft.magnitude() == 0) {
      tft.led(0, screen_brightness, screen_transition);
    }
    activity = true;
    // page manager
    onClick();
    // draw cursor
    mouse.draw();
  }

  if (container.changed(MICRO_CONTROLLER)) {
    // page manager
    if (container.getCurrentPage() == HOME_PAGE) {
      bitmap.handleHomeUpdate(esp.getClockWatch(), esp.get(RUN_CYCLES), esp.getNetwork() != "", 0);
    }
    // manage global events
    isEvent();
  }

  if (container.changed(GPS_NAVIGATOR)) {
    // page manager
    if (container.getCurrentPage() == SAT_TABLE) {
      bitmap.handleSatelliteUpdate(gps.getDateTime(), gps.getSatellite(), gps.getLatitude(), gps.getLongitude(), gps.getSpeed(),
        container.getField(ALTITUDE), gps.getCompassAngle(), gps.getFixQuality(), VSCREEN_OFFSET_HEADER + 10);
    }
  }

  if (container.changed(METEO_SENSOR)) { // TODO vasco only METEO ?
    // page manager
    if (container.getCurrentPage() == CURRENT_EVENT) {
      vsensor code = container.getCurrentEvent();
      bitmap.handleHistoryUpdate(code, container.getField(code), container.getBuffer(code), COLOR_BLACK, VSCREEN_OFFSET_HEADER);
    }
    if (container.getCurrentPage() == BUFFER_UP || container.getCurrentPage() == BUFFER_DOWN) {
      vsensor code = container.getCurrentSensor();
      bitmap.handleHistoryUpdate(code, container.getField(code), container.getBuffer(code), COLOR_BLACK, VSCREEN_OFFSET_HEADER);
    }
  }

  if (container.changed(INERTIAL_UNIT)) {
    // screen saver
    if (imu.get(MOVEMENT).value > 0.1) {
      if (tft.magnitude() == 0) {
        tft.led(0, screen_brightness, screen_transition);
      }
      activity = true;
    }
  }

  // activity & screensaver
  if (activity) {
    _lastActivityTime = millis();
  }
  if (millis() - _lastActivityTime > screen_saver_delay) {
    // turn off if not already done
    if (tft.magnitude() != 0) {
      tft.led(screen_brightness, 0, screen_transition);
    }
  }
}

void onCheck()
{
  // update realtime page
  switch (container.getCurrentPage()) {
    case EAR_REALTIME:
      if (container.isEnabled(MICROPHONE_SENSOR)) {
        bitmap.handleRealtimeUpdate(ear, 240, container.getProcessedChecks(), VSCREEN_OFFSET_HEADER);
      }
      break;
    case EMF_REALTIME:
      if (container.isEnabled(EMF_SENSOR)) {
        bitmap.handleRealtimeUpdate(emf, 240, container.getProcessedChecks(), VSCREEN_OFFSET_HEADER);
      }
      break;
  }
}


void isEvent()
{
  vfield field = {};
  Buffer buffer = {};
  vsensor index;
  vstatus limit = JAUNE;
  vstatus status = GRIS;
  for (int i = 0; i < VSENSOR_COUNT; i++) {
    vfield value = container.getField((vsensor)i);
    if (value.status > status) { // && (vsensor)i != RUN_CYCLES) {
      status = value.status;
      field = value;
      buffer = container.getBuffer((vsensor)i);
      index = (vsensor)i;
    }
  }
  if (status >= limit) {
    container.setCurrentEvent(index);
    if (container.getCurrentPage() == HOME_PAGE) {
      bitmap.handleEventUpdate(index, esp.getClockWatch(), field, buffer, COLOR_GREY_DEEPDARK, VSCREEN_OFFSET_EVENT);
    }
    track.led(status);
  } else {
    track.led(false);
  }
}


void test()
{
  tft.show("test");

  // -----------------------------------------------------
  // Test : Led

  if (container.isEnabled(MICRO_CONTROLLER)) {
    esp.led(100, 0, 3000);
    Serial.println("Led: check esp blue led");
  }

  // -----------------------------------------------------
  // Test : Pointer

  if (container.isEnabled(POINTER)) {
    Serial.println("Pointer: check led colors and screen cursor");
    track.led(COLOR_GREEN, 0); Serial.print("- green");
    delay(500);
    track.led(COLOR_YELLOW, 0); Serial.print(" yellow");
    delay(500);
    track.led(COLOR_ORANGE, 0); Serial.print(" orange");
    delay(500);
    track.led(COLOR_RED, 0); Serial.print(" red");
    delay(500);
    track.led(COLOR_VIOLET, 0); Serial.println(" violet");
    delay(500);
    track.led(COLOR_BLACK, 0);
  }

  // -----------------------------------------------------
  // Test : TFT

  if (container.isEnabled(TFT_SD_SCREEN)) {
    Serial.println("Screen: check diplay, fade out when program loops");
  }

  // -----------------------------------------------------
  // Test : SD

  if (container.isEnabled(TFT_SD_SCREEN)) {
    //tft.listFiles();
  }

  // -----------------------------------------------------
  // Test : EEPROM & PSRAM

  if (container.isEnabled(MICRO_CONTROLLER)) {
    //esp.getEepromTest(); // warning destroys rom in using it, keep commented
    esp.getPsramTest();
  }

  // -----------------------------------------------------
  // Test : Realtime sensors calibration

  Serial.println("Calibration:");
  Serial.print("- EMF zero: ");
  calibrate(EMF_LEVEL);
  Serial.print("- Hall zero: ");
  calibrate(GAUSS_LEVEL);
  Serial.print("- Sound zero: ");
  calibrate(EAR_LEVEL);

  // -----------------------------------------------------
  // Test : Buzzer

  if (container.isEnabled(TFT_SD_SCREEN)) {
    Serial.println("Buzzer: beep once");
    buzzer.open();
  }
}


void calibrate(vsensor code)
{
  int  value = 0;
  int  total = 0;
  int  length = 100;

  for (int i = 0; i < length; i++) {
    switch (code) {
      case EMF_LEVEL:
        value = emf.raw();
        break;
      case GAUSS_LEVEL:
        value = hall.raw();
        break;
      case EAR_LEVEL:
        value = ear.raw();
        break;
      default:

        return;
    }
    total += value;
    delay(1);
  }

  Serial.println(String(total / (float)length));
}







// ***********************************************************************************************************************

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");

  initialize();
  //test();
  buzzer.warning();
  Serial.println("OK");

  // draw first page
  onClick(HOME_PAGE);
}


void loop()
{
  if (server.run()) {
    //Serial.println("+Webserver: " + String(server.getProcessedTime()) + "ms");
  }
  if (container.run()) {
    onUpdate();
  }
  onCheck();
}
