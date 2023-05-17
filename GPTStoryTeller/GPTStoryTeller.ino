//use esp32audioI2S I2S speech pin 26
#ifdef ESP32
#include "0key.h"
extern bool led1 = true;
extern bool led2 = true;

#include <ChatGPT.hpp>
#include "OTA.h"
#include "wifiSetup.h"
#ifdef TOUCH
#include "touch.h"
#else
#include "key.h"
#endif
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
#ifdef CN
 ttsThis("我来给你讲一个故事吧.");
#else
 ttsThis("I am going to tell you a story.");
#endif
  requestStory();
}


void loop()
{
  loopOTA();
  //loopPlayer();
  if (loopKey()) {
    requestStory();
  }
}
#endif
