#ifndef ESPAPI_H
#define ESPAPI_H

#include <Arduino.h>

class EspApi
{
public:
    EspApi();
    ~EspApi();
    bool savePhotoToS3(String name, uint8_t *data, size_t size);
};

#endif