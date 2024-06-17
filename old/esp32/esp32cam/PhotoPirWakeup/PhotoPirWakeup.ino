/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-cam-pir-motion-detector-photo-capture/
*/
 
#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"     // RTC wake up
#include "EEPROM.h"            // read and write from flash memory

// define the number of bytes you want to access
#define EEPROM_SIZE 1
// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
 
RTC_DATA_ATTR int bootCount = 0;
  
void setup() 
{
  // Global config
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("************************************");
  Serial.println("Boot number " + String(bootCount++));

  // Init PIR sensor
  if (bootCount == 1) {
    Serial.print("Starting IR sensor. Waiting for warmup... ");
    delay(60000);
    Serial.println("OK");
  }
  
  // Flash LED config
  pinMode(4, INPUT);
  digitalWrite(4, LOW);
  rtc_gpio_hold_dis(GPIO_NUM_4);
   
  Serial.println("Starting Camera");
  // Cam config
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  // Set sensor
  sensor_t * s = esp_camera_sensor_get();
  s->set_vflip(s, 1); 
  s->set_hmirror(s, 1); 
  //s->set_gainceiling(s, (gainceiling_t)1);
  s->set_lenc(s, 0);
  
/*  
  // {"framesize":5,"quality":10,"brightness":0,"contrast":0,"saturation":0,"sharpness":0,"denoise":0,"special_effect":0,
  // "wb_mode":0,"awb":1,"awb_gain":1,"aec":1,"aec2":0,"ae_level":0,"aec_value":168,"agc":1,"agc_gain":0,"gainceiling":0,
  // "bpc":0,"wpc":1,"raw_gma":1,"lenc":1,"vflip":0,"hmirror":0,"dcw":1,"colorbar":0,"face_detect":0,"face_enroll":0,"face_recognize":0}
  s->set_brightness(s, 0);     // brightness -2 to 2
  s->set_contrast(s, 0);       // contrast -2 to 2
  s->set_saturation(s, 0);     // saturation -2 to 2
  s->set_sharpness(s, 0);      // sharpness
  s->set_special_effect(s, 0); // special_effect 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_wb_mode(s, 0);        // wb_mode 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_whitebal(s, 1);       // awb 0 = disable , 1 = enable
  s->set_awb_gain(s, 1);       // awb_gain 0 = disable , 1 = enable
  s->set_exposure_ctrl(s, 1);  // aec 0 = disable , 1 = enable
  s->set_aec2(s, 0);           // aec2 0 = disable , 1 = enable
  s->set_ae_level(s, 0);       // ae_level -2 to 2
  s->set_aec_value(s, 168);    // aec_value 0 to 1200
  s->set_gain_ctrl(s, 1);      // agc 0 = disable , 1 = enable
  s->set_agc_gain(s, 0);       // agc_gain 0 to 30
  s->set_gainceiling(s, 0);    //gainceiling 0 to 6
  s->set_bpc(s, 0);            // bpc 0 = disable , 1 = enable
  s->set_wpc(s, 1);            // wpc 0 = disable , 1 = enable no deadpix
  s->set_raw_gma(s, 1);        // raw gma 0 = disable , 1 = enable
  s->set_lenc(s, 1);           // lenc 0 = disable , 1 = enable
  s->set_vflip(s, 1);          // vflip 0 = disable , 1 = enable 
  s->set_hmirror(s, 0);        // hmiror 0 = disable , 1 = enable
  s->set_dcw(s, 1);            // dcw 0 = disable , 1 = enable
*/
  delay(500);
  
  Serial.println("Starting SD Card");
  // Init SD 
  if(!SD_MMC.begin()){
    Serial.println("SD Card Mount Failed");
    //return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    return;
  }
   
  //////////////////////////////////////////////////////////////////////////////////////////
  
  // Take Picture
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  int pictureNumber = EEPROM.read(0) + 1;
 
  // Path where new picture will be saved in SD Card
  String path = "/picture" + String(pictureNumber) +".jpg";
  Serial.printf("Picture file name: %s\n", path.c_str());
 
  // Save in SD card
  fs::FS &fs = SD_MMC;
  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file in writing mode");
  } else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Saved file to path: %s\n", path.c_str());
    EEPROM.write(0, pictureNumber);
    EEPROM.commit();
  }
  file.close();
  esp_camera_fb_return(fb);
  
  delay(1000);
  
  // Turns off flash LED
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  rtc_gpio_hold_en(GPIO_NUM_4);

  // Set sleep GPIO
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, HIGH);
  Serial.println("Going to sleep now");
  Serial.println("");
  esp_deep_sleep_start();
} 
 
void loop() 
{
 
}

