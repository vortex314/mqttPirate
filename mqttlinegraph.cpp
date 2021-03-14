#include "mqttlinegraph.h"
using namespace QtCharts;

MqttLineGraph::MqttLineGraph() : QChartView()
{
    _chart = new QChart;
    _chartView=this;
    _chartView->setMinimumSize(100, 100);

/*
    _axisX->setTickCount(5);
    _axisX->setFormat("hh:mm:ss.zzz");
    QDateTime dt=QDateTime::currentDateTime();
    qint64 ms=dt.toMSecsSinceEpoch();

    _axisX->setRange(dt,dt.addSecs(100));
    _axisY->setRange(_min,_max);
    // _chart->legend()->hide();
    //    _chart->setAnimationOptions(QChart::AllAnimations);
    _chart->addSeries(_lineSeries);
    _chart->addAxis(_axisX,Qt::AlignBottom);
    _chart->addAxis(_axisY,Qt::AlignLeft);
    _lineSeries->attachAxis(_axisX);
    _lineSeries->attachAxis(_axisY);

    //   _chart->createDefaultAxes();
    _chart->setTitle("Simple line chart example");
*/
    setChart(_chart);
    //    _chartView = new QChartView(this);
    _chartView->setRenderHint(QPainter::Antialiasing);
    _chartView->show();
}

Range getRangeX(QLineSeries* series){
    Range xRange={std::numeric_limits<double>::max(),std::numeric_limits<double>::min()};
    for(QPointF pt:series->pointsVector()) {
        xRange.max = pt.x() > xRange.max ? pt.x() : xRange.max;
        xRange.min = pt.x() < xRange.min ? pt.x() : xRange.min;
    }
    return xRange;
}

void MqttLineGraph::on(const MqttMessage& m){
    for(Graph graph:_lines){
        if ( graph.name.compare(m.topic.c_str())==0){
            DynamicJsonDocument doc(1000);
            deserializeJson(doc,m.message);
            double y=doc.as<double>();

            QDateTime dt=QDateTime::currentDateTime();
            graph.lineSeries->append(dt.toMSecsSinceEpoch(),y);
            Range xRange = getRangeX(graph.lineSeries);
            graph.xAxis->setRange(QDateTime::fromMSecsSinceEpoch(xRange.min),QDateTime::fromMSecsSinceEpoch(xRange.max));
            if ( graph.lineSeries->count() > _sampleCount || (xRange.max-xRange.min)>_timeSpan)
                graph.lineSeries->remove(0);
            //            _chart->update();
            INFO(" %s : %f",_topic.c_str(),y);
        }
    }
}

void MqttLineGraph::config(JsonObject& json){
    JsonArray topics=json["topics"];
    _min = json["min"] | -1000.0;
    _max = json["max"] | 1000.0;
    _sampleCount = json["samples"] | 100;
    _timeSpan = json["timeSpan"] | 100;
    _timeSpan *= 1000; // to  msec

    QValueAxis* yAxis = new QValueAxis();
    yAxis->setMax(_max);
    yAxis->setMin(_min);
    yAxis->setRange(_min,_max);
    _chart->addAxis(yAxis,Qt::AlignLeft);


    QDateTimeAxis* xAxis = new QDateTimeAxis();
    xAxis->setTickCount(5);
    QDateTime dt=QDateTime::currentDateTime();
    xAxis->setRange(dt,dt.addSecs(100));
    xAxis->setFormat("hh:mm:ss.zzz");
    _chart->addAxis(xAxis,Qt::AlignBottom);


    for(std::string topic:topics) {
        Graph graph={topic.c_str(),new QLineSeries(),xAxis,yAxis};
        _chart->addSeries(graph.lineSeries);
        graph.lineSeries->attachAxis(xAxis);
        graph.lineSeries->attachAxis(yAxis);
        graph.lineSeries->setName(topic.c_str());
        _lines += graph;
        _topic=topic;
    }


    INFO(" linegraph : %s %f -> %f ",_topic.c_str(),_min,_max);

}
