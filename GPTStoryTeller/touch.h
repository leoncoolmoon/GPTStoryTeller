#include <Arduino.h>

int threshold = 40;
bool touchActive = false;
bool lastTouchActive = false;
bool testingLower = true;

void gotTouchEvent() {
  if (lastTouchActive != testingLower) {
    touchActive = !touchActive;
    testingLower = !testingLower;
    // Touch ISR will be inverted: Lower <--> Higher than the Threshold after ISR event is noticed
    touchInterruptSetThresholdDirection(testingLower);
  }
}

void setupKey() {
  touchAttachInterrupt(T1, gotTouchEvent, threshold);
  touchInterruptSetThresholdDirection(testingLower);
}

bool loopKey() {
  if (lastTouchActive != touchActive) {
    lastTouchActive = touchActive;
    if (touchActive) {
      Serial.println("  ---- Touch was Pressed");
    } else {
      Serial.println("  ---- Touch was Released");
return true;
    }
  }
return false;
}
