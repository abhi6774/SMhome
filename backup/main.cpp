#include <Arduino.h>

int ENA = 2;
int IN1 = 3;
int IN2 = 4;

String firebase_host = "devapp-32835-default-rtdb.firebaseio.com";
String firebase_auth = "bC1CoYyxeTOUEikK8xNRoAax0RX6f0QynlrK1DNA";

void setup()
{
  Serial.begin(115200);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop()
{
  int data = Serial.read();
  Serial.print(data);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // analogWrite(ENA, 0);
  digitalWrite(ENA, LOW);

  delay(500);
}

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Firebase Files
#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#include <FirebaseArduino.h>

const char *ssid = "Avnica";
const char *password = "zbef2601";

String firebase_host = "devapp-32835-default-rtdb.firebaseio.com";
String firebase_auth = "bC1CoYyxeTOUEikK8xNRoAax0RX6f0QynlrK1DNA";

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Uploading New Code");
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]()
                     {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type); });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    } });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(firebase_host, firebase_auth);
}

int n = 0;

void loop()
{
  ArduinoOTA.handle();
  Firebase.setFloat("number", 42.0);
  // handle error
  if (Firebase.failed())
  {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
    return;
  }
  delay(1000);

  // update value
  Firebase.setFloat("number", 43.0);
  // handle error
  if (Firebase.failed())
  {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
    return;
  }
  delay(1000);

  // get value
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // remove value
  Firebase.remove("number");
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed())
  {
    Serial.print("setting /message failed:");
    Serial.println(Firebase.error());
    return;
  }
  delay(1000);

  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed())
  {
    Serial.print("setting /truth failed:");
    Serial.println(Firebase.error());
    return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed())
  {
    Serial.print("pushing /logs failed:");
    Serial.println(Firebase.error());
    return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
}