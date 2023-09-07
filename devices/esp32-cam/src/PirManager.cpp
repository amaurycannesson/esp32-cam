#include "PirManager.h"

#include <ArduinoJson.h>

#include "utilities.h"

#define PIR_STATE_TOPIC "esp32/pir/state"

PirManager::PirManager(PubSubClient &pubSubClient) : pubSubClient(pubSubClient), lastState(false)
{
}

PirManager::~PirManager() {}

void PirManager::init()
{
    pinMode(PIR_INPUT_PIN, INPUT);
    this->lastState = digitalRead(PIR_INPUT_PIN);
}

bool PirManager::getState()
{
    return this->lastState;
}

void PirManager::loop()
{
    int curState = digitalRead(PIR_INPUT_PIN);

    if (curState != this->lastState)
    {
        this->publishState(curState);
    }
}

void PirManager::publishState(int state)
{
    StaticJsonDocument<128> doc;
    doc["state"] = state ? "high" : "low";
    this->lastState = state;
    this->pubSubClient.publish(PIR_STATE_TOPIC, doc, true, 0);
}