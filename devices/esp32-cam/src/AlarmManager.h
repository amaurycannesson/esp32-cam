#ifndef ALARMMANAGER_H
#define ALARMMANAGER_H

#include "PubSubClient.h"

class AlarmManager
{
public:
    AlarmManager(PubSubClient &);
    ~AlarmManager();
    void init();
    bool getIsEnabled();

private:
    PubSubClient &pubSubClient;
    void onMessage(String &topic, String &payload);
    void publishState(const char *status);
    void sendCommand(const int *cmdToSend);
    void setIsEnabled(bool isEnabled);
    bool isEnabled;
};

#endif