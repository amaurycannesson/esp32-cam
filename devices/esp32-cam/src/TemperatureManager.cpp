#include "TemperatureManager.h"

#include <ArduinoJson.h>

#define TEMP_VALUE_TOPIC "esp32/temperature/value"
#define DURATION_1_HOUR 3600 * 1000

TemperatureManager::TemperatureManager(PubSubClient &pubSubClient, ShellyApi &shellyApi) : pubSubClient(pubSubClient), shellyApi(shellyApi), lastTemperatureTime(0)
{
}

TemperatureManager::~TemperatureManager() {}

void TemperatureManager::init()
{
}

void TemperatureManager::loop()
{
    if (this->lastTemperatureTime != 0 && millis() - this->lastTemperatureTime < DURATION_1_HOUR)
        return;

    this->lastTemperatureTime = millis();

    StaticJsonDocument<1536> doc;
    this->shellyApi.getStatus(doc);
    this->publishValue(doc["temperature"].as<float>());
}


void TemperatureManager::publishValue(float value)
{
    StaticJsonDocument<128> valueDoc;
    valueDoc["value"] = value;
    this->pubSubClient.publish(TEMP_VALUE_TOPIC, valueDoc, true, 0);
}