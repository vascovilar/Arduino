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

#include "run/Container.h"
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
Webserver     server;

Buffer        graph[VSENSOR_COUNT];
Html          html;
Container     container;

bool          error = false;
int           screenMagnitude = 4095; 

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");  

  //Wire.begin(21, 22);
  //SPI.begin();




/*
  container.add(buzzer);

  Serial.println("device code: " + String(container.go1()));
  Serial.println("device init: " + String((container.go2() ? "true": "false")));
  container.go3();

  //while(true); 
*/





  // -----------------------------------------------------
  // devices
  // -----------------------------------------------------

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
  if (!deviceIMU.begin(HIGH_REFRESH)) error = true;
  
  Serial.println("Devices: initialized");

  for (int field = 0; field < VSENSOR_COUNT; field++) {
    graph[field] = Buffer();
  }
  delay(1000);

  // -----------------------------------------------------
  // wifi 
  // -----------------------------------------------------
  
  //int lenght = esp.getWifiAccessPoints();
  //Serial.println("bornes: " + String(lenght));
  //for (int i = 0; i < lenght; i++) {
  //  Serial.println(esp.getWifiAccessPointInfo(i));
  //}
   
  Serial.print("Connecting to Wifi... ");
  if (esp.connectWifi()) {
    Serial.println("OK");
    Serial.println("Sync dateTime from Internet " + esp.getDateTime());
    esp.led(true);
    Serial.println("LED: check blue led");
  } else {
    error = true;
    Serial.println("failed !");
  }
  delay(1000);

  //if (esp.disconnectWifi()) {
  //  esp.led(false);
  //}

  // -----------------------------------------------------
  // webserver
  // -----------------------------------------------------

  server.onHtml("/", [](){ return html.handleHomePage(3000); });
  for (int field = 0; field < VSENSOR_COUNT; field++) {
    server.onHtml("/sensor/" + String(field) + ".svg", [field](){ return html.handleHistorySvgGraph(getSensor((vsensor) field), graph[field]); });
  }
  //server.onHtml("/sensors", [](){ return getAllSensorsTable(); });
  //server.onHtml("/logger", [](){ return esp.getDateTime() + "  " + logger.dump(SHORT_REFRESH_RATE); });
  if (server.begin(CONTINUOUS)) {
    Serial.println("Webserver: listening on port 80, go http://" + esp.getIP());
  } else {
    error = true;
    Serial.println("Webserver: can't start");
  }
  delay(1000);

  // -----------------------------------------------------
  // Trackball
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
    delay(1000);
  } 
  // -----------------------------------------------------
  // tft
  // -----------------------------------------------------
  
  if (deviceTFT.isEnabled()) {
    Serial.println("Screen: check diplay, fade out after beep");
    tft.light(screenMagnitude);
    tft.title("Bonjour !", 0, 0, COLOR_WHITE);
    tft.text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", 0, 20, COLOR_GREY);
    delay(1000);
  }
  
  // -----------------------------------------------------
  // sd
  // -----------------------------------------------------

  //tft.listFiles(); 

  // -----------------------------------------------------
  // EEPROM & PSRAM
  // -----------------------------------------------------
  
  //esp.getEepromTest(); // warning destroys rom in using it, keep commented
  esp.getPsramTest();
  delay(1000);

  // -----------------------------------------------------
  // realtime sensors calibration
  // -----------------------------------------------------

  Serial.println("Instant snapshot for calibration:");
  Serial.print("- EMF zero: ");
  calibrate(EMF_LEVEL);
  Serial.print("- Hall zero: ");
  calibrate(GAUSS_LEVEL);
  Serial.print("- Sound zero: ");
  calibrate(EAR_LEVEL);
  Serial.print("- Light zero: ");
  calibrate(VISIBLE);
  delay(1000);

  // -----------------------------------------------------
  // buzzer
  // -----------------------------------------------------
  
  Serial.println("Buzzer: listen to beep");
  buzzer.warning();
  delay(1000);

  // -----------------------------------------------------
  Serial.println("Tests " + String(error ? "failed !": "OK") + "\n");
}




void loop() 
{
  if (server.run()) {
  }

  if (devicePointer.run()) {
    vpointer pointer = trackball.getPointer();
    tft.point(pointer.x, pointer.y, COLOR_WHITE);
    //Serial.println("+Trackball: " + String(devicePointer.getProcessedTime()) + "ms (" + String(devicePointer.getProcessedChecks()) + ")");
  }
  
  if (deviceESP.run()) {
    setBuffer(MEMORY_USED);
    //Serial.println("+ESP: " + String(deviceESP.getProcessedTime()) + "ms (" + String(deviceESP.getProcessedChecks()) + ")");
  }

  if (deviceBUZ.run()) {
    //Serial.println("+Buzzer: " + String(deviceBUZ.getProcessedTime()) + "ms (" + String(deviceBUZ.getProcessedChecks()) + ")");
  }

  if (deviceTFT.run()) {
    if (screenMagnitude > 0) {
      screenMagnitude = screenMagnitude - 900;
      if (screenMagnitude < 0) screenMagnitude = 0;
      tft.light(screenMagnitude);
      Serial.println("Decreasing screen backlight (" + String(screenMagnitude) + ")");
    }
    //Serial.println("+TFT: " + String(deviceTFT.getProcessedTime()) + "ms (" + String(deviceTFT.getProcessedChecks()) + ")");
  }

  if (deviceEMF.run()) {
    setBuffer(EMF_LEVEL);
    //Serial.println("+EMF: " + String(deviceEMF.getProcessedTime()) + "ms (" + String(deviceEMF.getProcessedChecks()) + ")");
  }

  if (deviceLight.run()) {
    setBuffer(UV_INDEX);
    setBuffer(VISIBLE);
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

    // set color on trackball if one status > JAUNE
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
  }

  if (deviceGPS.run()) {
    setBuffer(ALTITUDE);
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
}


// program subroutines

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
    case GAUSS_LEVEL:
      data = hall.getMaxValue();
      break;   
  }

  return data;
}

void setBuffer(vsensor code)
{
  vfield sensor = getSensor(code);
  if (sensor.status > GRIS) {
    graph[(int) code].push(sensor.value, esp.getTimeStamp());
  }
}

void calibrate(vsensor code)
{
  long value = 0;
  long total = 0;
  
  for (int i = 0; i < 100; i++) {
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
    }
    value = value * 1000;
    value = graph[0].smoothe(value, 7);
    value = graph[0].maximume(value, 10); 
    value = graph[0].inertiae(value,10);
    total += value / 1000;
    delay(10);
  }

  Serial.println(String(total / 100));
}
