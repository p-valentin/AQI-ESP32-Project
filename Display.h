#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_PM25AQI.h>

extern Adafruit_ST7789 tft;

void setupDisplay();
void checkHumidity(float humidity);
void checkCO2(uint16_t co2);
void checkTemp(float temp);
void checkPM25(uint16_t pm25);
void displayAllData();
