#include "ShellyApi.h"

#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ArduinoLog.h>

#include "secrets.h"

ShellyApi::ShellyApi()
{
}

ShellyApi::~ShellyApi()
{
}

bool ShellyApi::turn(String &state)
{
    bool isSuccess = true;
    HTTPClient httpClient;
    WiFiClient wifiClient = WiFiClient();

    httpClient.begin(wifiClient, "http://" + String(SHELLY_PLUG_IP) + "/relay/0?turn=" + state);

    int statusCode = httpClient.GET();

    if (statusCode != 200)
    {
        Log.errorln("Shelly turn %s failed : %s", state, httpClient.getString());
        isSuccess = false;
    }

    wifiClient.stop();
    httpClient.end();

    return isSuccess;
}

bool ShellyApi::getStatus(StaticJsonDocument<1536> &doc)
{
    bool isSuccess = true;
    HTTPClient httpClient;
    WiFiClient wifiClient = WiFiClient();

    httpClient.begin(wifiClient, "http://" + String(SHELLY_PLUG_IP) + "/status");

    int statusCode = httpClient.GET();

    if (statusCode == 200)
    {
        deserializeJson(doc, httpClient.getString());
    }
    else
    {
        Log.errorln("Shelly status %s failed : %s", httpClient.getString());
        isSuccess = false;
    }

    wifiClient.stop();
    httpClient.end();

    return isSuccess;
}