/*
 * Display debug info on serial monitor
 * Implementation:
 *
 *   #include <VSensorCam.h>
 *
 *   VSensorCam out();
 *
 *   void setup() {
 *     out.begin();
 *   }
 *   void loop() {
 *     out.println(random(10,100));
 *     delay(1000);
 *   }
 */

#ifndef VSensorCam_h
#define VSensorCam_h

#include "Arduino.h"
#include "esp_camera.h"
#include "esp_http_server.h"

// ESP32-CAM OV2640 Camera
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

// Stream over web
static const char* STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=123456789000000000000987654321";
static const char* STREAM_BOUNDARY = "\r\n--123456789000000000000987654321\r\n";
static const char* STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

class VSensorCam
{  
  httpd_handle_t stream_httpd = NULL;
  
  public:
    void begin();
    void startStreamServer();
    esp_err_t stream_handler(httpd_req_t *req);
};

#endif
