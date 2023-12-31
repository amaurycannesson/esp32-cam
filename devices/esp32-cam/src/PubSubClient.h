#include <vector>

#include <ArduinoJson.h>

#ifndef PUBSUBCLIENT_H
#define PUBSUBCLIENT_H

class PubSubClient
{
public:
  PubSubClient();
  ~PubSubClient();
  void connect();
  void loop();
  void onMessage(std::function<void(String &, String &)> callback);
  void subscribe(const char *topic);
  void publish(const char *topic, StaticJsonDocument<128> &doc, bool isRetained = false, int qos = 0);
  void publishRaw(const char *topic, const char *message);

private:
  void _configureWill();
  void _reconnect();
  void _onMessage(String &topic, String &payload);
  void _addTime(StaticJsonDocument<128> &doc);
  std::vector<std::function<void(String &, String &)>> messageHandlers;
};

#endif