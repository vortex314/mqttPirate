#ifndef MQTTTABLE_H
#define MQTTTABLE_H

#include <QObject>
#include <QWidget>
#include <QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QtCharts>
#include <QtCharts/QValueAxis>
#include <mqttlistener.h>
#include <mqttmodel.h>
#include <limits>

class MqttTable : public MqttListener,public QTableView
{
    QVector<QString> _topics;
    uint32_t _index=100;
    double _max;
    double _min;
    uint32_t _sampleCount;
    uint32_t _timeSpan;
public:
    MqttTable(MqttModel*);
    void on(const MqttMessage&);
    void config(JsonObject& );
};

#endif // MQTTTABLE_H
