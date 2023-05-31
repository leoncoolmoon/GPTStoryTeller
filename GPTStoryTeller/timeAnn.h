#include <Arduino.h>
#include "time.h"
#include "esp_sntp.h"
#include "playStory.h"
#define TZName    "EST-10"    // https://remotemonitoringsystems.ca/time-zone-abbreviations.php

uint32_t sec1 = millis();
String time_s = "";
char chbuf[200];
int timeIdx = 0;

char strftime_buf[64];
struct tm timeinfo;
time_t now;
boolean obtain_time(bool cn) {
  time_t now = 0;
  int retry = 0;
  Serial.println("Initializing SNTP");
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "pool.ntp.org");
  sntp_init();

  const int retry_count = 10;
  while (timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
    Serial.printf("Waiting for system time to be set... (%d/%d)\n", retry, retry_count);
    vTaskDelay(uint16_t(2000 / portTICK_PERIOD_MS));
    time(&now);
    localtime_r(&now, &timeinfo);
  }
  setenv("TZ", TZName, 1);
  tzset();
  localtime_r(&now, &timeinfo);
  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
  if (retry < retry_count) {
    sprintf(strftime_buf, "%02d:%02d",  timeinfo.tm_hour, timeinfo.tm_min);

    if (cn) {
      ttsThis(String("现在时间") + strftime_buf);
    } else {
      ttsThis(String("the current time is ") + strftime_buf);
    }
    
    return true;
  }
  else {
    return false;
  }
}
