#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "ui_registration.h"
#include "exception.h"
//#include "error.h"
#include <QWidget>
#include <QObject>
#include <QString>
#include <QMessageBox>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument >
#include <QJsonArray>
#include <vector>
#include <QFile>
#include "sound.h"

namespace Ui {
class registration;
}

class registration : public QWidget, public sound
{
    Q_OBJECT

public:
    explicit registration(QWidget *parent = nullptr);
    ~registration();

private slots:
    void on_playNoReg_clicked();

    void on_reg_clicked();

    void on_back_clicked();

    void funct_registr();
    void Save();
    void record();
    void Save_record();
    void read_db();
    void removing_file();
    void read_rec();



   

private:
    Ui::registration *ui;
};

#endif // REGISTRATION_H
