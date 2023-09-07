#ifndef TEMPERATUREMANAGER_H
#define TEMPERATUREMANAGER_H

#include "PubSubClient.h"
#include "ShellyApi.h"

class TemperatureManager
{
public:
    TemperatureManager(PubSubClient &, ShellyApi &);
    ~TemperatureManager();
    void init();
    void loop(); 

private:
    PubSubClient &pubSubClient;
    ShellyApi &shellyApi;
    void publishValue(float value);
    unsigned long lastTemperatureTime;
};

#endif