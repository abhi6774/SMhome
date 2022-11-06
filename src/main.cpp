#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

int Addr = 0;

ESP8266WebServer server(80);
const char *settings;
class Car
{
private:
  uint8_t ENA = D2;
  uint8_t IN1 = D3;
  uint8_t IN2 = D4;
  uint8_t ENB = D5;
  uint8_t IN3 = D6;
  uint8_t IN4 = D7;
  uint8_t pins[6] = {ENA, IN1, IN2, ENB, IN3, IN4};

public:
  Car() {}
  void setupPinMode()
  {
    for (auto &&i : pins)
    {
      pinMode(i, OUTPUT);
    }
  }

  void lefttest()
  {

    Serial.println("Testing left motor switches");
    digitalWrite(ENA, HIGH);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    Serial.println("Testing left motor switches completed");

    Serial.println("Starting Left motor speed test");
    delay(2000);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    for (int i = 0; i < 256; i++)
    {
      analogWrite(ENA, i);
      Serial.println(analogRead(ENA));
      delay(50);
    }

    for (int i = 255; i >= 0; --i)
    {
      analogWrite(ENA, i);
      Serial.println(analogRead(ENA));
      delay(50);
    }
    Serial.println("Left motor speed test completed");

    delay(2000);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    Serial.println("Turning Off Left  motors done");
  }

  void righttest()
  {
    Serial.println("Testing right motor switches");
    digitalWrite(ENB, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    Serial.println("Testing right motor switches completed");

    Serial.println("Starting right motor speed test");

    for (int i = 0; i < 256; i++)
    {
      Serial.println(analogRead(ENB));
      analogWrite(ENB, i);
      delay(50);
    }

    for (int i = 255; i >= 0; --i)
    {
      analogWrite(ENB, i);
      Serial.println(analogRead(ENB));
      delay(50);
    }

    Serial.println("Right motor speed test completed");

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  void forward()
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Motor moving forward");
  }

  void backward()
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Motor moving backward");
  }

  void left()
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Motor moving left");
  }

  void right()
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Motor moving right");
  }

  void stop()
  {
    if (!allStopped())
    {
      for (int i = 255; i >= 0; --i)
      {
        analogWrite(ENA, i);
        analogWrite(ENB, i);
        delay(5);
      }
      for (auto &&i : pins)
      {
        digitalWrite(i, LOW);
      }
    }
    Serial.println("Motors stopped");
  }

  bool allStopped()
  {
    bool status = false;
    for (auto &&pin : pins)
    {
      status = digitalRead(pin) == 0 ? false : true;
      if (!status)
        return status;
    }
    return status;
  }

  String status()
  {
    String states;
    states = "<ul>\n"
             "  <li>Enable 1:" +
             String(analogRead(ENA)) + "</li>\n"
                                       "  <li>Enable 2:" +
             String(analogRead(ENB)) + "</li>\n"
                                       "  <li>Motor 1:" +
             String(digitalRead(IN1)) + "</li>\n"
                                        "  <li>Motor 2:" +
             String(digitalRead(IN2)) + "</li>\n"
                                        "  <li>Motor 3:" +
             String(digitalRead(IN3)) + "</li>\n"
                                        "  <li>Motor 4:" +
             String(digitalRead(IN4)) + "</li>\n"
                                        "</ul>";

    return states;
  }
};

Car car = Car();

void serverHandles();

bool writeData(String data)
{
  size_t i;
  for (i = 0; i < data.length(); i++)
  {
    EEPROM.put(i, data[i]);
  }
  EEPROM.put(i, NULL);
  return true;
}

String readData(int startpoint)
{
  String data = "";
  data.reserve(256);
  int i = 0;
  char str;
  do
  {
    EEPROM.get(i, str);
    if (str == NULL)
      continue;
    data += str;
    i++;
  } while (str != NULL);
  return data;
}

void handleRoot()
{
  char temp[600];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 600, "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>Nodemcu Car</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    %s\
    </body> </html>",

           hr,
           min % 60, sec % 60, car.status().c_str());
  server.send(200, "text/html", temp);
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void handleSettings()
{
  DynamicJsonDocument setting(256);

  if (server.method() == HTTP_POST)
  {
  }
  else
  {

    Serial.print("settings not available");
    server.send(200, "application/json", settings);
  }
}

void handlePing()
{
  String ping_res = "{\"response\": \"pong\",\"status\": \"Available\"}";
  server.send(200, "application/json", ping_res);
}

IPAddress local_ip(192, 168, 79, 52);
IPAddress gateway(192, 168, 79, 52);
IPAddress subnet(255, 255, 255, 0);

void setup(void)
{
  car.setupPinMode();
  Serial.begin(115200);
  EEPROM.begin(512);
  WiFi.config(local_ip, gateway, subnet);
  WiFi.mode(WIFI_STA);
  DynamicJsonDocument doc(256);
  deserializeJson(doc, readData(0));
  String ssid = doc["ssid"];
  String password = doc["password"];

  if (ssid)
  {
    String settings = "{\"ssid\":\"" + "Avnica" + "\",\"password\":\"" + "zbef2601" + "\"}";
    writeData(settings);
  }

  Serial.print(ssid);
  Serial.print(" ");
  Serial.println(password);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  serverHandles();
}

void loop(void)
{
  server.handleClient();
  MDNS.update();
}

void serverHandles()
{

  server.on("/", handleRoot);

  server.on("/lefttest", []()
            {
              car.lefttest();
               server.send(200, "application/json", "{\"response\": \"left motor testing completed\'}"); });

  server.on("/righttest", []()

            { 
              car.righttest();
               server.send(200, "application/json", "{\"response\": \"right motor testing completed\'}"); });

  server.on("/forward", []()
            { 
              car.forward();
              server.send(200, "application/json", "{\"response\": \"moving forward at speed 255\'}"); });

  server.on("/backward", []()
            { car.backward(); server.send(200, "application/json", "{\"response\": \"moving backward with speed 255\'}"); });

  server.on("/left", []()
            { car.left(); server.send(200, "application/json", "{\"response\": \"moving left with speed 255\'}"); });
  server.on("/right", []()
            { car.right(); server.send(200, "application/json", "{\"response\": \"moving left with speed 255\'}"); });

  server.on("/stop", []()
            { car.stop(); server.send(200, "application/json", "{\"response\": \"Stopping All the motors\'}"); });

  server.on("/on", []()
            { server.send(200, "application/json", "{\"response\": \"success ON\'}"); });

  server.on("/off", []()
            { server.send(200, "application/json", "{\"response\": \"success OFF\'}"); });

  server.on("/settings", handleSettings);
  server.on("/ping", handlePing);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}