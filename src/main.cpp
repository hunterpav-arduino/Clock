#include <Arduino.h>
#include "main.h"

#include <ArduinoJson.h>

bool mqttConnection = false;
Config settings;

const char datafile[] = "/data.csv";

int i = 1;

void setup() {
  Serial.begin(115200);
  Serial.println("Main init...");
  main_init();

  Serial.println("Main setup...");

  load_config(settings);
  autoconnect_setup(settings.name);
  mqtt_setup(settings.mqtt_server.c_str());
  ntp_setup();
  Serial.println("Setup done!");
}

void loop() {
  clockTick();
  main_publish_mqtt(app_state);
  autoconnect_update();
  ntp_update();  
  mqtt_update(settings);

}


void wdtTickerCheck(){
  if ((WiFi.status() != WL_CONNECTED) || (settings.mqtt_server && !mqtt.connected()) ) {
    app_state.wdtTicker++;
    app_state.isNew = true;
  }

  if(app_state.wdtTicker > 5){
    restartESP();
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  // Serial.print(". Message: ");
  // for (int i = 0; i < length; i++) {
  //   Serial.print((char)message[i]);
  // }

  StaticJsonDocument<512> doc;
  deserializeJson(doc, message, length);
  
  app_state.Temp = (int)doc["Temp"];
  app_state.Humid = (int)doc["Humid"];

  Serial.println();
}


String postUpload(AutoConnectAux& aux, PageArgument& args) {

  AutoConnectText&  aux_files = aux["files"].as<AutoConnectText>();
  aux_files.value = "";

  settings.name = auxConfig["name"].value;
  settings.mqtt_server = auxConfig["mqtt_server"].value;

  if(saveConfiguration(settings)){
    if(settings.mqtt_server) mqtt_setup(settings.mqtt_server.c_str());
    //mqtt.setServer(settings.mqtt_server.c_str(), 1883);
  }
  
  Dir dir = LittleFS.openDir("/");
  while (dir.next()) {
    aux_files.value = aux_files.value + dir.fileName() + "<br>";
  }

  return String();

}

String getAppName(PageArgument& arg) { return settings.name; }