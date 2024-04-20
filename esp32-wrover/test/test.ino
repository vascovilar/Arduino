#include "Arduino.h"

#include "ESP32X.h"
#include "BUZ001.h"
#include "ST7789SD.h"
#include "EMF001.h"
#include "LTR390.h"
#include "SEN0487.h"
#include "BME680.h"
#include "PA1010D.h"
#include "AH49E.h"
#include "PIM447.h"
#include "BMI160X.h"

#include "Container.h"
#include "run/Sequencer.h"
#include "run/Webserver.h"
#include "data/Buffer.h"
#include "data/Html.h"

ESP32X        esp(2);
BUZ001        buzzer(4);
ST7789SD      tft(5, 25, 33, 32);
EMF001        emf(36);
LTR390        light(0x53);
SEN0487       ear(34);
BME680        air(0x77);
PA1010D       gps(0x10);
AH49E         hall(35);
PIM447        trackball(0x0A);
BMI160X       imu(0x68);

// TODO vasco in container
Sequencer     deviceESP(esp);
Sequencer     deviceBUZ(buzzer);
Sequencer     deviceTFT(tft);
Sequencer     deviceEMF(emf);
Sequencer     deviceLight(light);
Sequencer     deviceEar(ear);
Sequencer     deviceAir(air);
Sequencer     deviceGPS(gps);
Sequencer     deviceHall(hall);
Sequencer     devicePointer(trackball);
Sequencer     deviceIMU(imu);
Buffer        graph[VSENSOR_COUNT];

Webserver     server;
Html          html;
Container     container;




void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");

  //Wire.begin(21, 22);
  //SPI.begin();





/*
  container.add(buzzer);COLOR_WHITE

  Serial.println("device code: " + String(container.go1()));
  Serial.println("device init: " + String((container.go2() ? "true": "false")));
  container.go3();

  //while(true);
*/





  // -----------------------------------------------------
  // Init: devices
  // -----------------------------------------------------

  // TODO vasco in container
  bool error = false;
  if (!deviceESP.begin(LOW_REFRESH)) error = true;
  if (!deviceBUZ.begin(LOW_REFRESH)) error = true;
  if (!deviceTFT.begin(LOW_REFRESH)) error = true;
  if (!deviceEMF.begin(LOW_REFRESH)) error = true;
  if (!deviceLight.begin(LOW_REFRESH)) error = true;
  if (!deviceEar.begin(LOW_REFRESH)) error = true;
  if (!deviceAir.begin(LOW_REFRESH)) error = true;
  if (!deviceGPS.begin(LOW_REFRESH)) error = true;
  if (!deviceHall.begin(LOW_REFRESH)) error = true;
  if (!devicePointer.begin(CONTINUOUS)) error = true;
  if (!deviceIMU.begin(LOW_REFRESH)) error = true;

  Serial.print("\nDevices initialization..."); 
  if (!error) {
    Serial.println("OK");
  } else {
    Serial.println("Fail !");
  }
  // TODO vasco in container
  for (int field = 0; field < VSENSOR_COUNT; field++) {
    graph[field] = Buffer();
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
    error = true;
    Serial.println("failed !");
  }
  delay(500);

  // -----------------------------------------------------
  // Init: webserver
  // -----------------------------------------------------

  Serial.print("Initializing webserver..."); 
  server.onHtml("/", [](){ return html.handleHomePage(10000); }); // TODO vasco get delay from container
  for (int field = 0; field < VSENSOR_COUNT; field++) {
    server.onHtml("/sensor/" + String(field) + ".svg", [field](){ return html.handleHistorySvgGraph(getSensor((vsensor) field), graph[field]); });
  }
  //server.onHtml("/sensors", [](){ return getAllSensorsTable(); });
  //server.onHtml("/logger", [](){ return esp.getDateTime() + "  " + logger.dump(SHORT_REFRESH_RATE); });
  if (server.begin(CONTINUOUS)) {
    Serial.println("OK");
    Serial.println("- listening port 80, go: http://" + esp.getIP());
  } else {
    error = true;
    Serial.println("failed");
  }
  delay(500);

  // -----------------------------------------------------
  // Test: Led
  // -----------------------------------------------------

  if (deviceESP.isEnabled()) {
    esp.led(true);
    Serial.println("Led: check esp blue led");
    delay(500);
  }

  // -----------------------------------------------------
  // Test: Trackball
  // -----------------------------------------------------

  if (devicePointer.isEnabled()) {
    Serial.println("Trackball: check led colors and screen cursor");
    trackball.setBoundary(240, 135);
    trackball.led(COLOR_GREEN, 0); Serial.print("- green"); delay(1000);
    trackball.led(COLOR_YELLOW, 0); Serial.print(" yellow"); delay(1000);
    trackball.led(COLOR_ORANGE, 0); Serial.print(" orange"); delay(1000);
    trackball.led(COLOR_RED, 0); Serial.print(" red"); delay(1000);
    trackball.led(COLOR_VIOLET, 0); Serial.println(" violet"); delay(1000);
    trackball.led(COLOR_BLACK, 0);
    delay(500);
  }

  // -----------------------------------------------------
  // Test: TFT
  // -----------------------------------------------------

  if (deviceTFT.isEnabled()) {
    Serial.println("Screen: check diplay, fade out when program loops");
    tft.clear();
    tft.led(4095);
    tft.title("IOT #3", 0, 0, COLOR_GREY);
    tft.text("http://" + esp.getIP(), 0, 17, COLOR_WHITE);
    tft.text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat.", 0, 30, COLOR_GREY);
    delay(500);
  }

  // -----------------------------------------------------
  // Test: SD
  // -----------------------------------------------------

  //tft.listFiles();

  // -----------------------------------------------------
  // Test: EEPROM & PSRAM
  // -----------------------------------------------------

  //esp.getEepromTest(); // warning destroys rom in using it, keep commented
  esp.getPsramTest();
  delay(500);

  // -----------------------------------------------------
  // Test: realtime sensors calibration
  // -----------------------------------------------------

  Serial.println("Calibration:");
  Serial.print("- EMF zero: ");
  calibrate(EMF_LEVEL);
  Serial.print("- Hall zero: ");
  calibrate(GAUSS_LEVEL);
  Serial.print("- Sound zero: ");
  calibrate(EAR_LEVEL);
  Serial.print("- Light zero: ");
  calibrate(VISIBLE);
  delay(500);

  // -----------------------------------------------------
  // Test: buzzer
  // -----------------------------------------------------

  Serial.println("Buzzer: beep once\n");
  buzzer.warning();
  delay(500);

  // -----------------------------------------------------
  esp.led(4095, 0, 5000);
  tft.led(4095, 0, 5000);
}




