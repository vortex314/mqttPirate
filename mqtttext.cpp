#include "mqtttext.h"

MqttText::MqttText():QLineEdit(){
    setText("----------------");
}

void MqttText::on(const MqttMessage& m){
    if ( m.topic.compare(_topic)==0) {
        setText((_label+m.message).c_str());
        INFO(" %s:%s",m.topic.c_str(),m.message.c_str());
    }
}

void MqttText::config(JsonObject& json){
    JsonArray topics=json["topics"];
    for(std::string topic:topics) {
        _topic=topic;
        INFO(" topic for Text : %s",_topic.c_str());
    }
    _label = json["label"] | "";
}
