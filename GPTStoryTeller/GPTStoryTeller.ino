//use esp32audioI2S I2S speech pin 26->esp32s2 pin 17
#ifdef ESP32
#include "0key.h"
extern bool led1 = true;
extern bool led2 = true;
extern bool play = false;
extern bool finish = false;
extern bool CN = false;
#include "OTA.h"
#include "wifiSetup.h"
#include "key.h"
#include "getStory.h"



void setup()
{
  Serial.begin(115200);
  wifi_STA_Steup();
  setupKey();
  setupOTA();
  setupPlayer();
#ifdef LED
  setupLed();
#endif
  requestStory();
}


void loop()
{
  loopOTA();
}
#endif
