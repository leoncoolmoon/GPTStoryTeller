#include <Arduino.h>
#include <EEPROM.h>            // read and write from flash memory
#define EEPROM_SIZE 1
#define  storyKey 0
void changeLanguage() {
  CN = !CN;
  pinMode(15, OUTPUT);
  digitalWrite(15, CN?LOW:HIGH);
  EEPROM.write(0, CN);
  EEPROM.commit();
};
void IRAM_ATTR play_Pause_Story()
{ if (!finish) {
    play = !play;
  } else {
    changeLanguage();
  }
  Serial.println(" --- storyKey pressed");
}
void setupKey() {
  EEPROM.begin(EEPROM_SIZE);
  CN = EEPROM.read(0);
  pinMode(storyKey, INPUT_PULLUP);
  attachInterrupt(storyKey, play_Pause_Story, RISING);
  pinMode(15, OUTPUT);
  digitalWrite(15, CN?LOW:HIGH);
}


