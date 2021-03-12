#ifndef MQTTLINEGRAPH_H
#define MQTTLINEGRAPH_H

#include <QObject>
#include <QWidget>
#include <QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QtCharts>

#include <mqttlistener.h>

class MqttLineGraph:public MqttListener,public QChartView
{
    QLineSeries *_lineSeries;
    QChartView *_chartView;
    QChart* _chart;
public:
    MqttLineGraph();
    void on(const MqttMessage&);
    void config(JsonObject& );
};

#endif // MQTTLINEGRAPH_H
