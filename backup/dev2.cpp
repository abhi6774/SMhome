#include <device.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <map>
#include <EEPROM.h>
#include <FS.h>

using namespace std;
const int led_builtin = 2;

const String ssid = "Avnica";
const String password = "zbef2601";
typedef std::map<String, String> Query;
Devices devices;

ESP8266WebServer server(80);

void handleRoot() {
    server.send(200, "text/html", 
    "<form action=\"/LED_BUILTIN_on\" method=\"get\" id=\"form1\"> </form> <button type=\"submit\" form=\"form1\" value=\"On\">On</button> <form action=\"/LED_BUILTIN_off\" method=\"get\" id=\"form2\"> </form><button type=\"submit\" form=\"form2\" value=\"Off\">Off</button>");
}

void handleSave() {
    if (server.arg("pass") != "")
    {
        Serial.print(server.arg("pass"));
    }
}

String mapToString(Query query) {
    String str = "{";
    Query::iterator itr;
    for (itr = query.begin(); itr != query.end(); ++itr)
    {
        str += "\"" + itr->first + "\":\"" + itr->second + "\",";
    }
    str += "}";
    return str;
}


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    delay(3000);
    Serial.begin(115200);
    Serial.println();

    Serial.print("Configuring access point ...");
    WiFi.begin(ssid.c_str(), password.c_str());
    EEPROM.begin(100);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");

    Serial.println("WiFi Connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    server.on("/", handleRoot);
    server.begin();

    Serial.println ("HTTP server started" );
    server.on("/LED_BUILTIN_on", []() {
        digitalWrite(LED_BUILTIN, 1);
        Serial.println("on");
        handleRoot();
    });
    server.on("/LED_BUILTIN_off", []() {
        digitalWrite(LED_BUILTIN, 0);
        Serial.println("off");
        handleRoot();
    });
    
    server.on("/worker", HTTP_GET, []() {
        Query query;
        for (int i = 0; i < server.args(); i++)
        {
            query.insert(pair<String, String>(server.argName(i), server.arg(i))); 
        }
        server.send(200, "application/json", mapToString(query));
    });

    server.on("/add", []() {
        if (server.hasArg("name") && server.hasArg("id"))
        {
            uint8_t id = (uint8_t) server.arg("id").c_str()[0];
            Device Equip;
            Equip.id = id;
            Equip.name = server.arg("name");
            Equip.status = 1;
            devices.add(Equip);
        }

        String list = devices.jsonlist();
        Serial.print(list);
        server.send(200, "application/json", list);        
    });

    server.on("/status", []() {
        int status = digitalRead(LED_BUILTIN);
        String state = "[";
        state += status + "]";
        Serial.print("Status: ");
        Serial.println(state.c_str());
        server.send(200, "application/json", state.c_str());
    });

}

void loop() {
    server.handleClient();
}