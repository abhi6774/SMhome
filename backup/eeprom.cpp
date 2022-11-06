#include <Arduino.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

bool writeData(int start, const char *data)
{
  size_t i;
  String d = String(data);
  for (i = start; i < d.length(); i++)
  {
    EEPROM.put(i, data[i]);
    delay(3);
  }
  EEPROM.put(i, NULL);
  delay(3);
  return true;
}

const char *readData(int startpoint)
{
  const char *data = "";
  int i = 0;
  char str;
  do
  {
    EEPROM.get(i, str);
    delay(3);
    if (str == NULL)
      continue;
    data += str;
    i++;
  } while (str != NULL);
  return data;
}

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(512);
  const char *ssid = readData(0);
  const char *password = readData(30);
  Serial.print("First: ");
  Serial.print(ssid);
  Serial.print(" ");
  Serial.println(password);
  if (ssid == "" || password == "")
  {
    ssid = "Avnica";
    password = "zbef2601";

    writeData(0, ssid);

    writeData(30, password);
    Serial.print("Second: ");
    Serial.print(ssid);
    Serial.print(" ");
    Serial.println(password);
  }

  WiFi.begin(ssid, password);
}

void loop()
{
}