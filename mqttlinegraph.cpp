#include "mqttlinegraph.h"
using namespace QtCharts;

MqttLineGraph::MqttLineGraph()
{
    _lineSeries = new QtCharts::QLineSeries();
    _lineSeries->append(0, 6);
    _lineSeries->append(2, 4);
    _lineSeries->append(3, 8);
    _lineSeries->append(7, 4);
    _lineSeries->append(10, 5);
    *_lineSeries << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6)
            << QPointF(18, 3) << QPointF(20, 2);
    _chart = new QChart;
//    _chart->legend()->hide();
    _chart->addSeries(_lineSeries);
    _chart->createDefaultAxes();
    _chart->setTitle("Simple line chart example");

    _chartView=this;
//    _chartView = new QChartView(this);
    _chartView->setRenderHint(QPainter::Antialiasing);
    _chartView->show();
}

void MqttLineGraph::on(const MqttMessage& m){

}

void MqttLineGraph::config(JsonObject& ){

}
