#include "Arduino.h"
#include "VDataBuffer.h"
#include "VDataHtml.h"
#include "VDataWebServer.h"
#include "VDevice.h"
#include "VDeviceESP32.h"
#include "VDeviceBuzzer.h"
#include "VSensor.h"
#include "VSensorEMF.h"
#include "VSensorLTR390.h"
#include "VSensorSEN0487.h"
#include "VSensorBME680.h"
#include "VSensorPA1010D.h"
#include "VSequencer.h"

/*
#include "VDeviceST7789.h"
#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789
#include "SPI.h"
*/

#define TFT_CS 5
#define TFT_RST -1  // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 4
#define SD_CS 2

VDataBuffer     graph[VSENSOR_COUNT];
VDataHtml       html;
VDataWebServer  server;

VDeviceESP32    esp(2);
VSensorEMF      emf(36);
VSensorLTR390   sun(0);
VSensorSEN0487  ear(34);
VSensorBME680   air(0);
VDeviceBuzzer   buzzer(4);

VSequencer      deviceEMF(emf);
VSequencer      deviceSun(sun);
VSequencer      deviceEar(ear);
VSequencer      deviceAir(air);

//VDeviceST7789     tft;
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

  
void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");  

  esp.init();
  buzzer.init();

  deviceEMF.begin(LOW_REFRESH);
  deviceSun.begin(LOW_REFRESH);
  deviceEar.begin(LOW_REFRESH);
  deviceAir.begin(LOW_REFRESH);
  
  Serial.println("Devices initialized");
  
  for (int field = 0; field < VSENSOR_COUNT; field++) {
    graph[field] = VDataBuffer();
  }

  if (esp.connectWifi()) {
    esp.blueLed(true);
    Serial.println("Sync date from web: " + esp.getDateTime() + " from " + esp.getDeviceIP());
  }

  server.onHtml("/", [](){ return html.handleHomePage(3000); });
  for (int field = 0; field < VSENSOR_COUNT; field++) {
    server.onHtml("/graph/" + String(field) + ".svg", [field](){ return html.handleHistorySvgGraph(graph[field], getSensor((sensor_code) field)); });
  }
  //server.onHtml("/logger", [](){ return esp.getDateTime() + "  " + logger.dump(SHORT_REFRESH_RATE); });
  //server.onHtml("/sensors", [](){ return getAllSensorsTable(); });
  server.init();
  


  /*
  int lenght = esp.getWifiAccessPoints();
  Serial.println("bornes: " + String(lenght));
  
  for (int i = 0; i < lenght; i++) {
    Serial.println(esp.getWifiAccessPointInfo(i));
  }
  */




  /*
  Serial.println("dateTime:" + esp.getDateTime());
  Serial.println("networks:" + String(esp.getWifiAccessPoints()));
  delay(2000); 

  if (esp.connectWifi()) {
    Serial.println("connected !");
  } else {
    Serial.println("fail to connect");
  }
  delay(2000);
  if (esp.disconnectWifi()) {
    Serial.println("disconnected");
  }
  Serial.println("dateTime:" + esp.getDateTime());
  */
  
  /*
  VSensorEMF emf(8);
  Sequencer deviceEMF(emf);
  deviceEMF.begin();
  deviceEMF.begin();
  */ 

  /*
  pinMode(2, OUTPUT); digitalWrite(2, HIGH);
  */

  /*
  pinMode(36, INPUT);
  analogSetWidth(12);
  analogSetPinAttenuation(36, ADC_0db);
  */

  //sound.begin(15);
  //ear.begin(39);
  //emf.begin(HIGH_REFRESH);
  //joy.begin(34, 39, 0);
  //screen.begin(0);

  /*
  tft.init(135, 240);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setTextWrap(true);
  tft.print("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ");
  */
  
  buzzer.warning();
}

void loop() 
{ 
  
  server.run();

  if (deviceEMF.update()) {
    setBuffer(EMF_LEVEL);
  }

  if (deviceSun.update()) {
    setBuffer(UV_INDEX);
    setBuffer(VISIBLE);
  }

  if (deviceEar.update()) {
    setBuffer(EAR_LEVEL);
  }

  if (deviceAir.update()) {
    setBuffer(TEMPERATURE);
    setBuffer(PRESSURE);
    setBuffer(HUMIDITY);
    setBuffer(AIR_QUALITY);
    setBuffer(GAS_PERCENTAGE);
  }
  

  //Serial.println(String(deviceEMF.read()) + "," + String(deviceSun.read()) + "," + String(deviceEar.read()));
  //Serial.println("5," + String(deviceEMF.read()) + "," + String(deviceEar.read()) + ",-5");
  //Serial.println("1," + String(deviceEar.read()) + ",-1");
  //Serial.println(analogRead(36));
  //Serial.println(((float) analogRead(36) / (float) 4096) * 100);
  //sound.warning();
  //Serial.println(ear.read());
  //Serial.println(String(deviceSun.read()));
  //Serial.println("*");
  //delay(1000);
  //if (joy.update(100)) Serial.println(String(joy.getX()) + "    " + String(joy.getY()) + "     " + String(joy.getClick()));
  //screen.text(String(millis()), 0, 0); delay(1000); screen.clear();


  //Serial.println(esp.getDateTime());
  
  /*
  int count = esp.getWifiAccessPoints();
  String list[100] = esp.getWifiAccessPointList();
  for (int i = 0; i < count; i++) {
    Serial.println(String(i) + " | " + list[i]);
  }
  
  delay(10000);
  */
}

// TODO vasco include in a sequencer array, warning: hérite de VSequencer pour pas pourrir la classe
field_data getSensor(sensor_code code)
{
  field_data data = {};

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
      data = sun.getUvIndex();
      break;
    case VISIBLE:
      data = sun.getVisible();
      break;
    case EAR_LEVEL:
      data = ear.getMaxValue();
      break;
  }

  return data;
}

void setBuffer(sensor_code code)
{
  field_data sensor = getSensor(code);
  if (sensor.status > 0) {
    graph[code].push(sensor.value);
  }
}