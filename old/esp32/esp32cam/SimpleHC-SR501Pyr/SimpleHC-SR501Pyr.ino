
// TEST PYR HC-SR501

//int inputPin = D7; // ESP8266
int inputPin = GPIO_NUM_13; // ESP32-CAM


void getSensor()
{
  int sensorValue = digitalRead(inputPin);
  Serial.println(sensorValue, DEC);  
}

void setup() 
{
  pinMode(inputPin, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("********************************************************");
}

void loop() 
{
   getSensor();
   delay(100);
}
