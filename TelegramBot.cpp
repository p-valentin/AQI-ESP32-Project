#include "TelegramBot.h"
#include "Display.h"
#include "Sensors.h"

WiFiClientSecure clientSecure;
UniversalTelegramBot bot(BOT_TOKEN, clientSecure);

unsigned long lastNotificationSent = 0;
const long notificationCooldown = 1800000;

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

  if (humidity > 63.0 || humidity < 35.0) {
    alertMessage += "Humidity is abnormal: " + String(humidity, 1) + "%\n";
  }
  if (co2_ppm > 1000) { 
    alertMessage += "CO2 is high: " + String(co2_ppm) + " ppm\n";
  }
  if (pm_data.pm25_standard > 12) {
    alertMessage += "PM2.5 is moderate: " + String(pm_data.pm25_standard) + " ug/m3\n";
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
  message += "PM2.5: " + String(pm_data.pm25_standard) + " ug/m3\n";
  message += "CO2:   " + String(co2_ppm) + " ppm\n";
  message += "Temp:  " + String(temperature, 1) + " C\n";
  message += "Hum:   " + String(humidity, 1) + " %\n";
  message += "Pres:  " + String(pressure, 3) + " atm\n";

  bot.sendMessage(chat_id, message, "");
}
