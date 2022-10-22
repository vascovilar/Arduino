#include "VComWifi.h"
#include "VComWebApi.h"
#include "VGearSdCard.h"
#include "VSensorCam.h"
#include "driver/rtc_io.h"

VComWifi net;
VComWebApi api;
VGearSdCard card;
VSensorCam cam;

int LAPSE_TIME = 30;

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

// Suppression d'un fichier
void handleDelete(int id) 
{
  card.remove(getPath(id));
}


void setup(void) 
{
  Serial.begin(115200);
  card.begin();
  cam.begin();

  if (bootCount == 0) {
    // first sync date
    digitalWrite(33, LOW);
    net.begin();
    net.disconnect();
  }
  
  // warmup auto balance
  Serial.println("Waiting for cam auto balance warmup");
  delay(10000);
  
  // take picture
  handleCapture(bootCount);
  delay(2000);
  
  bootCount++;

  // turn off flash while sleeping
  digitalWrite(GPIO_NUM_4, LOW);
  rtc_gpio_hold_en(GPIO_NUM_4);

  // deep sleep  
  int time = millis();
  Serial.println("Set ESP32 to sleep ...@" + String(time) + "ms\n");
  esp_sleep_enable_timer_wakeup(((LAPSE_TIME * 1000) - time)*1000);
  esp_deep_sleep_start();
}
 
void loop(void) {
  // noop
}
