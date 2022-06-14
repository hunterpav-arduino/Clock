#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <avdweb_VirtualDelay.h>

#ifndef AP_ID
  #define AP_ID "ESP"
#endif

#ifndef APP_TOPIC
  #define APP_TOPIC "ESP/"
#endif

WiFiClient espClient;
PubSubClient mqtt(espClient);

byte failed_connections = 0;
VirtualDelay mqttDelay5;

// Need to be added in main
void callback(char* topic, byte* message, unsigned int length);
void restartESP();


void mqtt_setup(const char *mqtt_server){
  Serial.printf("MQTT setup [%s]...\n", mqtt_server);
  mqtt.setServer(mqtt_server, 1883);
  mqtt.setCallback(callback);
}


void reconnect(Config &settings) {
  mqttDelay5.start(5000);  
  if(!mqttDelay5.elapsed()) return;

  Serial.printf("Attempting MQTT connection [%s]... ", settings.mqtt_server.c_str());
  String clientId = "ESP-" + String(ESP.getChipId(), HEX);
  clientId += String(random(0xffff), HEX);
  if (mqtt.connect(clientId.c_str())) {
    Serial.println("connected");
    String topic_out = APP_TOPIC + settings.name;
    String topic_in = APP_TOPIC + String("in");
    mqtt.publish(topic_out.c_str(), "connected");
    mqtt.subscribe(topic_in.c_str());
  } else {
    failed_connections = failed_connections+1;
    Serial.print("failed, rc=");
    Serial.print(mqtt.state());
    Serial.println(" try again in 5 seconds");
  }
  if(failed_connections > 50){
    restartESP();
  }
}

void mqtt_update(Config &settings){
  if (settings.mqtt_server == "") {
    return;
  }

  if (!mqtt.connected()) {
    reconnect(settings);
  }
  mqtt.loop();
}