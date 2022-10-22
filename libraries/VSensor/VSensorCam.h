/*
 * Read data on OV2640 with AI Thinker - ESP32-CAM
 * Implementation:
 *
 *   #include <VSensorCam.h>
 *
 *   VSensorCam cam; 
 *
 *   void setup() {
 *     cam.begin();
 *     cam.open();
 *   }
 *   void loop() {
 *   }
 */

#ifndef VSensorCam_h
#define VSensorCam_h

#include "Arduino.h"
#include "esp_camera.h"

class VSensorCam
{
  public:

    void  begin();
    
    camera_fb_t* open(bool flash);
    void close(camera_fb_t* fb);

  private:

};

#endif
