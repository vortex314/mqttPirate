QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += no_keywords

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    configuration.cpp \
    main.cpp \
    mainwindow.cpp \
    mqttlinegraph.cpp \
    mqttlistener.cpp \
    mqttmodel.cpp \
    ../limero/linux/*.cpp \
    mqtttable.cpp \
    mqtttext.cpp

HEADERS += \
    configuration.h \
    mainwindow.h \
    mqttlinegraph.h \
    mqttlistener.h \
    mqttmodel.h \
    mqtttable.h \
    mqtttext.h

FORMS += \
    mainwindow.ui

DEFINES += LINUX

LIBS += -L"../paho.mqtt.c/build/output" -l:libpaho-mqtt3c.a

INCLUDEPATH += ../limero/inc ../limero/linux ../ArduinoJson/src ../paho.mqtt.c/src

DISTFILES += \
    mqttPirate.json


