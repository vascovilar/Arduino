#include "VGearWifi.h"
#include "VGearWebApi.h"
#include "VGearSdCard.h"
#include "VSensorCam.h"
#include "driver/rtc_io.h"

VGearWifi net;
VGearWebApi api;
VGearSdCard card;
VSensorCam cam;

int LAPSE_TIME = 60;

// Boot count
RTC_DATA_ATTR int bootCount = 0;

// Chemin des fichiers
String getPath(int id)
{
  return "/" + String(id) + ".jpg";
}

// Prise photo et création du fichier jpeg
void handleCapture(int id) 
{
  camera_fb_t * fb = cam.open(false);
  card.save(getPath(id), fb->buf, fb->len);
  cam.close(fb);
}


void setup(void) 
{
  Serial.begin(115200);
  delay(1000);

  cam.begin();
  card.begin();

  if (!card.status())
  {
    // infinite deep sleep : flash on
    esp_deep_sleep_start();
    return;
  }

  if (cam.status())
  { 
    if (bootCount == 0) {
      // first sync date: red light on
      digitalWrite(33, LOW);
      net.begin();
      net.disconnect();
    } else {
      // warmup auto balance
      Serial.println("Waiting for cam auto balance warmup");
      delay(3000);
      // take picture : red light pulse
      handleCapture(bootCount);
    }

    bootCount++;

    // turn off flash while sleeping
    digitalWrite(GPIO_NUM_4, LOW);
    rtc_gpio_hold_en(GPIO_NUM_4);
  }

  // deep sleep : flash on = error 
  delay(2000);
  int time = millis();
  Serial.println("Set ESP32 to sleep ...@" + String(time) + "ms\n");
  esp_sleep_enable_timer_wakeup(((LAPSE_TIME * 1000) - time)*1000);
  esp_deep_sleep_start();
}
 
void loop(void) {
  // noop
}
