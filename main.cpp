#include "mainwindow.h"

#include <QApplication>
#include <QTableView>
#include <QTableWidget>
#include <QLayout>
#include <QGridLayout>
#include <QVBoxLayout>
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

const char* ContentType(YAML::Node& node){
    YAML::NodeType::value type=node.Type();
    const char* types[]={"Undefined", "Null", "Scalar", "Sequence", "Map"};
    return types[type];
}


bool endsWith (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void commandArguments(JsonObject& config, int argc, char **argv) ;
Log logger(5120);
StaticJsonDocument<100000> doc;
JsonObject config;
Thread mainThread("main");
MqttPaho mqtt(mainThread);
MqttModel* mqttModel;

int main(int argc, char *argv[])
{
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
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(2,2,2,2);

    QWidget *w = new QWidget();
    w->setLayout(vLayout);
    w->setWindowTitle("Grid Layouts (12x6)");
    Configuration::load(doc,config,"/home/lieven/workspace/mqttPirate/mqttPirate.json");

    try {
        YAML::Node cfg = YAML::LoadFile("/home/lieven/workspace/mqttPirate/config.yml");

        YAML::Node dashboard=cfg["dashboard"];
        INFO(" %s ",ContentType(cfg));
        for (YAML::const_iterator it=dashboard.begin();it!=dashboard.end();++it) {
            std::string key = it->first.as<std::string>();
            if ( key.compare("row")==0 && it->second.IsSequence()) {
                QHBoxLayout* hLayout=new QHBoxLayout();
                vLayout->addLayout(hLayout);
                YAML::Node row=it->second;
                for (YAML::const_iterator rowItem=row.begin();rowItem!=row.end();++rowItem){
                    std::string itemName= rowItem->as<std::string>();
                    INFO(" %s",itemName.c_str());
                    YAML::Node itemCfg=cfg[itemName];
                    if ( endsWith(itemName,"Chart")) {
                        MqttLineGraph* mqttGraph=new MqttLineGraph();
                        mqttGraph->config(itemCfg);
                        mqtt.incoming>>mqttGraph;
                        hLayout->addWidget(mqttGraph);
                    } else  if ( endsWith(itemName,"Button")) {
                        QPushButton *button=new QPushButton();
                        std::string label =itemCfg["label"].as<std::string>();
                        button->setText(label.c_str());
                        hLayout->addWidget(button);
                    }
                }
                hLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
            }
        }
        vLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding));
    }  catch (std::exception& ex) {
        INFO("Exception : %s ",ex.what());
    }


    //    w->setFixedSize(500,500);
/*

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
            gridLayout->addWidget(mqttGraph);
        } else if ( type.compare("Text")==0) {
            INFO("add %s",type.c_str());
            MqttText* mqttText=new MqttText();
            mqttText->config(item);
            mqtt.incoming>>mqttText;
            gridLayout->addWidget(mqttText);
        }else if ( type.compare("Table")==0) {
            INFO("add %s",type.c_str());
            MqttTable* mqttTable=new MqttTable(mqttModel);
            mqttTable->config(item);
            mqtt.incoming>>mqttTable;
            gridLayout->addWidget(mqttTable);
        } else if ( type.compare("Space")==0) {
            INFO("add %s",type.c_str());
            gridLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Maximum,QSizePolicy::Minimum));
        }
    }

    mqtt.incoming >>
            [&](const MqttMessage &msg) {
        //        INFO(" %s : %s ",msg.topic.c_str(),msg.message.c_str());
        mqttModel->update(msg.topic, msg.message); };*/
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
