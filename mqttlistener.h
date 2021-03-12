#ifndef MQTTLISTENER_H
#define MQTTLISTENER_H

#include <limero.h>
#include <Mqtt.h>
#include <QWidget>

class MqttListener : public Sink<MqttMessage>
{
public:
    MqttListener();
    virtual void on(const MqttMessage&)=0;
    virtual void config(JsonObject& )=0;
};

#endif // MQTTLISTENER_H
