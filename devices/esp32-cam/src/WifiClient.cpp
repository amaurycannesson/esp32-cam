#include "WifiClient.h"

#include <ArduinoLog.h>
#include <WiFi.h>

WifiClient::WifiClient()
{
}

WifiClient::~WifiClient()
{
}

void WifiClient::connect()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Log.noticeln("Connecting to Wi-Fi %s...", WIFI_SSID);

    while (WiFi.status() != WL_CONNECTED)
    {
        Log.notice(".");
        delay(500);
    }

    Log.noticeln("");
    Log.noticeln("Connected to Wi-Fi!");
    Log.noticeln("IP address: ");
    Log.noticeln(WiFi.localIP());
}