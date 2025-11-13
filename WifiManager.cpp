#include "WifiManager.h"
#include <WiFi.h>
#include "Display.h"

const char* ssid = "Network Name";
const char* password = "Password";

void connectWifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  tft.print("Connecting to "); tft.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tft.print(".");
  }

  Serial.println("\nWi-Fi Connected!");
  Serial.print("IP: "); Serial.println(WiFi.localIP());
  tft.println("Connected!");
}
