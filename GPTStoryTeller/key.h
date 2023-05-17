#include <Arduino.h>

const int storyKey = 2;
void setupKey() {
 pinMode(storyKey, INPUT);
}
bool loopKey() {
  if (digitalRead(storyKey) != HIGH ) {
    Serial.println(" --- storyKey pressed");
    return true;
  }
  return false;
}