void loop()
{
  // TODO vasco in container

  if (server.run()) {
    //Serial.println("+Webserver: " + String(server.getProcessedTime()) + "ms"int);
  }

  if (deviceBUZ.run()) {
    //Serial.println("+Buzzer: " + String(deviceBUZ.getProcessedTime()) + "ms (" + String(deviceBUZ.getProcessedChecks()) + ")");
  }

  if (deviceTFT.run()) {
    //Serial.println("+TFT: " + String(deviceTFT.getProcessedTime()) + "ms (" + String(deviceTFT.getProcessedChecks()) + ")");
  }

  if (devicePointer.run()) {
    // write pixel in screen
    vpointer pointer = trackball.getPointer();
    tft.point(pointer.x, pointer.y, COLOR_WHITE);
    // screen saver
    tft.led(4095, 0, 20000);
    //Serial.println("+Trackball: " + String(devicePointer.getProcessedTime()) + "ms (" + String(devicePointer.getProcessedChecks()) + ")");
  }

  if (deviceEMF.run()) {
    setBuffer(EMF_LEVEL);
    //Serial.println("+EMF: " + String(deviceEMF.getProcessedTime()) + "ms (" + String(deviceEMF.getProcessedChecks()) + ")");
  }

  if (deviceLight.run()) {
    setBuffer(UV_INDEX);
    setBuffer(VISIBLE);
    //Serial.println("UVS:" + String(light.getUvIndex().value) + " ALS:" + String(light.getVisible().value));
    //Serial.println("+Light: " + String(deviceLight.getProcessedTime()) + "ms (" + String(deviceLight.getProcessedChecks()) + ")");
  }

  if (deviceEar.run()) {
    setBuffer(EAR_LEVEL);
    //Serial.println("+Ear: " + String(deviceEar.getProcessedTime()) + "ms (" + String(deviceEar.getProcessedChecks()) + ")");
  }

  if (deviceAir.run()) {
    setBuffer(TEMPERATURE);
    setBuffer(PRESSURE);
    setBuffer(HUMIDITY);
    setBuffer(AIR_QUALITY);
    setBuffer(GAS_PERCENTAGE);
    //Serial.println("+Air: " + String(deviceAir.getProcessedTime()) + "ms (" + String(deviceAir.getProcessedChecks()) + ")");
  }

  if (deviceGPS.run()) {
    setBuffer(ALTITUDE);
    /*
    tft.clear();
    tft.title("GPS", 0, 0, COLOR_GREY);
    tft.text(String(gps.getDateTime()), 0, 20, COLOR_WHITE);
    tft.text("sat: " + String(gps.getSatellite().value), 0, 30, COLOR_WHITE);
    tft.text("fix: " + String(gps.getFixQuality().text), 0, 40, COLOR_WHITE);
    tft.text("lat: " + String(gps.getLatitude(), 7), 0, 60, COLOR_WHITE);
    tft.text("lon: " + String(gps.getLongitude(), 7), 0, 50, COLOR_WHITE);
    */
    //Serial.println("+GPS: " + String(deviceGPS.getProcessedTime()) + "ms (" + String(deviceGPS.getProcessedChecks()) + ")");
  }

  if (deviceHall.run()) {
    setBuffer(GAUSS_LEVEL);
    //Serial.println("+Hall: " + String(deviceHall.getProcessedTime()) + "ms (" + String(deviceHall.getProcessedChecks()) + ")");
  }

  if (deviceIMU.run()) {
    /*
    vcoord coord = imu.getGyroscope();
    Serial.print(String(coord.x) + ", " + String(coord.y) + ", " + String(coord.z) + "    ");
    */
    //Serial.println("+IMU: " + String(deviceIMU.getProcessedTime()) + "ms (" + String(deviceIMU.getProcessedChecks()) + ")");
  }

  if (deviceESP.run()) {
    setBuffer(MEMORY_USED);
    setBuffer(RUN_CYCLES); // TODO vasco get data from global Container
    //Serial.println("+ESP: " + String(deviceESP.getProcessedTime()) + "ms (" + String(deviceESP.getProcessedChecks()) + ")");

    /*
    // EVENT manager to do: set color on trackball if one status > JAUNE
    vstatus limit = GRIS;
    for (int i = 0; i < VSENSOR_COUNT; i++) {
      vfield value = getSensor((vsensor) i);
      if ((int) value.status > limit) limit = value.status;
    }
    if (limit > JAUNE) {
      trackball.led(limit);
    } else {
      trackball.led(0, 0);
    }
    */
  }
}




