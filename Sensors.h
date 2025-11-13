#pragma once
#include <Adafruit_BME280.h>
#include <SensirionI2cScd4x.h>
#include <Adafruit_PM25AQI.h>
#include <Wire.h>

extern Adafruit_BME280 bme;
extern SensirionI2cScd4x scd4x;
extern Adafruit_PM25AQI pm25;

extern float temperature, humidity, pressure, scd_temp, scd_humidity;
extern uint16_t co2_ppm;
extern PM25_AQI_Data pm_data;

void setupSensors();
void readAllSensors();
