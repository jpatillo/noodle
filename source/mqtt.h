#ifndef MQTT_H_
#define MQTT_H_

#include <mosquittopp.h>
#include <string>

class Mqtt : public mosqpp::mosquittopp {
    std::string _id;

public:
    Mqtt(std::string id, std::string host, int port=1883);
    ~Mqtt();

    void on_connect(int rc);
    void on_disconnect(int result);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
    void on_log(int level, const char *str);

    int publish(const std::string topic, const std::string message="");

    int publish_connected(bool isConnected);
};

#endif
