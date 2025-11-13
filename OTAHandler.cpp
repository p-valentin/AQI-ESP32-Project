#include "OTAHandler.h"
#include <ArduinoOTA.h>

void setupOTA() {
  ArduinoOTA.setHostname("my-aqi-monitor");
  ArduinoOTA.begin();
  Serial.println("OTA ready!");
}

void handleOTA() {
  ArduinoOTA.handle();
}