// TODO vasco in container

vfield getSensor(vsensor code)
{
  vfield data = {};

  switch (code) {
    case TEMPERATURE:
      data = air.getTemperature();
      break;
    case PRESSURE:
      data = air.getPressure();
      break;
    case HUMIDITY:
      data = air.getHumidity();
      break;
    case AIR_QUALITY:
      data = air.getAirQuality();
      break;
    case GAS_PERCENTAGE:
      data = air.getGasPercentage();
      break;
    case EMF_LEVEL:
      data = emf.getMaxValue();
      break;
    case UV_INDEX:
      data = light.getUvIndex();
      break;
    case VISIBLE:
      data = light.getVisible();
      break;
    case EAR_LEVEL:
      data = ear.getMaxValue();
      break;
    case ALTITUDE:
      data = gps.getAltitude();
      break;
    case MEMORY_USED:
      data = esp.getMemoryUsed();
      break;
    case RUN_CYCLES:
      // TODO vasco get data from global Container
      data = (vfield) {"Cycles programme", "/s", 10.0, (float)deviceESP.getProcessedChecks() / (float)(deviceESP.getCurrentDelay() / 1000), VERT};
      break;
    case GAUSS_LEVEL:
      data = hall.getMaxValue();
      break;
  }

  return data;
}

// TODO vasco in container

void setBuffer(vsensor code)
{
  vfield sensor = getSensor(code);
  if (sensor.status > GRIS) {
    graph[(int)code].push(sensor.value, esp.getTimeStamp());
  }
}

// program subroutines

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
      case VISIBLE:
        value = light.read();
        break;
      default:

        return;
    }
    total += value;
    delay(10);
  }

  Serial.println(String(total / length));
}
