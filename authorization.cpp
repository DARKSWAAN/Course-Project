#include "authorization.h"
#include "ui_authorization.h"
#include <mainwindow.h>
#include <QFileDialog>
#include <QTextStream>
#include <QMainWindow>
#include <QTextStream>
#include "error.h"

extern bool isMusic;



authorization::authorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::authorization)
{
    ui->setupUi(this);

}

authorization::~authorization()
{
    delete ui;
}

void authorization::on_back_clicked()
{
    clickSound(isMusic);
    MainWindow *mWindow;
    mWindow = new MainWindow;
    mWindow->show();
    this->close();
}



void authorization::on_playNoReg_clicked()
{
    clickSound(isMusic);
    gamepreparation *auth;
    auth = new gamepreparation;
    auth->show();
    this->close();

}


void authorization::on_reg_clicked()
{
    clickSound(isMusic);
    registration *reg;
    reg = new registration;
    reg->show();
    this->close();
}






void authorization::on_play_clicked()
{
    clickSound(isMusic);
}


//Alex
void authorization::on_not_reg_clicked()
{
    clickSound(isMusic);
    gamepreparation *auth2;
    auth2 = new gamepreparation;
    auth2->show();
    this->close();
}


//Samira

bool rightEntrancy = true;
QString Users_name;

void authorization::Entrancy()
{
    bool error = false;
    Error * ex;
    ex = new Error;
    //rightEntrancy = true;
    registration *reg;
    reg = new registration;


    QString pass_correct;
    QString Login_correct;
    QString login = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    Users_name = login;

    QFile fileOut("Baza.json");
   if( fileOut.open(QIODevice::ReadOnly | QIODevice::Text ))
   {
      qDebug() << "Файл існує 1";

        QJsonDocument json= QJsonDocument().fromJson(fileOut.readAll());
        qDebug() << json;

        QJsonArray arrayJson = json.array();
        for (int i=0; i < arrayJson.size();i++)
        {
            qDebug() << arrayJson[i];
            QJsonObject jobj = arrayJson[i].toObject();
            QJsonValue    _login = jobj["login"];
            QJsonValue   _password = jobj["password"];
            Login_correct = _login.toString();
            pass_correct = _password.toString();
            qDebug() << Login_correct;
            qDebug() << pass_correct;

            if (Login_correct == login && pass_correct ==password)
                        {
                                     error = true;

                        }
        }
   }
   rightEntrancy = error;
   if (error == false)
   {
        QMessageBox::information(reg, "Помилка", "Логін чи пароль введені не вірно!");
   }

}

void authorization::on_done_clicked()
{
    // Перевірка на латинські букви імені
    QString playerName = ui->lineEdit->text();
    QString password = ui->lineEdit_2 ->text();

    int errorCode =  ExeptionOn_done_clicked(playerName, password);
    Error * er;
    er = new Error;
    er->getErrorCode(errorCode);

    switch (er->getErrorCode(errorCode)) {
    case 301:
        ui->lineEdit->setText("");
        return;

    case 302:
        ui->lineEdit_2->setText("");
        return;

    case 303:
        ui->lineEdit_2->setText("");
        return;

    case 306:
        ui->lineEdit->setText("");
        return;

    case 307:
        ui->lineEdit_2->setText("");
        return;

    case 304:
        return;

    default:
        break;
    }

    Entrancy();
    if(rightEntrancy == false)
    {
        return;
    }

   // Entrancy();
    gamepreparation *auth;
    auth = new gamepreparation;
    auth->show();
    this->close();
}
