#include <Arduino.h>
#include <ArduinoLog.h>
#include "time.h"

#include "WifiClient.h"
#include "PubSubClient.h"
#include "EspApi.h"
#include "ShellyApi.h"
#include "PirManager.h"
#include "AlarmManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "TemperatureManager.h"

#define ESP32_NOTIFICATION_TOPIC "esp32/notification"
#define DURATION_24_HOURS 3600 * 1000 * 24

WifiClient wifiClient = WifiClient();
PubSubClient pubSubClient = PubSubClient();

EspApi espApi = EspApi();
ShellyApi shellyApi = ShellyApi();

AlarmManager alarmManager = AlarmManager(pubSubClient);
PirManager pirManager = PirManager(pubSubClient);
CameraManager cameraManager = CameraManager(pubSubClient, espApi);
LightManager lightManager = LightManager(pubSubClient, shellyApi);
TemperatureManager temperatureManager = TemperatureManager(pubSubClient, shellyApi);

unsigned long lastIntrusionTime = 0;

void setup()
{
    Serial.begin(115200);

    delay(3000);

    Log.begin(LOG_LEVEL_VERBOSE, &Serial, true);

    wifiClient.connect();
    pubSubClient.connect();

    configTime(0, 0, "fr.pool.ntp.org");

    pirManager.init();
    alarmManager.init();
    cameraManager.init();
    lightManager.init();
    temperatureManager.init();
}

void loop()
{
    temperatureManager.loop();
    pirManager.loop();
    pubSubClient.loop();

    delay(10);

    if (pirManager.getState() && alarmManager.getIsEnabled())
    {
        lightManager.setState(1);
        cameraManager.takePhoto();

        if (lastIntrusionTime == 0 || millis() - lastIntrusionTime > DURATION_24_HOURS) {
            pubSubClient.publishRaw(ESP32_NOTIFICATION_TOPIC, "Intrusion detected");
            lastIntrusionTime = millis();
        }
    }
}
