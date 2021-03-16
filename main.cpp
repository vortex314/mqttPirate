#include "mainwindow.h"

#include <QApplication>
#include <QTableView>
#include <QTableWidget>
#include <QLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include<configuration.h>
#include <Mqtt.h>
#include <MqttPaho.h>
#include <limero.h>
#include <mqttmodel.h>
#include <mqttlinegraph.h>
#include <mqtttext.h>
#include <mqtttable.h>

#include <unistd.h>

void commandArguments(JsonObject& config, int argc, char **argv) ;
Log logger(5120);
StaticJsonDocument<100000> doc;
JsonObject config;
Thread mainThread("main");
MqttPaho mqtt(mainThread);
MqttModel* mqttModel;

int main(int argc, char *argv[])
{
    Configuration::load(doc,config,"/home/lieven/workspace/mqttPirate/mqttPirate.json");
    commandArguments(config,argc,argv);
    std::string level = config["level"] | "I";
    logger.setLogLevel(level[0]);
    JsonObject mqttConfig = config["mqtt"];
    mqtt.config(mqttConfig);
    mqtt.init();
    mqtt.connect();
    JsonArray subscriptions = config["mqtt"]["subscribe"].as<JsonArray>();
    for (auto subscription : subscriptions) {
        mqtt.subscribe(subscription.as<std::string>());
    }

    INFO(" mqttView started. Build : %s ", __DATE__ " " __TIME__);


    QApplication a(argc, argv);
    //   MainWindow w;
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(2,2,2,2);
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);

    QWidget *w = new QWidget();
    w->setLayout(gridLayout);
    w->setWindowTitle("Grid Layouts (12x6)");
    //    w->setFixedSize(500,500);


    mqttModel = new MqttModel();


    JsonArray dashboard=config["dashboard"];
    for( JsonObject item : dashboard) {
        JsonArray coord=item["coord"];
        int row,col,rowSpan,colSpan;
        row = coord[0];
        col = coord[1];
        rowSpan = coord[2];
        colSpan = coord[3];
        std::string type=item["type"];
        JsonArray topics=item["topics"];
        if ( type.compare("LineChart")==0) {
            INFO("add %s",type.c_str());
            MqttLineGraph* mqttGraph=new MqttLineGraph();
            mqttGraph->config(item);
            mqtt.incoming>>mqttGraph;
            gridLayout->addWidget(mqttGraph,row,col,rowSpan,colSpan);
        } else if ( type.compare("Text")==0) {
            INFO("add %s",type.c_str());
            MqttText* mqttText=new MqttText();
            mqttText->config(item);
            mqtt.incoming>>mqttText;
            gridLayout->addWidget(mqttText,row,col,rowSpan,colSpan);
        }else if ( type.compare("Table")==0) {
            INFO("add %s",type.c_str());
            MqttTable* mqttTable=new MqttTable(mqttModel);
            mqttTable->config(item);
            mqtt.incoming>>mqttTable;
            gridLayout->addWidget(mqttTable,row,col,rowSpan,colSpan);
        } else if ( type.compare("Space")==0) {
            INFO("add %s",type.c_str());
            gridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Maximum,QSizePolicy::Minimum),row,col,rowSpan,colSpan);
        }
    }

    mqtt.incoming >>
            [&](const MqttMessage &msg) {
        //        INFO(" %s : %s ",msg.topic.c_str(),msg.message.c_str());
        mqttModel->update(msg.topic, msg.message); };
    /*  QTableView* tableView =  w.findChild<QTableView*>("tableView");
    if ( tableView ) {
        tableView->setModel(mqttModel);
        tableView->show();
    } else {
        WARN("tableWidget not found");
    }*/
    w->show();
    mainThread.start();

    return a.exec();
}



void commandArguments(JsonObject& config, int argc, char **argv) {
    int opt;

    while ((opt = getopt(argc, argv, "f:m:l:v:")) != -1) {
        switch (opt) {
        case 'm':
            config["mqtt"]["connection"] = optarg;
            break;
        case 'f':
            config["configFile"] = optarg;
            break;
        case 'v': {
            std::string logLevel;
            logLevel += optarg[0];
            config["log"]["level"] = logLevel;
            break;
        }
        case 'l':
            config["log"]["file"] = optarg;
            break;
        default: /* '?' */
            fprintf(stderr,
                    "Usage: %s [-v(TDIWE)] [-f configFile] [-l logFile] [-m "
                "mqtt_connection]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }
}
