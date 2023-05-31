#ifdef LED
#include "LED.h"
#endif

#include "Arduino.h"
#include "Audio.h"

//#define I2S_DOUT      25
//#define I2S_BCLK      27
//#define I2S_LRC       26
#define I2S_DOUT      17
#define I2S_BCLK      33
#define I2S_LRC       18

Audio audio;
//Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN);
void setupPlayer() {
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(1); // 0...21
}

void ttsThis(String iptText) {
  audio.connecttospeech( iptText.c_str(), CN?"zh-CN":"en");
  while ( audio.isRunning()) {
    audio.loop();
#ifdef LED
    loopLed();
#endif
  }
}
void audio_info(const char *info) {
  Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info) { //id3 metadata
  Serial.print("id3data     "); Serial.println(info);
}
void audio_eof_mp3(const char *info) { //end of file
  Serial.print("eof_mp3     "); Serial.println(info);
}
void audio_eof_speech(const char *info) {
  Serial.print("eof_speech  "); Serial.println(info);
}
