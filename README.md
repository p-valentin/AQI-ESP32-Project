# ESP32-S3 Air Quality Monitor with Telegram Bot

A comprehensive indoor air quality monitor built on the ESP32-S3 TFT Feather. It tracks PM2.5, CO2, temperature, humidity, and barometric pressure, displays the data on its screen with color-coded warnings, and sends pop-up notifications for unhealthy levels via a Telegram bot.



## Features

* **Live Sensor Monitoring:** Tracks PM2.5, CO2, Temperature, Humidity, and Barometric Pressure.
* **TFT Display:** Shows all sensor values in real-time on the built-in screen.
* **Color-Coded Warnings:** The display shows "!" warnings and custom-colored messages for unhealthy levels of CO2, PM2.5, temperature, and humidity.
* **Telegram Bot Integration:** Connects to a Telegram bot to send periodic air quality reports directly to your phone.
* **Interactive Bot:** The bot is programmed to respond to the `/start` command.
* **Wi-Fi & OTA:** Connects to your local Wi-Fi and supports wireless Over-The-Air (OTA) code updates. You only need to plug it in via USB one time.
* **Clean Code:** The project is organized into separate files (`.h` and `.cpp`) for managing sensors, display, Wi-Fi, and the bot.

## Hardware Required

* **Main Board:** Adafruit ESP32-S3 TFT Feather
* **Sensors:**
    * Adafruit BME280 (Temperature, Humidity, Pressure)
    * Adafruit SCD-40 (CO2)
    * Adafruit PMSA003I (PM2.5)
* **Cables:** 3 x STEMMA QT / Qwiic cables
* **Power:** A reliable 5V 2A (or higher) USB power adapter and a USB-C cable.

## Software & Libraries

This project is built using the Arduino IDE. You must install the following libraries via the Arduino Library Manager (`Tools > Manage Libraries...`):

* `Adafruit GFX Library`
* `Adafruit ST7735 and ST7789 Library`
* `Adafruit BME280 Library`
* `Sensirion I2C SCD4x`
* `Adafruit PM25 AQI`
* `UniversalTelegramBot`
* `ArduinoJson`

## 🚀 Setup & Installation

### 1. Hardware Assembly

Connect all three sensors to the ESP32-S3's STEMMA QT port in a daisy-chain. The order does not matter.

`ESP32` <--> `BME280` <--> `SCD-40` <--> `PMSA003I`

### 2. Create Your Telegram Bot

1.  Open Telegram and start a chat with **`@BotFather`**.
2.  Send the `/newbot` command and follow the instructions to get your **Bot Token**.
3.  Start a chat with **`@getidsbot`** and send `/start` to get your personal **Chat ID**.

### 3. Configure Your Code

1.  **`TelegramBot.h`**: Paste your **Bot Token** and **Chat ID** into this file.
2.  **`WifiManager.cpp`**: Paste your Wi-Fi network's **SSID (name)** and **password** into this file.

### 4. First-Time Upload (via USB)

1.  Plug your ESP32 into your computer via USB.
2.  In the Arduino IDE, select the correct board and **COM port**.
3.  Click the **Upload** button to flash the code.
    * *Troubleshooting:* If the upload fails, you may need to force it into bootloader mode: **Hold** the "BOOT" button, **tap** "RST," then **release** "BOOT" and try uploading again.
4.  Once the code is uploaded, your device is ready.

## Usage

Power on the device. It will automatically connect to your Wi-Fi, initialize all sensors, and start displaying data on the TFT screen.

* You will receive a Telegram notification every time the values are abnormal. Notifications are rate-limited to once per hour to avoid spam.
* You can send `/start` to your bot at any time to see if it's online.

### Future Updates (OTA)

You do not need to use the USB cable again.
1.  In the Arduino IDE, go to `Tools > Port`.
2.  Under **"Network Ports,"** select **"my-aqi-monitor"**.
3.  Click the **Upload** button to send your new code over Wi-Fi.

## File Structure

The project is organized into a modular file structure for easy management.

## License
This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.
