#include "VSensorCam.h"

void VSensorCam::begin()
{
  Serial.print("Initialize Camera ...");

  // Onboard flash LED configuration
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);

  // Onborad red LED configuration
  pinMode(33, OUTPUT);
  digitalWrite(33, HIGH);

  // Pin configuration for AI Thinker - ESP32-CAM
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;  //YUV422|GRAYSCALE|RGB565|JPEG
  config.frame_size = FRAMESIZE_UXGA;    // QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA    1600x1200
  config.jpeg_quality = 5;               // 0-63 ; plus bas = meilleure qualité
  config.fb_count = 2;                   // nombre de frame buffers

  // Camera initialisation
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Cam init failed, error 0x%x", err);
    return;
  }

  // image quality
  sensor_t * s = esp_camera_sensor_get();
  s->set_brightness(s, 0);     // -2 to 2
  s->set_contrast(s, 0);       // -2 to 2
  s->set_saturation(s, 0);     // -2 to 2
  s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
  s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
  s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
  s->set_aec2(s, 1);           // 0 = disable , 1 = enable
  s->set_ae_level(s, 0);       // -2 to 2
  s->set_aec_value(s, 300);    // 0 to 1200
  s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
  s->set_agc_gain(s, 5);       // 0 to 30
  s->set_gainceiling(s, (gainceiling_t)1);  // 0 to 6
  s->set_bpc(s, 0);            // 0 = disable , 1 = enable
  s->set_wpc(s, 1);            // 0 = disable , 1 = enable
  s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
  s->set_lenc(s, 0);           // 0 = disable , 1 = enable
  s->set_hmirror(s, 1);        // 0 = disable , 1 = enable
  s->set_vflip(s, 1);          // 0 = disable , 1 = enable
  s->set_dcw(s, 0);            // 0 = disable , 1 = enable
  s->set_colorbar(s, 0);       // 0 = disable , 1 = enable

  Serial.println("OK");
}

camera_fb_t * VSensorCam::open(bool flash)
{
  digitalWrite(33, LOW);
  
  if (flash) {
  	digitalWrite(4, HIGH);
  	delay(200);
  }

  camera_fb_t * fb = NULL;

  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Capture failed");
  }

  if (flash) {
    digitalWrite(4, LOW);
  }

  return fb;
}

void VSensorCam::close(camera_fb_t * fb)
{
  // needs to redirect buffer pointer to sensor
  esp_camera_fb_return(fb);
  // red LED off
  digitalWrite(33, HIGH);
}