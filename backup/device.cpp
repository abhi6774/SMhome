#include <device.h>
#include <string>

// Device::Device(uint8_t id, String name, int status = 1) : id {id}, status {status}, name{name}  {};

String toString (Device device){
    String str = "{ \"name\": \"";
    str += device.name + "\", \"id\":\"";
    // str += std::to_string(device.id);
    str += (uint8_t) device.id;
    str +=  "\", \"status\":\"";
    // str += std::to_string(device.status);
    str += String(std::to_string(device.status).c_str());
    str += "\"}";
    return str;
}

Devices::Devices(){}
Devices::~Devices(){}

int Devices::add(Device equipment) {
    for (auto &&item : device)
    {
        if (item.id == equipment.id)
        {
            return -1;
        }
        
    }
    
    device.push_back(equipment);
    return 0;
}

int Devices::remove(String name) {
    for(auto itr = device.begin(); itr != device.end(); ++itr){
        if (itr->name == name)
        {
            device.erase(itr);
            return 0;
            break;
        }
    } 
    return -1;  
}

int Devices::on(uint8_t id) {
    for(auto itr = device.begin(); itr != device.end(); ++itr){
        if (itr->id == id)
        {
            digitalWrite(id, HIGH);
            return 0;
            break;
        }
    }
    return -1;
}

int Devices::off(uint8_t id) {
    for(auto itr = device.begin(); itr != device.end(); ++itr){
        if (itr->id == id)
        {
            digitalWrite(id, LOW);
            return 0;
            break;
        }
    }
    return -1;
}

int Devices::on(String name) {
    for(auto itr = device.begin(); itr != device.end(); ++itr){
        if (itr->name == name)
        {
            digitalWrite(itr->id, HIGH);
            return 0;
            break;
        }
    }
    return -1;
}

int Devices::off(String name) {
    for(auto itr = device.begin(); itr != device.end(); ++itr){
        if (itr->name == name)
        {
            digitalWrite(itr->id, LOW);
            return 0;
            break;
        }
    }
    return -1;
}

String Devices::jsonlist() {
    String jsonstring = "[";
    for (auto &&item : device)
    {
        jsonstring += toString(item);
        jsonstring += ",";
    }
    jsonstring.remove(jsonstring.lastIndexOf(","));
    jsonstring += "]";
    return jsonstring;
}