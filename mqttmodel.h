#ifndef MQTTMODEL_H
#define MQTTMODEL_H


#include <Log.h>

#include <QTime>
#include <QTimer>
#include <QThread>
#include <QtCore/QAbstractTableModel>
#include <QtGui/QBrush>
#include <QtGui/QFont>
#include <map>
struct Entry {
  std::string topic;
  std::string message;
  QTime time;
  uint32_t count;
  bool operator<(const Entry &entry) const { return (topic < entry.topic); }
};

class MqttModel : public QAbstractTableModel {
  Q_OBJECT
  QTimer *_timer;
  QThread *_qthread;

  std::vector<Entry> table;
  enum { COL_TOPIC, COL_MESSAGE, COL_TIME, COL_COUNT };
  const uint32_t COLS = 4;
  const char *headers[4] = {"topic", "message", "time", "count"};

 public:
  MqttModel(QObject *parent = nullptr);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  void timerHit();
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  void update(std::string topic, std::string message);
  void sort(int col);
};

#endif // MQTTMODEL_H
