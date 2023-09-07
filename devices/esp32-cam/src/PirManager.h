#ifndef PIRMANAGER_H
#define PIRMANAGER_H

#include "PubSubClient.h"

class PirManager
{
public:
    PirManager(PubSubClient &);
    ~PirManager();
    void init();
    void loop();
    bool getState();

private:
    PubSubClient &pubSubClient;
    void publishState(int state);
    int lastState;
};

#endif