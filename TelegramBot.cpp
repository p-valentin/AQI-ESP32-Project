#include "TelegramBot.h"
#include "Display.h"
#include "Sensors.h"

WiFiClientSecure clientSecure;
UniversalTelegramBot bot(BOT_TOKEN, clientSecure);

unsigned long lastNotificationSent = 0;
const long notificationCooldown = 3600000; 

const int CO2_SPIKE_THRESHOLD = 2000;
const int PM25_SPIKE_THRESHOLD = 15;

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
  if (millis() - lastNotificationSent < notificationCooldown) {
    return;
  }

  String alertMessage = "";
  
  if (previous_co2 != 0) { 
    if (co2_ppm > previous_co2) {
      int co2_diff = co2_ppm - previous_co2;
      if (co2_diff >= CO2_SPIKE_THRESHOLD) {
        alertMessage += "💨 CO2 SPIKE: +" + String(co2_diff) + " ppm\n";
      }
    }
    
    if (pm_data.pm25_standard > previous_pm25) {
      int pm25_diff = pm_data.pm25_standard - previous_pm25;
      if (pm25_diff >= PM25_SPIKE_THRESHOLD) {
        alertMessage += "🔥 PM2.5 SPIKE: +" + String(pm25_diff) + " ug/m3\n";
      }
    }
  }

  if (temperature > 34.0 || temperature < 15.0) {
    alertMessage += "🌡️ Temp is abnormal: " + String(temperature, 1) + "C\n";
  }
  if (humidity > 63.0 || humidity < 35.0) {
    alertMessage += "💧 Humidity is abnormal: " + String(humidity, 1) + "%\n";
  }
  if (co2_ppm > 1000 && alertMessage.indexOf("CO2") == -1) {
    alertMessage += "💨 CO2 is high: " + String(co2_ppm) + " ppm\n";
  }
  if (pm_data.pm25_standard > 12 && alertMessage.indexOf("PM2.5") == -1) {
    alertMessage += "🔥 PM2.5 is high: " + String(pm_data.pm25_standard) + " ug/m3\n";
  }

  if (alertMessage != "") {
    Serial.println("Sending Telegram alert...");
    String finalMessage = "!! ⚠️ AIR QUALITY ALERT ⚠️ !!\n" + alertMessage;
    
    if (bot.sendMessage(CHAT_ID, finalMessage, "")) {
      Serial.println("Alert sent!");
      lastNotificationSent = millis(); 
    } else {
      Serial.println("Alert send FAILED!");
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