#include "mqttlistener.h"

MqttListener::MqttListener():Sink<MqttMessage>(30)
{

}

