#include "record.h"
#include "ui_record.h"
#include "mainwindow.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>

extern bool isMusic;

record::record(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::record)
{
    ui->setupUi(this);
}

record::~record()
{
    delete ui;
}

void record::on_back_clicked()
{
    clickSound(isMusic);
    MainWindow *mWindow;
    mWindow = new MainWindow;
    mWindow->show();
    this->close();
}


void record::on_show_json_clicked()
{
    QString _login ;
    QString _password ;
    QFile fileOut("Baza.json");
   if( fileOut.open(QIODevice::ReadOnly | QIODevice::Text ))
   {


        ui->textEdit->clear();

        QJsonDocument json= QJsonDocument().fromJson(fileOut.readAll());
        qDebug() << json;

        QJsonArray arrayJson = json.array();
        for (int i=0; i < arrayJson.size();i++)
        {
         qDebug() << arrayJson[i];
        QJsonObject jobj = arrayJson[i].toObject();
        QJsonValue    login = jobj["login"];
        QJsonValue    password = jobj["password"];
     _login = login.toString();
     _password = password.toString();
     qDebug() << _login;
     qDebug() << _password;

        ui->textEdit->append(_login +   " " + _password);
        }
  }

  fileOut.close();
}

void record::Save_xml(){
   /* Открываем файл для Записи с помощью пути, указанного в lineEditWrite */

    QFile   name_file("myDataFile.xml");
      if (name_file.open(QIODevice::WriteOnly))
      {
          qDebug() << " удалось открыть файл xml";

       /* Создаем объект, с помощью которого осуществляется запись в файл */
       QXmlStreamWriter xmlWriter(&name_file);
       xmlWriter.setAutoFormatting(true);  // Устанавливаем автоформатирование текста
       xmlWriter.writeStartDocument();     // Запускаем запись в документ
       xmlWriter.writeStartElement("Members");   // Записываем первый элемент с его именем

       xmlWriter.writeStartElement("Data_Base");  // Записываем тег с именем для первого чекбокса

       /* Записываем также в тело этого элемента строку
        * */
       xmlWriter.writeCharacters("Samira");
       xmlWriter.writeEndElement();        // Закрываем тег


    // ========================================================
       xmlWriter.writeStartElement("exceptions");
       xmlWriter.writeCharacters("Liza");

       xmlWriter.writeEndElement();
    // ========================================================
       xmlWriter.writeStartElement("classes_and_architectures");
       xmlWriter.writeCharacters("Sasha");

       xmlWriter.writeEndElement();

    // ========================================================
       xmlWriter.writeStartElement("interface");
       xmlWriter.writeCharacters("Andrey");

       xmlWriter.writeEndElement();

    // ========================================================
       xmlWriter.writeStartElement("algorithms");
       xmlWriter.writeCharacters("Dima");

       xmlWriter.writeEndElement();


       /* Закрываем тег "resources"
        * */
       xmlWriter.writeEndElement();
       /* Завершаем запись в документ
        * */
       xmlWriter.writeEndDocument();
       }
       name_file.close();   // Закрываем файл
}

void record::Read_xml()
{   ui->textEdit_2->clear();

    QFile name_file("myDataFile.xml");
        if (!name_file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this,
                                 "Ошибка файла",
                                 "Не удалось открыть файл",
                                 QMessageBox::Ok);
        } else {

            /* Создается объект, с помощью которого осуществляется чтение из файла */
            QXmlStreamReader xmlReader;
            //QXmlStreamReader xml;
            xmlReader.setDevice(&name_file);
            xmlReader.readNext();   // Переходит к первому элементу в файле

            /* пока не достигнем конца документа
             * */

            while(!xmlReader.atEnd())
            {

                /* Проверяем, является ли элемент началом тега
                 * */
                if(xmlReader.isStartElement())
                    {

                       QString name = xmlReader.name().toString();

                        if (name == "Data_Base" )
                        {
                       QString memb1 = xmlReader.readElementText();
                        qDebug() << name;
                        qDebug() << memb1;
                        ui->textEdit_2->append(name + ": " + memb1);
                        }

                        if (name == "exceptions" )
                        {
                       QString memb1 = xmlReader.readElementText();
                        qDebug() << name;
                        qDebug() << memb1;
                        ui->textEdit_2->append(name + ": " + memb1);
                        }

                        if (name == "classes_and_architectures" )
                        {
                       QString memb1 = xmlReader.readElementText();
                        qDebug() << name;
                        qDebug() << memb1;
                        ui->textEdit_2->append(name + ": " + memb1);
                        }
                        if (name == "interface" )
                        {
                       QString memb1 = xmlReader.readElementText();
                        qDebug() << name;
                        qDebug() << memb1;
                        ui->textEdit_2->append(name + ": " + memb1);
                        }
                        if (name == "algorithms" )
                        {
                       QString memb1 = xmlReader.readElementText();
                        qDebug() << name;
                        qDebug() << memb1;
                        ui->textEdit_2->append(name + ": " + memb1);
                        }



                    }
                xmlReader.readNext();
            }

         name_file.close();
        }


}


void record::on_show_sql_clicked()
{
    Save_xml();
    Read_xml();
}

