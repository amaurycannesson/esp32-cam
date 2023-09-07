#include "EspApi.h"

#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoLog.h>

#include "secrets.h"

EspApi::EspApi()
{
}

EspApi::~EspApi()
{
}

bool EspApi::savePhotoToS3(String name, uint8_t *data, size_t size)
{
    bool isSuccess = true;
    HTTPClient httpClient;
    WiFiClientSecure wifiSecureClient = WiFiClientSecure();

    wifiSecureClient.setCACert(AWS_CERT_CA);
    httpClient.begin(wifiSecureClient, ESP_API_ENDPOINT + String("/save-photo-to-s3"));

    httpClient.addHeader("x-api-key", ESP_API_KEY);
    httpClient.addHeader("Content-Type", "image/jpeg");
    httpClient.addHeader("Content-Disposition", "attachment; filename=\"" + name + "\"");

    int statusCode = httpClient.POST(data, size);

    if (statusCode != 200)
    {
        Log.errorln("savePhotoToS3 failed : %s", httpClient.getString());
        isSuccess = false;
    }

    httpClient.end();

    return isSuccess;
}