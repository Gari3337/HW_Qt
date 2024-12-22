#include <QJsonArray>
#include <QJsonObject>
#include <QHash>
#include <QString>
#include <QMessageBox>

class UniqueComputers {
 public:
  UniqueComputers() = default;

  void initialize(const QJsonArray &computers) {
    macHash.clear();
    numberHash.clear();
    for (const QJsonValue &value : computers) {
      QJsonObject computer = value.toObject();
      QString mac = computer["mac"].toString();
      QString number = computer["number"].toString();
      macHash.insert(mac, true);
      numberHash.insert(number, true);
    }
  }

  bool isUnique(const QString &mac, const QString &number) const {
    return !macHash.contains(mac) && !numberHash.contains(number);
  }

  void add(const QString &mac, const QString &number) {
    macHash.insert(mac, true);
    numberHash.insert(number, true);
  }

  QJsonArray removeDuplicates(const QJsonArray &computers) const {
    QJsonArray uniqueComputersArray;
    QSet<QString> seenMacs;
    QSet<QString> seenNumbers;

    for (const QJsonValue &value : computers) {
      QJsonObject computer = value.toObject();
      QString mac = computer["mac"].toString();
      QString number = computer["number"].toString();

      if (seenMacs.contains(mac) || seenNumbers.contains(number)) {
        continue;
      }

      seenMacs.insert(mac);
      seenNumbers.insert(number);
      uniqueComputersArray.append(computer);
    }

    return uniqueComputersArray;
  }

 private:
  QHash<QString, bool> macHash;
  QHash<QString, bool> numberHash;
};

