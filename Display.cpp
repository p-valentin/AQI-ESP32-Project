#include "Display.h"
#include "Sensors.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setupDisplay() {
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(100);

  tft.init(135, 240);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
}

void checkHumidity(float humidity) {
  if (humidity > 63.0 || humidity < 35.0) {
    tft.setTextColor(ST77XX_RED);
    tft.print("!!!");
    tft.setTextColor(ST77XX_WHITE);
  }
}

void checkCO2(uint16_t co2) {
  if (co2 > 10000) {
    tft.setTextColor(ST77XX_RED);
    tft.println("!!! EXTREME CO2 LEVELS");
  } else if (co2 > 5000) {
    tft.setTextColor(ST77XX_RED);
    tft.println("!! Very High CO2 Levels");
  } else if (co2 > 2000) {
    tft.setTextColor(ST77XX_ORANGE);
    tft.println("! High CO2");
  } else if (co2 > 1000) {
    tft.setTextColor(ST77XX_YELLOW);
    tft.println("Moderately High CO2");
  } else {
    tft.setTextColor(ST77XX_GREEN);
    tft.println("Normal CO2");
  }
  tft.setTextColor(ST77XX_WHITE);
}

void checkTemp(float temp) {
  if (temp > 34.0 || temp < 15.0) {
    tft.setTextColor(ST77XX_RED);
    tft.print("!!!");
    tft.setTextColor(ST77XX_WHITE);
  }
}

void checkPM25(uint16_t pm25) {
  if (pm25 > 55) {
    tft.setTextColor(ST77XX_RED);
    tft.println("!!! Air quality unhealthy");
  } else if (pm25 >= 35) {
    tft.setTextColor(ST77XX_ORANGE);
    tft.println("!! Very Unhealthy");
  } else if (pm25 >= 12) {
    tft.setTextColor(ST77XX_YELLOW);
    tft.println("! Acceptable");
  } else {
    tft.println("");
  }
  tft.setTextColor(ST77XX_WHITE);
}

void displayAllData() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);

  tft.print("TEMP: "); tft.print(temperature, 1); tft.println("C");
  checkTemp(temperature);
  tft.print("HUM: "); tft.print(humidity, 1); tft.println("%");
  checkHumidity(humidity);
  tft.print("PM2.5: "); tft.print(pm_data.pm25_standard); tft.println("ug/m3");
  checkPM25(pm_data.pm25_standard);
  tft.print("CO2: "); tft.print(co2_ppm); tft.println("ppm");
  checkCO2(co2_ppm);
}
