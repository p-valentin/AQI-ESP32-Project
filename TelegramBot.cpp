#include "TelegramBot.h"
#include "Display.h"
#include "Sensors.h"

WiFiClientSecure clientSecure;
UniversalTelegramBot bot(BOT_TOKEN, clientSecure);

unsigned long lastNotificationSent = 0;
const long standardCooldown = 3600000; 
const long criticalCooldown = 30000;   

const int CO2_SPIKE_THRESHOLD = 500;
const int PM25_SPIKE_THRESHOLD = 20;

uint16_t co2History[5] = {0, 0, 0, 0, 0};
uint16_t pm25History[5] = {0, 0, 0, 0, 0};

void setupTelegram() {
  clientSecure.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  tft.print("Connecting to Telegram...");
  if (bot.getMe()) {
    Serial.println("Telegram bot connected!");
    tft.setTextColor(ST77XX_GREEN);
    tft.println(" OK!");
  } else {
    Serial.println("Telegram bot FAILED!");
    tft.setTextColor(ST77XX_RED);
    tft.println(" FAILED!");
  }
}

void checkIncomingMessages() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  if (numNewMessages > 0) {
    Serial.println("Received new message(s)");

    for (int i = 0; i < numNewMessages; i++) {
      String text = bot.messages[i].text;
      String chat_id = bot.messages[i].chat_id;

      if (text == "/start") {
        String welcome = "Welcome! This bot is online and monitoring.\n";
        welcome += "I will send a notification if air quality becomes abnormal.";
        bot.sendMessage(chat_id, welcome, "");
      }
      
      if (text == "/status") {
        Serial.println("Received /status command. Sending data.");
        sendCurrentData(chat_id);
      }
    }
    bot.last_message_received = bot.messages[numNewMessages - 1].update_id;
  }
}

void sendAlertNotification() {
  bool isCritical = false;
  String alertMessage = "";

  if (co2History[0] != 0) { 
    if (co2_ppm > co2History[0]) {
      int co2_diff = co2_ppm - co2History[0];
      if (co2_diff >= CO2_SPIKE_THRESHOLD) {
        alertMessage += "💨 **RAPID CO2 SPIKE!** (+" + String(co2_diff) + " ppm)\n";
        isCritical = true;
      }
    }
    
    if (pm_data.pm25_standard > pm25History[0]) {
      int pm25_diff = pm_data.pm25_standard - pm25History[0];
      if (pm25_diff >= PM25_SPIKE_THRESHOLD) {
        alertMessage += "🔥 **RAPID PM2.5 SPIKE!** (+" + String(pm25_diff) + " ug/m3)\n";
        isCritical = true;
      }
    }
  }

  for (int i = 0; i < 4; i++) {
    co2History[i] = co2History[i+1];
    pm25History[i] = pm25History[i+1];
  }
  co2History[4] = co2_ppm;
  pm25History[4] = pm_data.pm25_standard;

  if (pm_data.pm25_standard > 35) {
    alertMessage += "🔥 PM2.5 is VERY HIGH: " + String(pm_data.pm25_standard) + " ug/m3\n";
    isCritical = true;
  } else if (pm_data.pm25_standard > 12 && !isCritical) {
    alertMessage += "⚠️ PM2.5 is moderate: " + String(pm_data.pm25_standard) + " ug/m3\n";
  }

  if (co2_ppm > 2000) {
    alertMessage += "💨 CO2 is VERY HIGH: " + String(co2_ppm) + " ppm\n";
    isCritical = true;
  } else if (co2_ppm > 1000 && !isCritical) {
    alertMessage += "⚠️ CO2 is high: " + String(co2_ppm) + " ppm\n";
  }

  if (temperature > 34.0 || temperature < 15.0) {
    alertMessage += "🌡️ Temp abnormal: " + String(temperature, 1) + "C\n";
  }
  if (humidity > 63.0 || humidity < 35.0) {
    alertMessage += "💧 Humidity abnormal: " + String(humidity, 1) + "%\n";
  }

  long currentCooldown = isCritical ? criticalCooldown : standardCooldown;

  if (alertMessage != "") {
    if (millis() - lastNotificationSent > currentCooldown || lastNotificationSent == 0) {
      Serial.println("Sending Telegram alert...");
      String finalMessage = "";
      if (isCritical) {
        finalMessage = "‼️ 🚨 CRITICAL AIR ALERT 🚨 ‼️\n\n" + alertMessage;
      } else {
        finalMessage = "⚠️ Air Quality Warning ⚠️\n\n" + alertMessage;
      }
      
      if (bot.sendMessage(CHAT_ID, finalMessage, "")) {
        Serial.println("Alert sent!");
        lastNotificationSent = millis(); 
      } else {
        Serial.println("Alert send FAILED!");
      }
    }
  }
}

void sendCurrentData(String chat_id) {
  
  String message = "Current Air Quality:\n";
  message += "🔥 PM2.5: " + String(pm_data.pm25_standard) + " ug/m3\n";
  message += "💨 CO2:   " + String(co2_ppm) + " ppm\n";
  message += "🌡️ Temp:  " + String(temperature, 1) + " C\n";
  message += "💧 Hum:   " + String(humidity, 1) + " %\n";
  message += "📉 Pres:  " + String(pressure, 3) + " atm\n";

  String recommendations = "";

  if (pm_data.pm25_standard > 35) {
      recommendations += "• **PM2.5 is unhealthy.** Close windows & run an air purifier.\n";
  } else if (pm_data.pm25_standard > 12) {
      recommendations += "• **PM2.5 is moderate.** Consider using an air purifier.\n";
  }

  if (co2_ppm > 2000) {
      recommendations += "• **CO2 is very high!** Ventilate the room immediately.\n";
  } else if (co2_ppm > 1000) {
      recommendations += "• **CO2 is high.** Open a window to get fresh air.\n";
  }

  if (humidity > 63.0) {
      recommendations += "• **Humidity is high.** Use a dehumidifier to prevent mold.\n";
  } else if (humidity < 35.0) {
      recommendations += "• **Humidity is low.** Consider using a humidifier.\n";
  }

  if (temperature > 34.0 || temperature < 15.0) {
      recommendations += "• **Temperature is outside comfort zone.** Adjust thermostat.\n";
  }

  if (recommendations != "") {
    message += "\n**Recommendations:**\n" + recommendations;
  } else {
    message += "\nAll levels are normal. 👍";
  }

  bot.sendMessage(chat_id, message, "");
}