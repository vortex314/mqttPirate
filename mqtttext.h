#ifndef MQTTTEXT_H
#define MQTTTEXT_H
#include <QObject>
#include <QWidget>
#include <QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QtCharts>

#include <mqttlistener.h>

class MqttText:public MqttListener,public QLineEdit
{
   std::string _topic;
   std::string _label;

public:
    MqttText();
    void on(const MqttMessage&);
    void config(JsonObject& );
};

#endif // MQTTTEXT_H
