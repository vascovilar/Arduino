#include "Arduino.h"
#include "VData.h"
#include "VDataBuffer.h"
#include "VDataHtml.h"
#include "VDataWebServer.h"
#include "VDevice.h"
#include "VDeviceESP32.h"
#include "VDeviceBuzzer.h"
#include "VDeviceST7789SD.h"
#include "VSensor.h"
#include "VSensorEMF.h"
#include "VSensorLTR390.h"
#include "VSensorSEN0487.h"
#include "VSensorBME680.h"
#include "VSensorPA1010D.h"
#include "VRunSequencer.h"

VDeviceESP32      esp(2);
VDeviceBuzzer     buzzer(4);
VDeviceST7789SD   tft(5, 25, 33, 32);
VSensorEMF        emf(36);
VSensorLTR390     light(0x53);
VSensorSEN0487    ear(34);
VSensorBME680     air(0x77);
VSensorPA1010D    gps(0x10);

VDataBuffer       graph[VSENSOR_COUNT];
VDataWebServer    server;
VDataHtml         html;

VRunSequencer     deviceESP(esp);
VRunSequencer     deviceBuzzer(buzzer);
VRunSequencer     deviceTFT(tft);
VRunSequencer     deviceEMF(emf);
VRunSequencer     deviceLight(light);
VRunSequencer     deviceEar(ear);
VRunSequencer     deviceAir(air);
VRunSequencer     deviceGPS(gps);

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
  
  Serial.println("Devices: initialized");

  for (int field = 0; field < VSENSOR_COUNT; field++) {
    graph[field] = VDataBuffer();
  }

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
    Serial.println("Wifi: sync dateTime from web, " + esp.getDateTime());
  }

  //if (esp.disconnectWifi()) {
  //  esp.onboardLed(false);
  //}

  // -----------------------------------------------------
  // webserver
  // -----------------------------------------------------

  /**/
  server.onHtml("/", [](){ return html.handleHomePage(3000); });
  for (int field = 0; field < VSENSOR_COUNT; field++) {
    server.onHtml("/sensor/" + String(field) + ".svg", [field](){ return html.handleHistorySvgGraph(getSensor((vsensor_code) field), graph[field]); });
  }
  //server.onHtml("/sensors", [](){ return getAllSensorsTable(); });
  //server.onHtml("/logger", [](){ return esp.getDateTime() + "  " + logger.dump(SHORT_REFRESH_RATE); });
  if (server.begin(HIGH_REFRESH)) {
    Serial.println("Webserver: listening on port 80, go http://" + esp.getIP());
  }



  // -----------------------------------------------------
  // tft
  // -----------------------------------------------------
  
  tft.light(255);
  tft.title("Bonjour !", 0, 0, COLOR_WHITE);
  tft.text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", 0, 20, COLOR_GREY);
  tft.light(0);
  
  
  // -----------------------------------------------------
  // sd
  // -----------------------------------------------------

  //tft.listFiles();

  // -----------------------------------------------------
  // buzzer
  // -----------------------------------------------------
  
  buzzer.warning();
  

  // -----------------------------------------------------
  // EEPROM & PSRAM
  // -----------------------------------------------------
  
  //esp.getEepromTest(); // warning destroys rom in using it, keep commented
  esp.getPsramTest();

  // end
  Serial.println("Tests OK\n");
}



int a = 0;

void loop() 
{
  // TODO vasco continue testing magnitude
  /*a++; if (a > 25500) a = 25500;
  tft.light(a);
  Serial.println((int) (a/100));

  if (server.run()) {
    // TODO vasco on client call
  }*/
  
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
    Serial.println("+GPS: " + String(deviceGPS.getProcessedTime()) + "ms (" + String(deviceEMF.getProcessedChecks()) + ")");
  }
}





vfield_data getSensor(vsensor_code code)
{
  vfield_data data = {};

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
  }

  return data;
}

void setBuffer(vsensor_code code)
{
  vfield_data sensor = getSensor(code);
  if (sensor.status > GRIS) {
    graph[(int) code].push(sensor.value, esp.getTimeStamp());
  }
}


