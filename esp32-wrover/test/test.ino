#include "Arduino.h"
#include "ESP32X.h"
#include "Buzzer.h"
#include "ST7789SD.h"
#include "EMF.h"
#include "LTR390.h"
#include "SEN0487.h"
#include "BME680.h"
#include "PA1010D.h"
#include "AH49E.h"
#include "Sequencer.h"
#include "Webserver.h"
#include "data/Buffer.h"
#include "Html.h"

ESP32X        esp(2);
Buzzer        buzzer(4);
ST7789SD      tft(5, 25, 33, 32);
EMF           emf(36);
LTR390        light(0x53);
SEN0487       ear(34);
BME680        air(0x77);
PA1010D       gps(0x10);
AH49E         hall(35);

Sequencer     deviceESP(esp);
Sequencer     deviceBuzzer(buzzer);
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

  // -----------------------------------------------------
  // devices
  // -----------------------------------------------------

  deviceESP.begin(HIGH_REFRESH);
  deviceBuzzer.begin(HIGH_REFRESH);
  deviceTFT.begin(HIGH_REFRESH);
  deviceEMF.begin(HIGH_REFRESH); // TODO vasco passer les messages d'erreur ici bool begin
  deviceLight.begin(HIGH_REFRESH);
  deviceEar.begin(HIGH_REFRESH);
  deviceAir.begin(HIGH_REFRESH);
  deviceGPS.begin(HIGH_REFRESH);
  deviceHall.begin(HIGH_REFRESH);
  
  Serial.println("Devices: initialized");

  for (int field = 0; field < VSENSOR_COUNT; field++) {
    graph[field] = Buffer();
  }

  // -----------------------------------------------------
  // realtime sensors calibration
  // -----------------------------------------------------

  Serial.println("Sensor calibration:");
  Serial.print("- EMF zero: ");
  calibrate(EMF_LEVEL);
  Serial.print("- hall zero: ");
  calibrate(GAUSS_LEVEL);

  // -----------------------------------------------------
  // wifi 
  // -----------------------------------------------------
  
  //int lenght = esp.getWifiAccessPoints();
  //Serial.println("bornes: " + String(lenght));
  //for (int i = 0; i < lenght; i++) {
  //  Serial.println(esp.getWifiAccessPointInfo(i));
  //}
   
  if (esp.connectWifi()) {
    esp.onboardedLed(true);
    Serial.println("Onboard LED: check blue led is on");
    Serial.println("Wifi: connected, sync dateTime from web " + esp.getDateTime());
  }

  //if (esp.disconnectWifi()) {
  //  esp.onboardLed(false);
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
  if (server.begin(HIGH_REFRESH)) {
    Serial.println("Webserver: listening on port 80, go http://" + esp.getIP());
  }
    
  // -----------------------------------------------------
  // tft
  // -----------------------------------------------------
  
  Serial.println("Screen: check message diplayed");
  tft.light(4095);
  tft.title("Bonjour !", 0, 0, COLOR_WHITE);
  tft.text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", 0, 20, COLOR_GREY);
  /*delay(2000);
  tft.clear();
  for (int i = 4095; i >= 0; i--) {
    tft.light(i);
  }*/
  
  // -----------------------------------------------------
  // sd
  // -----------------------------------------------------

  //tft.listFiles(); 

  // -----------------------------------------------------
  // EEPROM & PSRAM
  // -----------------------------------------------------
  
  //esp.getEepromTest(); // warning destroys rom in using it, keep commented
  esp.getPsramTest();

  // -----------------------------------------------------
  // buzzer
  // -----------------------------------------------------
  
  Serial.println("Buzzer: listen to a simple beep");
  buzzer.warning();

  // -----------------------------------------------------

  Serial.println("Tests OK\n");
}





void loop() 
{
  if (server.run()) {
    // TODO vasco on client call
  }
  
  if (deviceESP.run()) {
    setBuffer(MEMORY_USED);
    //Serial.println("+ESP: " + String(deviceESP.getProcessedTime()) + "ms (" + String(deviceEMF.getProcessedChecks()) + ")");
  }

  if (deviceBuzzer.run()) {
    //Serial.println("+Buzzer: " + String(deviceBuzzer.getProcessedTime()) + "ms (" + String(deviceEMF.getProcessedChecks()) + ")");
  }

  if (deviceTFT.run()) {
    //Serial.println("+TFT: " + String(deviceTFT.getProcessedTime()) + "ms (" + String(deviceEMF.getProcessedChecks()) + ")");
  }

  if (deviceEMF.run()) {
    setBuffer(EMF_LEVEL);
    //Serial.println("+EMF: " + String(deviceEMF.getProcessedTime()) + "ms (" + String(deviceEMF.getProcessedChecks()) + ")");
  }

  if (deviceLight.run()) {
    setBuffer(UV_INDEX);
    setBuffer(VISIBLE);
    //Serial.println("+Light: " + String(deviceLight.getProcessedTime()) + "ms (" + String(deviceEMF.getProcessedChecks()) + ")");
  }

  if (deviceEar.run()) {
    setBuffer(EAR_LEVEL);
    //Serial.println("+Ear: " + String(deviceEar.getProcessedTime()) + "ms (" + String(deviceEMF.getProcessedChecks()) + ")");
  }

  if (deviceAir.run()) {
    setBuffer(TEMPERATURE);
    setBuffer(PRESSURE);
    setBuffer(HUMIDITY);
    setBuffer(AIR_QUALITY);
    setBuffer(GAS_PERCENTAGE);
    //Serial.println("+Air: " + String(deviceAir.getProcessedTime()) + "ms (" + String(deviceEMF.getProcessedChecks()) + ")");
  }

  if (deviceGPS.run()) {
    setBuffer(ALTITUDE);
    //Serial.println("+GPS: " + String(deviceGPS.getProcessedTime()) + "ms (" + String(deviceEMF.getProcessedChecks()) + ")");
  }

  if (deviceHall.run()) {
    setBuffer(GAUSS_LEVEL);
    //Serial.println("+Hall: " + String(deviceHall.getProcessedTime()) + "ms (" + String(deviceHall.getProcessedChecks()) + ")");
  }  
}





bool calibrate(vsensor code)
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

  if (value != 0) {
    Serial.print(String(total / 100) + "\n");

    return true;
  }
  
  return false;
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




