#pragma once
#include <QObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutex>

class FileWorker : public QObject {
  Q_OBJECT

 public:
  explicit FileWorker(const QString &filePath, QObject *parent = nullptr)
      : QObject(parent), filePath(filePath) {}

  QJsonArray getComputers() {
    QMutexLocker locker(&mutex);
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
      emit errorOccurred("Не удалось открыть файл.");
      return QJsonArray();
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject jsonObj = jsonDoc.object();
    return jsonObj["computers"].toArray();
  }

  void saveComputers(const QJsonArray &computers) {
    QMutexLocker locker(&mutex);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
      emit errorOccurred("Не удалось сохранить данные в файл.");
      return;
    }

    QJsonObject jsonObj;
    jsonObj["computers"] = computers;
    QJsonDocument jsonDoc(jsonObj);
    file.write(jsonDoc.toJson());
    file.close();
  }

  signals:
      void errorOccurred(const QString &errorMessage);

 private:
  QString filePath;
  QMutex mutex;
};
