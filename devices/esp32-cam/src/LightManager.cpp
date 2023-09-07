#include "LightManager.h"

#include <ArduinoJson.h>

#define LIGHT_STATE_TOPIC "esp32/light/state"
#define LIGHT_COMMAND_TOPIC "esp32/light/command"

LightManager::LightManager(PubSubClient &pubSubClient, ShellyApi &shellyApi) : pubSubClient(pubSubClient), shellyApi(shellyApi), state(false)
{
}

LightManager::~LightManager() {}

void LightManager::init()
{
    this->pubSubClient.subscribe(LIGHT_COMMAND_TOPIC);
    this->pubSubClient.onMessage(std::bind(&LightManager::onMessage, this, std::placeholders::_1, std::placeholders::_2));
}

void LightManager::onMessage(String &topic, String &payload)
{
    StaticJsonDocument<128> cmdDoc;
    deserializeJson(cmdDoc, payload);

    if (topic == LIGHT_COMMAND_TOPIC)
    {
        if (cmdDoc["cmd"] == "on")
        {
            this->setState(1);
        }
        else if (cmdDoc["cmd"] == "off")
        {
            this->setState(0);
        }
    }
}

void LightManager::setState(int state)
{
    if (state == this->state)
        return;

    this->state = state;
    String _state = state ? "on" : "off";

    bool isSuccess = this->shellyApi.turn(_state);

    if (isSuccess)
    {
        this->publishState(state);
    }
}

void LightManager::publishState(int state)
{
    StaticJsonDocument<128> stateDoc;
    stateDoc["state"] = state ? "on" : "off";
    this->pubSubClient.publish(LIGHT_STATE_TOPIC, stateDoc, true, 0);
}