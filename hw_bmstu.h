#include <QApplication>
#include <QPushButton>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QThread>
#include <QMutex>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QFileSystemWatcher>
#include "fileworker.h"
#include "cheking.h"

// Главное окно
class MainWindow : public QWidget {

 public:
  MainWindow() {
    setFixedSize(970, 500);
    setWindowTitle("Список компьютеров");

    QVBoxLayout *layout = new QVBoxLayout(this);

    table = new QTableWidget(this);
    table->setColumnCount(9);
    table->setHorizontalHeaderLabels({"", "Тип", "Номер", "Поставщик", "Модель", "MAC", "ОС", "Дата производства", "Дата окончания срока службы"});
    layout->addWidget(table);

    QPushButton *addButton = new QPushButton("Добавить компьютер", this);
    layout->addWidget(addButton);

    threadforfile = new QThread(this);
    fileWorker = new FileWorker("computers.json");
    fileWorker->moveToThread(threadforfile);
    threadforfile->start();

    fileWatcher = new QFileSystemWatcher(this);
    fileWatcher->addPath("computers.json");

    connect(fileWatcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::updateTable);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::AddComp);

    // Загрузка начальных данных в таблицу
    updateTable();
  }

  ~MainWindow() override {
    threadforfile->quit();
    threadforfile->wait();
    delete fileWorker;
  }

 private:
  void updateTable() {
    QMetaObject::invokeMethod(fileWorker, [this]() {
      QJsonArray computers = fileWorker->getComputers();
      uniqueComputers.initialize(computers); // Инициализация уникальных записей
      computers = uniqueComputers.removeDuplicates(computers);
      QMetaObject::invokeMethod(this, [this, computers]() {
        populateTable(computers);
      });
    });
  }

  void populateTable(const QJsonArray &computers) {
    table->clearContents();
    table->setRowCount(computers.size());

    for (int i = 0; i < computers.size(); ++i) {
      QJsonObject computer = computers[i].toObject();

      // Добавляем кнопку управления в первую колонку
      QPushButton *manageButton = new QPushButton("Управление", table);
      table->setCellWidget(i, 0, manageButton);
      connect(manageButton, &QPushButton::clicked, [this, i, computers]() {
        ManageComp(i, computers);
      });

      table->setItem(i, 1, new QTableWidgetItem(computer["type"].toString()));
      table->setItem(i, 2, new QTableWidgetItem(computer["number"].toString()));
      table->setItem(i, 3, new QTableWidgetItem(computer["supplier"].toString()));
      table->setItem(i, 4, new QTableWidgetItem(computer["model"].toString()));
      table->setItem(i, 5, new QTableWidgetItem(computer["mac"].toString()));
      table->setItem(i, 6, new QTableWidgetItem(computer["os"].toString()));
      table->setItem(i, 7, new QTableWidgetItem(computer["production_date"].toString()));
      table->setItem(i, 8, new QTableWidgetItem(computer["end_of_life_date"].toString()));
    }
    // Автоматическая подгонка ширины колонок
    table->resizeColumnsToContents();
  }

  void AddComp() {
    QDialog dialogAdd(this);
    dialogAdd.setWindowTitle("Добавить компьютер");
    dialogAdd.setFixedSize(400, 300);

    QFormLayout *formLayout = new QFormLayout(&dialogAdd);

    QLineEdit *typeEdit = new QLineEdit();
    QLineEdit *numberEdit = new QLineEdit();
    QLineEdit *supplierEdit = new QLineEdit();
    QLineEdit *modelEdit = new QLineEdit();
    QLineEdit *macEdit = new QLineEdit();
    QLineEdit *osEdit = new QLineEdit();
    QLineEdit *prodDateEdit = new QLineEdit();
    QLineEdit *eolDateEdit = new QLineEdit();

    formLayout->addRow("Тип:", typeEdit);
    formLayout->addRow("Номер:", numberEdit);
    formLayout->addRow("Поставщик:", supplierEdit);
    formLayout->addRow("Модель:", modelEdit);
    formLayout->addRow("MAC:", macEdit);
    formLayout->addRow("ОС:", osEdit);
    formLayout->addRow("Дата производства:", prodDateEdit);
    formLayout->addRow("Дата окончания срока службы:", eolDateEdit);

    QPushButton *saveButton = new QPushButton("Сохранить");
    formLayout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, [&]() {
      QString newMac = macEdit->text();
      QString newNumber = numberEdit->text();
      // Проверяем уникальность MAC-адреса и номера
      if (!uniqueComputers.isUnique(newMac, newNumber)) {
        QMessageBox::warning(this, "Ошибка", "Компьютер с таким MAC-адресом или номером уже существует.");
        return;
      }

      QJsonObject newComputer;
      newComputer["type"] = typeEdit->text();
      newComputer["number"] = newNumber;
      newComputer["supplier"] = supplierEdit->text();
      newComputer["model"] = modelEdit->text();
      newComputer["mac"] = newMac;
      newComputer["os"] = osEdit->text();
      newComputer["production_date"] = prodDateEdit->text();
      newComputer["end_of_life_date"] = eolDateEdit->text();

      QMetaObject::invokeMethod(fileWorker, [this, newComputer]() {
        QJsonArray computers = fileWorker->getComputers();
        computers.append(newComputer);
        fileWorker->saveComputers(computers);
        QMetaObject::invokeMethod(this, [this, newComputer]() {
          uniqueComputers.add(newComputer["mac"].toString(), newComputer["number"].toString());
          updateTable();
        });
      });
      dialogAdd.close();
    });

    dialogAdd.exec();
  }

  void ManageComp(int index, const QJsonArray &computers) {
    QDialog dialogManage(this);
    dialogManage.setWindowTitle("Управление компьютером");
    dialogManage.setFixedSize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialogManage);

    QPushButton *deleteButton = new QPushButton("Удалить");
    QPushButton *editButton = new QPushButton("Изменить");

    layout->addWidget(deleteButton);
    layout->addWidget(editButton);

    connect(deleteButton, &QPushButton::clicked, [&]() {
      QJsonArray updatedComputers = computers;
      updatedComputers.removeAt(index);
      saveDataToFile(updatedComputers);
      dialogManage.close();
    });

    connect(editButton, &QPushButton::clicked, [&]() {
      dialogManage.close();
      EditComp(index, computers);
    });

    dialogManage.exec();
  }

  void EditComp(int index, const QJsonArray &computers) {
    QDialog dialogEdit(this);
    dialogEdit.setWindowTitle("Редактировать компьютер");
    dialogEdit.setFixedSize(400, 300);

    QFormLayout *formLayout = new QFormLayout(&dialogEdit);

    QJsonObject computer = computers[index].toObject();

    QLineEdit *typeEdit = new QLineEdit(computer["type"].toString());
    QLineEdit *numberEdit = new QLineEdit(computer["number"].toString());
    QLineEdit *supplierEdit = new QLineEdit(computer["supplier"].toString());
    QLineEdit *modelEdit = new QLineEdit(computer["model"].toString());
    QLineEdit *macEdit = new QLineEdit(computer["mac"].toString());
    QLineEdit *osEdit = new QLineEdit(computer["os"].toString());
    QLineEdit *prodDateEdit = new QLineEdit(computer["production_date"].toString());
    QLineEdit *eolDateEdit = new QLineEdit(computer["end_of_life_date"].toString());

    formLayout->addRow("Тип:", typeEdit);
    formLayout->addRow("Номер:", numberEdit);
    formLayout->addRow("Поставщик:", supplierEdit);
    formLayout->addRow("Модель:", modelEdit);
    formLayout->addRow("MAC:", macEdit);
    formLayout->addRow("ОС:", osEdit);
    formLayout->addRow("Дата производства:", prodDateEdit);
    formLayout->addRow("Дата окончания срока службы:", eolDateEdit);

    QPushButton *saveButton = new QPushButton("Сохранить");
    formLayout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, [&]() {
      QString newMac = macEdit->text();
      QString newNumber = numberEdit->text();
      // Проверяем уникальность, игнорируя текущий элемент
      if ((newMac != computer["mac"].toString() || newNumber != computer["number"].toString()) &&
          !uniqueComputers.isUnique(newMac, newNumber)) {
        QMessageBox::warning(this, "Ошибка", "Компьютер с таким MAC-адресом или номером уже существует.");
        return;
      }

      QJsonObject updatedComputer;
      updatedComputer["type"] = typeEdit->text();
      updatedComputer["number"] = newNumber;
      updatedComputer["supplier"] = supplierEdit->text();
      updatedComputer["model"] = modelEdit->text();
      updatedComputer["mac"] = newMac;
      updatedComputer["os"] = osEdit->text();
      updatedComputer["production_date"] = prodDateEdit->text();
      updatedComputer["end_of_life_date"] = eolDateEdit->text();

      QJsonArray updatedComputers = computers;
      updatedComputers[index] = updatedComputer;
      saveDataToFile(updatedComputers);
      dialogEdit.close();
    });

    dialogEdit.exec();
  }

  void saveDataToFile(const QJsonArray &computers) {
    QMetaObject::invokeMethod(fileWorker, [this, computers]() {
      fileWorker->saveComputers(computers);
      QMetaObject::invokeMethod(this, &MainWindow::updateTable);
    });
  }

  QThread *threadforfile;
  FileWorker *fileWorker;
  QTableWidget *table = nullptr;
  QFileSystemWatcher *fileWatcher = nullptr;
  UniqueComputers uniqueComputers;
};
