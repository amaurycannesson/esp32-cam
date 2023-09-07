#include "PubSubClient.h"

#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoLog.h>

#include "secrets.h"

#define ESP32_STATUS_TOPIC "esp32/status"

WiFiClientSecure wifiSecureClient = WiFiClientSecure();
MQTTClient mqttClient = MQTTClient();

PubSubClient::PubSubClient()
{
}

PubSubClient::~PubSubClient() {}

void PubSubClient::onMessage(std::function<void((String &, String &))> messageHandler)
{
    this->messageHandlers.push_back(messageHandler);
}

void PubSubClient::connect()
{
    wifiSecureClient.setCACert(AWS_CERT_CA);
    wifiSecureClient.setCertificate(AWS_CERT_CRT);
    wifiSecureClient.setPrivateKey(AWS_CERT_PRIVATE);
    mqttClient.begin(AWS_IOT_ENDPOINT, 8883, wifiSecureClient);

    this->configureWill();

    Log.noticeln("Connecting to AWS IOT...");

    while (!mqttClient.connect(THINGNAME))
    {
        Log.notice(".");
        delay(100);
    }

    if (!mqttClient.connected())
    {
        Log.noticeln("AWS IoT Timeout!");
        return;
    }

    Log.noticeln("AWS IoT Connected!");

    mqttClient.onMessage(std::bind(&PubSubClient::_onMessage, this, std::placeholders::_1, std::placeholders::_2));
}

void PubSubClient::_onMessage(String &topic, String &payload)
{
    Log.noticeln("[FROM: %s] %s", topic.c_str(), payload.c_str());
    for (unsigned int i = 0; i < this->messageHandlers.size(); ++i)
    {
        this->messageHandlers[i](topic, payload);
    }
}

void PubSubClient::subscribe(const char *topic)
{
    mqttClient.subscribe(topic);
}

void PubSubClient::configureWill()
{
    char jsonBuffer[64];
    StaticJsonDocument<128> doc;
    doc["status"] = "disconnected";
    serializeJson(doc, jsonBuffer);
    mqttClient.setWill(ESP32_STATUS_TOPIC, jsonBuffer, true, 0);
}

void PubSubClient::loop()
{
    mqttClient.loop();
}

void PubSubClient::publish(const char *topic, StaticJsonDocument<128> &doc, bool isRetained, int qos)
{
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        char timeStr[21];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
        doc["time"] = timeStr;
    }

    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);
    Log.noticeln("[TO: %s] %s", topic, jsonBuffer);
    mqttClient.publish(topic, jsonBuffer, isRetained, qos);
}

void PubSubClient::publishRaw(const char *topic, const char *message) {
    Log.noticeln("[TO: %s] %s", topic, message);
    mqttClient.publish(topic, message);
}