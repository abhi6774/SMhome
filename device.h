#include <vector>
#include <Arduino.h>
// #include <string>


struct Device {
    String name;
    uint8_t id;
    int status;
};

String toString(Device);

class Devices
{
private:
    std::vector<Device> device;
public:
    Devices();
    ~Devices();

    int add(Device);
    int remove(String);

    int off(uint8_t);
    int on(uint8_t);
    int on(String);
    int off(String);

    String jsonlist();
};
