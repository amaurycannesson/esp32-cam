# LILYGO T-Camera S3

Embedded code for the ESP32 Camera.

## Setup

Add `./src/secrets.h`

```cpp
#ifndef SECRETS_H_
#define SECRETS_H_

#define SECRET
#define THINGNAME "esp32-cam"

static const char WIFI_SSID[] = "";
static const char WIFI_PASSWORD[] = "";
static const char AWS_IOT_ENDPOINT[] = "";

static const char ESP_API_ENDPOINT[] = "";
static const char ESP_API_KEY[] = "";

static const char SHELLY_PLUG_IP[] = "";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
-----END RSA PRIVATE KEY-----
)KEY";

#endif
```

## Run

Run it with [PlatformIO](https://platformio.org/install/ide?install=vscode)

## Credits

* Cloned from [LilyGo-Cam-ESP32S3](https://github.com/Xinyuan-LilyGO/LilyGo-Cam-ESP32S3)
* https://aws.amazon.com/fr/blogs/compute/building-an-aws-iot-core-device-using-aws-serverless-and-an-esp32/