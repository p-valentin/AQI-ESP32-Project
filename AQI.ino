#include "Display.h"
#include "Sensors.h"
#include "WifiManager.h"
#include "OTAHandler.h"
#include "TelegramBot.h"

unsigned long lastSensorRead = 0;
const long sensorInterval = 6000;

void setup() {
  Serial.begin(9600);
  Serial.println("Full Air Quality Monitor Test");

  setupDisplay();
  connectWifi();
  setupTelegram();
  setupSensors();
  setupOTA();

  delay(2000);
}

void loop() {
  handleOTA();
  checkIncomingMessages();

  if (millis() - lastSensorRead > sensorInterval) {
    readAllSensors();
    displayAllData();
    sendAlertNotification();
    lastSensorRead = millis();
  }
}
