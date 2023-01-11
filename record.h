#ifndef RECORD_H
#define RECORD_H

#include <QDialog>
#include <QString>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument >
#include <QJsonArray>
#include <QFile>
#include "sound.h"


namespace Ui {
class record;
}

class record : public QDialog, public sound
{
    Q_OBJECT

public:
    explicit record(QWidget *parent = nullptr);
    ~record();

private slots:
    void on_back_clicked();

    void on_show_json_clicked();

    void on_show_sql_clicked();

private:
    Ui::record *ui;

    void Save_xml();

    void Read_xml();
};

#endif // RECORD_H
