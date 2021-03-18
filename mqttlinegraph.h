#ifndef MQTTLINEGRAPH_H
#define MQTTLINEGRAPH_H

#include <QObject>
#include <QWidget>
#include <QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QtCharts>
#include <QtCharts/QValueAxis>
#include <mqttlistener.h>
#include <limits>

struct Range {
    double min;
    double max;
};

struct Graph {
    QString name;
    QLineSeries* lineSeries;
    QDateTimeAxis* xAxis;
    QValueAxis* yAxis;
};

class MqttLineGraph:public MqttListener,public QChartView
{
    QVector<Graph> _lines;
    QChartView *_chartView;
    QChart* _chart;
    std::string _topic;
    uint32_t _index=100;
    double _max;
    double _min;
    uint32_t _sampleCount;
    uint32_t _timeSpan;
public:
    MqttLineGraph();
    void on(const MqttMessage&);
    void config(JsonObject& );
    void config(YAML::Node& );
};

#endif // MQTTLINEGRAPH_H
