#include "Sensors.h"
#include "Display.h"

Adafruit_BME280 bme;
SensirionI2cScd4x scd4x;
Adafruit_PM25AQI pm25;

float temperature = 0.0, humidity = 0.0, pressure = 0.0;
float scd_temp = 0.0, scd_humidity = 0.0;
uint16_t co2_ppm = 0;
PM25_AQI_Data pm_data;
uint16_t previous_co2 = 0;
uint16_t previous_pm25 = 0;

void setupSensors() {
  tft.println("Initializing sensors...");

  if (!bme.begin()) {
    tft.setTextColor(ST77XX_RED);
    tft.println("BME280 FAILED!");
    while (1);
  }

  scd4x.begin(Wire, 0x62);
  scd4x.stopPeriodicMeasurement();
  delay(500);
  scd4x.startPeriodicMeasurement();

  if (!pm25.begin_I2C(&Wire)) {
    tft.setTextColor(ST77XX_RED);
    tft.println("PM2.5 FAILED!");
    while (1);
  }

  tft.setTextColor(ST77XX_GREEN);
  tft.println("All sensors OK!");
}

void readAllSensors() {
  pm25.read(&pm_data);
  temperature = bme.readTemperature() - 8.0;
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 101325.0F;
  bool dataReady = false;
  scd4x.getDataReadyStatus(dataReady);
  if (dataReady) scd4x.readMeasurement(co2_ppm, scd_temp, scd_humidity);
  previous_co2 = co2_ppm;
  previous_pm25 = pm_data.pm25_standard;

  Serial.println("--------------------");
  Serial.print("PM2.5: "); Serial.println(pm_data.pm25_standard);
  Serial.print("CO2: "); Serial.println(co2_ppm);
  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Hum: "); Serial.println(humidity);
  Serial.print("Pressure: "); Serial.print(pressure); Serial.println(" atm");
}
