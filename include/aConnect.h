#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
typedef ESP8266WebServer  WiFiWebServer;
#include <AutoConnect.h>
#include "PageBuilder.h"

#include "pages.h"

#ifndef AP_ID
  #define AP_ID "ESP"
#endif
#ifndef AP_PASS
  #define AP_PASS "1234567890"
#endif
#ifndef APP_TITLE
  #define APP_TITLE "ESP Project "
#endif
#ifndef APP_HOSTNAME
  #define APP_HOSTNAME "esp-"
#endif

#define CREDENTIAL_OFFSET 64

WiFiWebServer server;
AutoConnect portal(server);
AutoConnectConfig config;

// Need to be added in main
String getAppName(PageArgument& arg);
String postUpload(AutoConnectAux& aux, PageArgument& args);

String millisToTime(){
  long durationInMillis = millis();
  // long millis = durationInMillis % 1000;
  long second = (durationInMillis / 1000) % 60;
  long minute = (durationInMillis / (1000 * 60)) % 60;
  long hour = (durationInMillis / (1000 * 60 * 60)) % 24;
  
  return String(hour) + ":" + String(minute) + ":" + String(second);
}

// Page construction
PageElement Home(_PAGE_HOME, {
  {"CONFIG_NAME", getAppName},
  {"AC_LINK", [](PageArgument& arg) { return String(AUTOCONNECT_LINK(COG_16)); }},
  {"UPTIME", [](PageArgument& arg) { return millisToTime(); }}
});
PageBuilder HomePage("/", {Home});

AutoConnectAux auxConfig;
AutoConnectAux auxConfigSave;

void autoconnect_setup(String name){
  Serial.println("Portal begin...");
  config.apid = AP_ID + name;
  config.psk = AP_PASS;
  config.ota = AC_OTA_BUILTIN;
  config.title = APP_TITLE + name;
  config.hostName = APP_HOSTNAME + name;
  config.ticker = true;
  config.tickerPort = LED_BUILTIN;
  config.tickerOn = HIGH;
  config.boundaryOffset = 2000;
  config.autoReconnect = true;
//  config.portalTimeout = 60000;
//  config.immediateStart = true;
  portal.config(config);

  HomePage.insert(server);

  auxConfigSave.load(PAGE_CONFIG_SAVE);
  portal.join({ auxConfig, auxConfigSave });
  auxConfigSave.on(postUpload);

  portal.begin();
  Serial.println("Portal ready...");
}

void autoconnect_config_page(Config &config){
  auxConfig.load(PAGE_CONFIG);
  auxConfig["name"].value = config.name;
  auxConfig["mqtt_server"].value = config.mqtt_server;
}

void autoconnect_update(){
  portal.handleClient();
}