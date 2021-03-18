#ifndef MQTTLISTENER_H
#define MQTTLISTENER_H

#include <limero.h>
#include <Mqtt.h>
#include <QWidget>
#include <yaml-cpp/yaml.h>


class MqttListener : public Sink<MqttMessage>
{
public:
    MqttListener();
    virtual void on(const MqttMessage&)=0;
    virtual void config(JsonObject& ){};
    virtual void config(YAML::Node&){};
};

#endif // MQTTLISTENER_H
