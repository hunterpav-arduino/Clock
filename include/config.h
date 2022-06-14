#include <Arduino.h>
#include <ArduinoJson.h>
#include "LittleFS.h"

struct Config {
  String name;
  String mqtt_server;
};


// Our configuration structure.
//
// Never use a JsonDocument to store the configuration!
// A JsonDocument is *not* a permanent storage; it's only a temporary storage
// used during the serialization phase. See:
// https://arduinojson.org/v6/faq/why-must-i-create-a-separate-config-object/

Config defaults = {
  String(ESP.getChipId(), HEX), 
  "",
};

const char *config_filename = "./config.json";

// Loads the configuration from a file
void loadConfiguration(Config &config) {
  // Open file for reading
  LittleFS.begin();
  File file = LittleFS.open(config_filename, "r");
  if (!file) {
    Serial.println(F("Failed to open config file, use defaults"));
    config = defaults;
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<512> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  serializeJson(doc, Serial);
  Serial.println("");
  // Copy values from the JsonDocument to the Config
  config.name = doc["name"] | defaults.name;
  config.mqtt_server = doc["mqtt_server"] | defaults.mqtt_server;
  
  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
}

// Saves the configuration to a file
bool saveConfiguration(const Config &config) {
  // Open file for writing
  LittleFS.begin();
  File file = LittleFS.open(config_filename, "w+");
  if (!file) {
    Serial.println(F("Failed to create file"));
    return false;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  doc["name"] = config.name;
  doc["mqtt_server"] = config.mqtt_server;

  // Serialize JSON to file
  serializeJson(doc, Serial);
  Serial.println("");
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
    return false;
  }

  // Close the file
  file.close();
  return true;
}
