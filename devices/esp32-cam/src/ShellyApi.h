#ifndef SHELLYAPI_H
#define SHELLYAPI_H

#include <Arduino.h>
#include <ArduinoJson.h>

class ShellyApi
{
public:
    ShellyApi();
    ~ShellyApi();
    bool turn(String &state);
    bool getStatus(StaticJsonDocument<1536> &doc);
};

#endif