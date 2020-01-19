#include <iostream>
#include "mqtt.h"


Mqtt::Mqtt(std::string id, std::string host, int port) : mosquittopp(id.c_str()) {
    _id = id;
    int keepalive = 120;
    // Connect to the broker

    if(connect_async(host.c_str(),port,keepalive))
        throw "Unable to connect to server.";

    if(loop_start())
        throw "Unable to start connection loop.";
}
Mqtt::~Mqtt(){
    std::cout << "Stopping the connection." << std::endl;
    disconnect();
}

void Mqtt::on_connect(int rc) {
    std::cout<<"MQTT connect attempt with code "<<rc<<std::endl;
    if(!rc){
        std::cout<<"Connection successful."<<std::endl;
        // Subscribe to topics here.
        // ...

        std::string status = "{msg:connected}";
        publish("status",status);
    } else {
        //TODO output to stderr?
        std::cout<<"Connect failed"<<std::endl;
    }

}
void Mqtt::on_disconnect(int result)
{
    if(result) // result is 0 iff we requested the disconnect
    {
        switch(reconnect_async())
        {
            case MOSQ_ERR_SUCCESS: printf("Reconnected to broker");
            break;
            case MOSQ_ERR_INVAL: printf("Invalid parameters while trying to reconnect to broker.");
            break;
            case MOSQ_ERR_ERRNO: printf("Error reconnecting to broker.");/*\n%s",strerror_r());*/
            break;
        }
    }
}
void Mqtt::on_message(const struct mosquitto_message *message){
    // Not yet subscribed to anything...
}
void Mqtt::on_subscribe(int mid, int qos_count, const int *granted_qos){
    // Not yet subscribed to anything...
}

void Mqtt::on_log(int level, const char *str)
{
	/* Print all log messages regardless of level. */
	printf("\e[32m%s\e[0m %s\n", "LOG",str);

    // Get these on a server!
    //mosquitto_publish(	mosq, NULL, "garage/34567/log", strlen(str), str, 0, false);
}

/* Easy publishing with typical params. The topic prefix (noodle/DEVICE_ID/) is automatically added.
@param topic i.e. "telemetry"
@param message (optional) The message payload to send. */
int Mqtt::publish(const std::string topic, const std::string message) {
    std::string t = "noodle/" + _id + "/" + topic;
    mosquittopp::publish(NULL, t.c_str(), message.length(), message.c_str(), 0, false);
    return 0;//TODO: error checking
}
