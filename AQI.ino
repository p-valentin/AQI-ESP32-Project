#include "Display.h"
#include "Sensors.h"
#include "WifiManager.h"
#include "OTAHandler.h"

unsigned long lastSensorRead = 0;
const long sensorInterval = 6000;

void setup() {
  Serial.begin(9600);
  Serial.println("Full Air Quality Monitor Test");

  setupDisplay();
  connectWifi();
  setupSensors();
  setupOTA();

  delay(2000);
}

void loop() {
  handleOTA();

  if (millis() - lastSensorRead > sensorInterval) {
    readAllSensors();
    displayAllData();
    lastSensorRead = millis();
  }
}
