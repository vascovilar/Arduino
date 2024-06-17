
// TEST PYR HC-SR501 wake up
// ESP32-CAM


// RTC 8k mem
RTC_DATA_ATTR int bootCount = 0;


void doWakeup()
{
  Serial.print("Wakeup number: " + String(bootCount) + ", ");
  esp_sleep_wakeup_cause_t wakeupReason = esp_sleep_get_wakeup_cause();  
  switch(wakeupReason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("caused by ULP program"); break;
    default : Serial.printf("was not caused by deep sleep: %d\n",wakeupReason); break;
  }
}

void redLight(int state)
{
  if (state == 1) {
    digitalWrite(GPIO_NUM_33, HIGH);  
  } else {
    digitalWrite(GPIO_NUM_33, LOW);
  }  
}

void getSensor()
{
  int sensorValue = digitalRead(GPIO_NUM_13);
  Serial.println(sensorValue, DEC);  
}

void doSleep()
{
  Serial.println("Going to sleep now");
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, HIGH);
  delay(1000);
  esp_deep_sleep_start();
}

void setup() 
{
  pinMode(GPIO_NUM_33, OUTPUT);
  pinMode(GPIO_NUM_13, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("********************************************************");

  ++bootCount;
  
  redLight(1);
  
  doWakeup();
  
  // init PYR by waiting
  Serial.print("Init sensor... ");
  if (bootCount == 1) {
    delay(60*1000);
  } else {
    delay(1000);
  }
  Serial.println("OK");  

  // Read sensor value
  Serial.print("Read sensor... ");
  getSensor();

  redLight(0);

  doSleep();
}

void loop() 
{
  //int sensorValue = digitalRead(inputPin);
  //Serial.println(sensorValue, DEC);
  //delay(100);
}
