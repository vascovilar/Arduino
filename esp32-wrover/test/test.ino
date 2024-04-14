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

#include "Sequencer.h"
#include "plugin/Webserver.h"
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

Sequencer     deviceESP(esp);
Sequencer     deviceBUZ(buzzer);
Sequencer     deviceTFT(tft);
Sequencer     deviceEMF(emf);
Sequencer     deviceLight(light);
Sequencer     deviceEar(ear);
Sequencer     deviceAir(air);
Sequencer     deviceGPS(gps);
Sequencer     deviceHall(hall);

Webserver     server;
Buffer        graph[VSENSOR_COUNT];
Html          html;





void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");  

  //Wire.begin(21, 22);
  //SPI.begin();

  // -----------------------------------------------------
  // devices
  // -----------------------------------------------------

  deviceESP.begin(LOW_REFRESH);
  deviceBUZ.begin(LOW_REFRESH);
  deviceTFT.begin(LOW_REFRESH);
  deviceEMF.begin(LOW_REFRESH); // TODO vasco passer les messages d'erreur ici bool begin
  deviceLight.begin(LOW_REFRESH);
  deviceEar.begin(LOW_REFRESH);
  deviceAir.begin(LOW_REFRESH);
  deviceGPS.begin(LOW_REFRESH);
  deviceHall.begin(LOW_REFRESH);
  
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
    Serial.println("KO");
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
  }
  delay(1000);
     
  // -----------------------------------------------------
  // Trackball
  // -----------------------------------------------------
  
  trackball.begin(CONTINUOUS);

  Serial.println("Trackball: check led colors and screen cursor");
  trackball.setBoundary(240, 135);
  trackball.led(COLOR_GREEN, 0); Serial.print("- green"); delay(1000);
  trackball.led(COLOR_YELLOW, 0); Serial.print(" yellow"); delay(1000);
  trackball.led(COLOR_ORANGE, 0); Serial.print(" orange"); delay(1000);
  trackball.led(COLOR_RED, 0); Serial.print(" red"); delay(1000);
  trackball.led(COLOR_VIOLET, 0); Serial.println(" violet"); delay(1000);
  trackball.led(COLOR_BLACK, 0); 
  delay(1000);
   
  // -----------------------------------------------------
  // tft
  // -----------------------------------------------------
  
  Serial.println("Screen: check diplay");
  tft.light(4095);
  tft.title("Bonjour !", 0, 0, COLOR_WHITE);
  tft.text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", 0, 20, COLOR_GREY);
  /*delay(2000);
  tft.clear();
  for (int i = 4095; i >= 0; i--) {
    tft.light(i);
  }*/
  delay(1000);
  
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

  Serial.println("Tests OK\n");
}




void loop() 
{
  if (server.run()) {
    // TODO vasco on client call
  }

  if (trackball.run()) {
    vpointer pointer = trackball.getPointer();
    tft.point(pointer.x, pointer.y, COLOR_WHITE);
    //Serial.println("+Trackball: " + String(deviceTrack.getProcessedTime()) + "ms (" + String(deviceTrack.getProcessedChecks()) + ")");
  }
  
  if (deviceESP.run()) {
    setBuffer(MEMORY_USED);
    //Serial.println("+ESP: " + String(deviceESP.getProcessedTime()) + "ms (" + String(deviceESP.getProcessedChecks()) + ")");
  }

  if (deviceBUZ.run()) {
    //Serial.println("+Buzzer: " + String(deviceBUZ.getProcessedTime()) + "ms (" + String(deviceBUZ.getProcessedChecks()) + ")");
  }

  if (deviceTFT.run()) {
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
  // TODO vasco sensor.getBuffer(value, esp.getTimeStamp())
}
