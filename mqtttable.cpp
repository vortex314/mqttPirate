#include "mqtttable.h"

MqttTable::MqttTable(MqttModel* mqttModel)
{
    setModel(mqttModel);
    show();
}

void MqttTable::config(JsonObject &){}

void MqttTable::on(const MqttMessage& ){}
