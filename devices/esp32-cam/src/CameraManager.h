#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "PubSubClient.h"

#include "EspApi.h"

class CameraManager
{
public:
    CameraManager(PubSubClient &, EspApi &);
    ~CameraManager();
    void init();
    void takePhoto();

private:
    PubSubClient &pubSubClient;
    EspApi &espApi;
    void onMessage(String &topic, String &payload);
    void publishPhoto(const char *name);
    unsigned long lastPhotoTime;
};

#endif