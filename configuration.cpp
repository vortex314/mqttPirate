#include <configuration.h>
#include <Mqtt.h>
#include <MqttPaho.h>
#include <limero.h>
#include <qtablewidget.h>
#include <unistd.h>

#include <QApplication>
#include <QHeaderView>
#include <QTableView>
#include <QWidget>
#include <QChartView>
#include <QtCharts/QLineSeries>

void commandArguments(JsonObject config, int argc, char **argv);
bool loadConfig(JsonDocument &doc, const char *name);


using namespace QtCharts;

void lineChart() {
  QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
  series->append(0, 6);
  series->append(2, 4);
  series->append(3, 8);
  series->append(7, 4);
  series->append(10, 5);
  *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6)
          << QPointF(18, 3) << QPointF(20, 2);
  QChart *chart = new QChart();
  chart->legend()->hide();
  chart->addSeries(series);
  chart->createDefaultAxes();
  chart->setTitle("Simple line chart example");

  QChartView *chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
}

/*
  load configuration file into JsonObject
*/
bool Configuration::load(JsonDocument& doc,JsonObject& json,const char *name) {
  FILE *f = fopen(name, "rb");
  if (f == NULL) {
    ERROR(" cannot open config file : %s", name);
    return false;
  }
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET); /* same as rewind(f); */

  char *string = (char *)malloc(fsize + 1);
  fread(string, 1, fsize, f);
  fclose(f);

  string[fsize] = 0;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, string);
  if (error) {
    ERROR(" JSON parsing config file : %s failed.", name);
    return false;
  }
  json=doc.as<JsonObject>();
  std::string jsonString;
  serializeJsonPretty(doc, jsonString);
  INFO(" config loaded : %s ", jsonString.c_str());
  return true;
}

