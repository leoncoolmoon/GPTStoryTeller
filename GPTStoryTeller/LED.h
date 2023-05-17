#include <Arduino.h>

const int ledPin0 = 2 ;
const int ledPin1 = 4 ;
static float led_brightness = 5.0;
const float changeRate = 0.1;
boolean fading = false;
static float k = 0.0;
static float j = 0.0;

void setupLed(){
  pinMode(ledPin0, OUTPUT);
  pinMode(ledPin1, OUTPUT);  
  }

void loopLed() {
  led_brightness = led_brightness + (fading ? (-1.0) : (+1.0))*changeRate;
  fading = led_brightness >= 255.0 ? true : led_brightness <= 0.0 ? false : fading;
  k = led_brightness > 128.0 ? led_brightness - 128.0 : 0.0; //128-255
  j = (176.0 > led_brightness) && (led_brightness > 48.0) ? led_brightness > 112.0 ? 176.0 - led_brightness : led_brightness - 48.0 : 0.0; //64-192
  analogWrite(ledPin0, round(led1 ? k : 0.0));
  analogWrite(ledPin1, round(led2 ? j * 2.0 : 0.0));
}
