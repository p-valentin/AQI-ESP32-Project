#pragma once
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>


#define BOT_TOKEN "TOKEN"
#define CHAT_ID "ID"

extern WiFiClientSecure clientSecure;
extern UniversalTelegramBot bot;

extern unsigned long lastNotificationSent;
extern const long notificationCooldown;
extern unsigned long spikeStartTime;
extern bool isSpikeActive;

void setupTelegram();
void checkIncomingMessages();
void sendAlertNotification();
void sendCurrentData(String chat_id);
