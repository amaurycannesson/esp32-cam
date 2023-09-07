#include "CameraManager.h"

#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <HTTPClient.h>
#include <base64.h>

#define XPOWERS_CHIP_AXP2101

#include "esp_camera.h"
#include "XPowersLib.h"

#include "utilities.h"
#include "camera_index.h"

#define CAM_COMMAND_TOPIC "esp32/cam/command"
#define CAM_PHOTO_TOPIC "esp32/cam/photo"
#define DURATION_5_SECONDS 5 * 1000

void startCameraServer();
XPowersPMU PMU;

CameraManager::CameraManager(PubSubClient &pubSubClient, EspApi &espApi) : pubSubClient(pubSubClient), espApi(espApi), lastPhotoTime(0)
{
}

CameraManager::~CameraManager() {}

void CameraManager::init()
{
    /*********************************
     *  step 1 : Initialize power chip,
     *  turn on camera power channel
     ***********************************/
    if (!PMU.begin(Wire, AXP2101_SLAVE_ADDRESS, I2C_SDA, I2C_SCL))
    {
        Log.errorln("Failed to initialize power.....");
        while (1)
        {
            delay(5000);
        }
    }
    // Set the working voltage of the camera, please do not modify the parameters
    PMU.setALDO1Voltage(1800); // CAM DVDD  1500~1800
    PMU.enableALDO1();
    PMU.setALDO2Voltage(2800); // CAM DVDD 2500~2800
    PMU.enableALDO2();
    PMU.setALDO4Voltage(3000); // CAM AVDD 2800~3000
    PMU.enableALDO4();

    // TS Pin detection must be disable, otherwise it cannot be charged
    PMU.disableTSPinMeasure();

    /*********************************
     *  step 3 : Initialize camera
     ***********************************/
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_UXGA;
    config.pixel_format = PIXFORMAT_JPEG; // for streaming
    // config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1;

    // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
    //                      for larger pre-allocated frame buffer.
    if (config.pixel_format == PIXFORMAT_JPEG)
    {
        if (psramFound())
        {
            config.jpeg_quality = 10;
            config.fb_count = 2;
            config.grab_mode = CAMERA_GRAB_LATEST;
        }
        else
        {
            // Limit the frame size when PSRAM is not available
            config.frame_size = FRAMESIZE_SVGA;
            config.fb_location = CAMERA_FB_IN_DRAM;
        }
    }
    else
    {
        // Best option for face detection/recognition
        config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
        config.fb_count = 2;
#endif
    }

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Log.errorln("Camera init failed with error 0x%x Please check if the camera is connected well.", err);
        while (1)
        {
            delay(5000);
        }
    }

    sensor_t *s = esp_camera_sensor_get();
    s->set_framesize(s, FRAMESIZE_FHD);

    // drop down frame size for higher initial frame rate
    // if (config.pixel_format == PIXFORMAT_JPEG)
    // {
    //     s->set_framesize(s, FRAMESIZE_QVGA);
    // }

    // #if defined(LILYGO_ESP32S3_CAM_PIR_VOICE)
    //     s->set_vflip(s, 1);
    //     s->set_hmirror(s, 1);
    // #endif

    /*********************************
     *  step 4 : start camera web server
     ***********************************/
    startCameraServer();

    this->pubSubClient.subscribe(CAM_COMMAND_TOPIC);
    this->pubSubClient.onMessage(std::bind(&CameraManager::onMessage, this, std::placeholders::_1, std::placeholders::_2));
}

void CameraManager::onMessage(String &topic, String &payload)
{
    StaticJsonDocument<128> doc;
    deserializeJson(doc, payload);

    if (topic == CAM_COMMAND_TOPIC && doc["cmd"] == "photo")
    {
        this->takePhoto();
    }
}

void CameraManager::takePhoto()
{
    if (this->lastPhotoTime != 0 && millis() - this->lastPhotoTime < DURATION_5_SECONDS)
        return;

    this->lastPhotoTime = millis();

    camera_fb_t *fb = NULL;
    fb = esp_camera_fb_get();

    struct tm timeinfo;
    int hasTime = getLocalTime(&timeinfo);

    if (fb && hasTime)
    {
        char timeStr[21];
        strftime(timeStr, sizeof(timeStr), "%Y%m%dT%H%M%SZ", &timeinfo);
        String filename = String(timeStr) + ".jpg";

        bool isSuccess = this->espApi.savePhotoToS3(filename, fb->buf, fb->len);
        esp_camera_fb_return(fb);

        if (isSuccess)
        {
            this->publishPhoto(filename.c_str());
        }
    }
}

void CameraManager::publishPhoto(const char *name)
{
    StaticJsonDocument<128> updateDoc;
    updateDoc["photo"] = name;
    this->pubSubClient.publish(CAM_PHOTO_TOPIC, updateDoc, false, 0);
}