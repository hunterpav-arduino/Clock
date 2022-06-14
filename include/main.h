#include "LittleFS.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "LedControl.h"
#include <Ticker.h>

#define AP_ID "clock-ap-"
#define APP_TITLE "Clock "
#define APP_HOSTNAME "clock-"
#define APP_TOPIC "Clock/"

struct AppState {
  uint32_t chipId;
  String name;
  bool isNew;
  int wdtTicker;

  int Temp;
  int Humid;
};

AppState app_state = {
  .chipId = ESP.getChipId(),
  .name = "",
  .isNew = false,
  .wdtTicker = 0,
  .Temp = 0,
  .Humid = 0,
};

#include "config.h"
#include "clock.h"
#include "neopixel.h"
#include "mqtt.h"
#include "aConnect.h"

Ticker wdtTicker;

void wdtTickerCheck();

void main_init(){
  Serial.println("[init] wdtTicker...");
  wdtTicker.attach(60, wdtTickerCheck);

  Serial.println("[init] WiFi.setSleepMode...");
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  Serial.println("[init] LittleFS...");
  LittleFS.begin();

  Serial.println("[init] NeoPixel...");
  neo_init();

  Serial.println("[init] LedControl...");
  lc_init();


}

void load_config(Config &config){
  Serial.println("Load config...");
  loadConfiguration(config);
  app_state.name = config.name;
  Serial.println("Set autoconnect config page...");
  autoconnect_config_page(config);
  Serial.println("Load config done!");
}

void main_publish_mqtt(AppState app_state){
  if(app_state.isNew){
    StaticJsonDocument<256> doc;
    // Set the values in the document
    doc["chipId"] = app_state.chipId;
    doc["name"] = app_state.name;
    doc["wdtTicker"] = app_state.wdtTicker;
    app_state.isNew = false;

    char buffer[256];
    size_t n = serializeJson(doc, buffer);
    mqtt.publish((APP_TOPIC + app_state.name).c_str(), buffer, n);
  }
}


void restartESP(){
  neo_clear();
  delay(50);
  ESP.restart();
}