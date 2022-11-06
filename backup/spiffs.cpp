// Example: storing JSON configuration file in flash file system
//
// Uses ArduinoJson library by Benoit Blanchon.
// https://github.com/bblanchon/ArduinoJson
//
// Created Aug 10, 2015 by Ivan Grokhotkov.
//
// This example code is in the public domain.

#include <ArduinoJson.h>
#include "FS.h"
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

bool saveConfig(const char *ssid, const char *password)
{
  StaticJsonDocument<200> doc;
  doc["ssid"] = ssid;
  doc["password"] = password;

  File configFile = LittleFS.open("/config.json", "w");
  if (!configFile)
  {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  serializeJson(doc, configFile);
  return true;
}

bool loadConfig()
{
  File configFile = LittleFS.open("/config.json", "r");
  if (!configFile)
  {
    Serial.println("Failed to open config file");
    if (!saveConfig("Avnica", "zbef2601"))
    {
      Serial.println("Failed to save config");
      return false;
    }
    else
    {
      Serial.println("Config saved");
    }
  }

  size_t size = configFile.size();
  if (size > 1024)
  {
    Serial.println("Config file size is too large");
    return false;
  }
  std::unique_ptr<char[]> buf(new char[size]);

  configFile.readBytes(buf.get(), size);

  StaticJsonDocument<200> doc;
  auto error = deserializeJson(doc, buf.get());
  if (error)
  {
    Serial.println("Failed to parse config file");
    return false;
  }

  const char *ssid = doc["ssid"];
  const char *password = doc["password"];

  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);
  WiFi.begin(ssid, password);
  return true;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  delay(1000);
  Serial.println("Mounting FS...");

  if (!LittleFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  if (!loadConfig())
  {
    Serial.println("Failed to load config");
  }
  else
  {
    Serial.println("WiFi Configured");
    }
}

void loop()
{
}