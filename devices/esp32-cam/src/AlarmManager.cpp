#include "AlarmManager.h"

#include <ArduinoJson.h>

#define ALARM_COMMAND_TOPIC "esp32/alarm/command"
#define ALARM_STATE_TOPIC "esp32/alarm/state"

#define SELECTED_PIN 15

#define REPEAT 10
#define MS_DELAY_BETWEEN_REPEAT 5

#define PAYLOAD_SIZE 136
#define PERIOD 480

const int DISABLE_CMD[] = {1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0};
const int ENABLE_CMD[] = {1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0};
const int TRIGGER_CMD[] = {1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0};

AlarmManager::AlarmManager(PubSubClient &pubSubClient) : pubSubClient(pubSubClient), isEnabled(false)
{
}

AlarmManager::~AlarmManager() {}

void AlarmManager::init()
{
    pinMode(SELECTED_PIN, OUTPUT);

    this->pubSubClient.subscribe(ALARM_COMMAND_TOPIC);
    this->pubSubClient.onMessage(std::bind(&AlarmManager::onMessage, this, std::placeholders::_1, std::placeholders::_2));
}

bool AlarmManager::getIsEnabled()
{
    return this->isEnabled;
}

void AlarmManager::onMessage(String &topic, String &payload)
{
    StaticJsonDocument<128> cmdDoc;
    deserializeJson(cmdDoc, payload);

    if (topic == ALARM_COMMAND_TOPIC)
    {
        if (cmdDoc["cmd"] == "disable")
        {
            this->setIsEnabled(false);
        }
        else if (cmdDoc["cmd"] == "enable")
        {
            this->setIsEnabled(true);
        }
        else if (cmdDoc["cmd"] == "beep")
        {
            this->sendCommand(TRIGGER_CMD);
            this->publishState("beeping");
        }
    }
}

void AlarmManager::setIsEnabled(bool isEnabled) 
{
    if (isEnabled) {
        this->sendCommand(ENABLE_CMD);
        this->isEnabled = true;
    } else {
        this->sendCommand(DISABLE_CMD);
        this->isEnabled = false;
    }

    this->publishState(isEnabled ? "enabled" : "disabled");
}

void AlarmManager::sendCommand(const int *cmdToSend)
{
    unsigned int remaningRepeat = REPEAT;
    unsigned int i = 0;

    while (remaningRepeat > 0)
    {
        while (i < PAYLOAD_SIZE)
        {
            digitalWrite(SELECTED_PIN, cmdToSend[i]);
            delayMicroseconds(PERIOD);
            i++;
        }

        i = 0;
        remaningRepeat--;
        delay(MS_DELAY_BETWEEN_REPEAT);
    }
}

void AlarmManager::publishState(const char *state)
{
    StaticJsonDocument<128> stateDoc;
    stateDoc["state"] = state;
    this->pubSubClient.publish(ALARM_STATE_TOPIC, stateDoc, true, 0);
}
