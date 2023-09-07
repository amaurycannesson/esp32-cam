#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include "PubSubClient.h"
#include "ShellyApi.h"

class LightManager
{
public:
    LightManager(PubSubClient &, ShellyApi &);
    ~LightManager();
    void init();
    void setState(int state);
    bool getState();

private:
    PubSubClient &pubSubClient;
    ShellyApi &shellyApi;
    void onMessage(String &topic, String &payload);
    void publishState(int state);
    bool state;
};

#endif