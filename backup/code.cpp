#include <map>
#include <iostream>
#include <string>
#include <json/jsonparse.h>

using namespace std;
struct Device
{
    string name;
    uint8_t id;
    int status;
    string to_string()
    {
        return "Hello";
    }
};
std::string toString(Device device)
{
    string str = "{ \"name \": \"";
    str += device.name + "\", \"id\":\"";
    str.append(std::to_string(device.id));
    str += "\", \"status\":\"";
    str.append(std::to_string(device.status));
    // str.insert()
    str += "\"}";
    return str;
}
int main()
{

    // map<string, string> query;

    // query.insert(pair<string, string>("name", "fan"));
    // query.insert(pair<string, string>("device", "1"));

    // map<string, string>::iterator itr;

    // for (auto itre = query.begin(); itre != query.end(); ++itr)
    // {
    //     cout << itr->first << " = " << itr->second << endl;
    // }

    // map<string, string>::iterator val = query.find("name");

    // cout << val->first << " = " << val->second << endl;
    // val--;
    // cout << val->first << " = " << val->second << endl;

    // string message = "2";
    // uint8_t str = (uint8_t) message.c_str()[0];

    // cout << str  << endl;

    Device Equip;
    Equip.id = 1;
    Equip.name = "name";
    Equip.status = 1;

    // string str = toString(Equip);
    // byte bytes[str.length()];

    // for (int i = 0; i < str.length(); i++)
    // {
    //     bytes[i] = (byte) str.at(i);
    // }

    // for (size_t i = 0; i < str.length(); i++)
    // {
    //     // cout << bytes[i] << " -> ";
    // }
    // cout << endl;

    return 0;
}

/* void appendFile(const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = LittleFS.open(path, "a");
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (LittleFS.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (LittleFS.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}
 */
