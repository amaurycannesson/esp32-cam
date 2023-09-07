#include "secrets.h"

#ifndef WIFICLIENT_H
#define WIFICLIENT_H

class WifiClient
{
public:
    WifiClient();
    ~WifiClient();
    void connect();
};

#endif