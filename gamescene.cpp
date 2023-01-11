#include "gamescene.h"
#include "ui_gamescene.h"
#include <mainwindow.h>
#include "registration.h"
#include "authorization.h"
#include "exception.h"
#include "registration.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QMessageBox>

Player_1* pl1;
Player_2* pl2;
Ball* gameBall;

Bonus* gameBonus;

b2Body* ballBody;
b2Body* PlayerBody1;
b2Body* PlayerBody2;

int player_1_Score = 0;             // Счёт первого игрока
int player_2_Score = 0;             // Счёт второго игрока
int ScoreToWin = 3;

bool is_player_1_goal = false;      // true - первый (левый) игрок забил, false - второй (правый) игрок забил
bool is_created = true;             // Создан ли мяч? true потому что в начале он создан
bool is_paused = false;             // Стоит ли пауза. Изначально не стоит - false
bool bonusCreated = false;          // Чи створен Бонус

int plr_1_Speed = PlayerSpeed;              // Швидкість гравця
int plr_1_HeightJump = PlayerHeightJump;    // Высота стрибка

int plr_2_Speed = PlayerSpeed;
int plr_2_HeightJump = PlayerHeightJump;

int counterPlayer1 = 0;             // double jump for Player1     200 row
int counterPlayer2 = 0;             // double jump for Player2

extern qreal ballSize;              // приходит с gamepreparation.cpp
extern qreal exitBool;              // приходит с паузы
extern qreal continueBool;          // приходит с паузы

extern QString gameMapPath;         // Карта
extern QString player1SkinPath;     // Шлях до модельки гравця 1
extern QString player2SkinPath;     // Шлях до модельки гравця 2
extern QString ballSkinPath;        // Шлях до модельки м'яча

extern std:: vector<std::pair<QString, QString>> db;
extern QString Users_name;
extern QString Users_name_reg;
extern std:: vector<std::pair<int, QString>> rec;


extern bool isMusic;

qreal fromB2(qreal value) {
    return value * SCALE;
}

qreal toB2(qreal value) {
    return value/SCALE;
}


GameScene::GameScene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameScene)
{

    ui->setupUi(this);
    ui->winLabel->hide();
    ui->startNewGame->hide();
    setAttribute(Qt::WA_DeleteOnClose);//alex

    world = new b2World(b2Vec2(0.00f, 10.00f));



    scene = new Scene(0, 0, 10, 6, world);      // x, y, width, height

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setStyleSheet(gameMapPath);

//    scene->addRect(scene->sceneRect());                   // Квадрат сцени

    scene->addItem(new Wall(world, QSizeF(10, 0.1), QPointF(5, 5.95), 0));    // Підлога
    scene->addItem(new Wall(world, QSizeF(10, 0.1), QPointF(5, -8), 0));    // Потолок
    scene->addItem(new Wall(world, QSizeF(20, 0.15), QPointF(0, 3), 90));       // Стіна зліва
    scene->addItem(new Wall(world, QSizeF(20, 0.1), QPointF(10, 3), 90));       // Стіна справа
    scene->addItem(new Wall(world, QSizeF(4, 0.2), QPointF(5, 4.5), 90));      // Перегородка по центру (сітка)

    pl1 = new Player_1(world, QPointF(1, 5.40), player1SkinPath);
    pl2 = new Player_2(world, QPointF(7, 5.40), player2SkinPath);
    scene->addItem(pl1);
    scene->addItem(pl2);

    gameBall = new Ball(world, ballSize, QPointF(5, 0.5), ballSkinPath);
    scene->addItem(gameBall);


    generateBonus = new QTimer(this);
    connect(generateBonus, SIGNAL(timeout()),
            this, SLOT(generateNewBonus()));
    generateBonus->start(30000);

    frameTimer = new QTimer(this);
    connect(frameTimer, SIGNAL(timeout()),
            scene, SLOT(advance()));
    frameTimer->start(1000/60);



    scoreTimer = new QTimer(this);
    connect(scoreTimer, SIGNAL(timeout()),
            this, SLOT(score()));
    scoreTimer->start(1000);

}

GameScene::~GameScene()
{
    delete ui;
    delete scene;
    delete world;
}

void GameScene::read_rec()
{
    int _userRecord ;
    QString _login ;
    QFile fileOut("record.json");
   if( fileOut.open(QIODevice::ReadOnly | QIODevice::Text ))
   {

        QJsonDocument json= QJsonDocument().fromJson(fileOut.readAll());
        qDebug() << "read_rec is open";
        QJsonArray arrayJson = json.array();

        for (int i=0; i < arrayJson.size();i++)
        {
            qDebug() << arrayJson[i];
            QJsonObject jobj = arrayJson[i].toObject();
            QJsonValue    userRecord = jobj["_userRecord"];
            QJsonValue    login = jobj["login"];
            _login = login.toString();
            _userRecord = userRecord.toInt();
            qDebug() << _login;
            qDebug() << _userRecord;

            if(rec.size() == 0)
            {
                rec.push_back({_userRecord, _login});
                qDebug() << "rec.size == 0";
            }
        }

        for (int i = 0; i < rec.size(); i++)
        {
            QString login;
            int _userRecord;

            if (rec[i].second == Users_name || rec[i].second == Users_name_reg)
            {
                login = rec[i].second;
                _userRecord = rec[i].first;
                _userRecord++;
                rec.erase(rec.begin() + i);
                qDebug() <<  Users_name;
                rec.push_back({ _userRecord, login});
                break;
            }
        }
     }

    Save_record();
    QApplication::quit();
    fileOut.close();
}
void GameScene::Save_record() // зберігає вектор з рекордами у файл
{
    QFile fileOut("record.json");
    if (fileOut.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "Save_record is open";
        QJsonArray arrayJson ;
        for (int i = 0; i < rec.size(); i++)
        {
            QJsonObject jobj;
            jobj.insert("_userRecord",QJsonValue::fromVariant(rec[i].first));
            jobj.insert("login",QJsonValue::fromVariant(rec[i].second));
            arrayJson.append(jobj);
        }
         fileOut.write(QJsonDocument(arrayJson).toJson(QJsonDocument::Indented));
    }

}

void GameScene::score() {

    if(player_1_Score == ScoreToWin) {

        read_rec();
        ui->scorePlayer1->setText(QString::number(player_1_Score));
        ui->winLabel->setText(QString("Gratz Team Red WIN"));
        ui->winLabel->show();
        ui->startNewGame->show();
        endGame();
        is_created = true;





        // Сброс бонусів
        plr_1_Speed = PlayerSpeed;
        plr_1_HeightJump = PlayerHeightJump;
        plr_2_Speed = PlayerSpeed;
        plr_2_HeightJump = PlayerHeightJump;
    } else if(player_2_Score == ScoreToWin) {
        ui->scorePlayer2->setText(QString::number(player_2_Score));
        ui->winLabel->setText(QString("Gratz Team Blue WIN"));
        ui->winLabel->show();
        ui->startNewGame->show();
        endGame();
        is_created = true;

        // Сброс бонусів
        plr_1_Speed = PlayerSpeed;
        plr_1_HeightJump = PlayerHeightJump;
        plr_2_Speed = PlayerSpeed;
        plr_2_HeightJump = PlayerHeightJump;
    }

    if(!is_created && is_player_1_goal && !is_paused) {                   // is_created = false когда был гол и мячик удалился в Ball::advance() | первый игрок забил
        is_created = true;
        gameBall = new Ball(world, ballSize, QPointF(3, 2), ballSkinPath);
        ballBody->SetType(b2_staticBody);
        QTimer::singleShot(1000, this, SLOT(ballSleep()));
        scene->addItem(gameBall);
        ui->scorePlayer1->setText(QString::number(player_1_Score));
        qDebug()  << "Score: " << player_1_Score << " : " << player_2_Score;

        // Сброс бонусів
        plr_1_Speed = PlayerSpeed;
        plr_1_HeightJump = PlayerHeightJump;
        plr_2_Speed = PlayerSpeed;
        plr_2_HeightJump = PlayerHeightJump;
    }
    else if(!is_created && !is_player_1_goal && !is_paused) {             // второй игрок забил
        is_created = true;
        gameBall = new Ball(world, ballSize, QPointF(7, 2), ballSkinPath);
        ballBody->SetType(b2_staticBody);
        QTimer::singleShot(1000, this, SLOT(ballSleep()));
        scene->addItem(gameBall);
        ui->scorePlayer2->setText(QString::number(player_2_Score));
        qDebug()  << "Score: " << player_1_Score << " : " << player_2_Score;

        // Сброс бонусів
        plr_1_Speed = PlayerSpeed;
        plr_1_HeightJump = PlayerHeightJump;
        plr_2_Speed = PlayerSpeed;
        plr_2_HeightJump = PlayerHeightJump;
    }
}


void GameScene::ballSleep() {
    if(!is_paused) {
        ballBody->SetType(b2_dynamicBody);
    }
}


void GameScene::endGame() {
    PlayerBody1->SetType(b2_staticBody);
    PlayerBody2->SetType(b2_staticBody);
    is_paused = true;
}


void GameScene::startGame() {
    ui->startNewGame->hide();
    ui->winLabel->hide();
    PlayerBody1->SetType(b2_dynamicBody);
    PlayerBody2->SetType(b2_dynamicBody);

    // Обновление счёта
    player_1_Score = 0;
    player_2_Score = 0;
    ui->scorePlayer1->setText(QString::number(player_1_Score));
    ui->scorePlayer2->setText(QString::number(player_2_Score));

    is_paused = false;
    is_created = false;

}


bool keyAPressed = false;
bool keyDPressed = false;

bool keyLeftPressed = false;
bool keyRightPressed = false;

void Scene::keyPressEvent(QKeyEvent *event) {
    // Player 1
       b2Vec2 pos = PlayerBody1->GetPosition();
       b2Vec2 vel = PlayerBody1->GetLinearVelocity();

    // Player 2
       b2Vec2 pos2 = PlayerBody2->GetPosition();
       b2Vec2 vel2 = PlayerBody2->GetLinearVelocity();

    switch(event->key()) {
    case Qt::Key_A:
            vel.x = -plr_1_Speed;
            keyAPressed = true;
        break;
    case Qt::Key_D:


        if(pos.x > 0 && pos.x < 4.3658) {   // Что бы он не смог двигаться по перегородке
            vel.x = plr_1_Speed;
            keyDPressed = true;
        }
        break;

    case Qt::Key_W:

        // counter обновляется в Player_1::advance
        if(pos.y > 2 && counterPlayer1 < 2)
        {
            vel.y = -plr_1_HeightJump;   // Высота прыжка
            counterPlayer1++;
            jumpSound(isMusic);
        }

        break;

    case Qt::Key_Left:
        if(pos2.x < 10 && pos2.x > 5.63) {   // Что бы он не смог двигаться по перегородке  // зацепка
            vel2.x = -plr_2_Speed;
            keyLeftPressed = true;
        }
        break;

    case Qt::Key_Right:
            vel2.x = plr_2_Speed;
            keyRightPressed = true;
        break;

    case Qt::Key_Up:

        if(pos2.y > 2 && counterPlayer2 < 2)
        {
            vel2.y = -plr_2_HeightJump;   // Высота прыжка
            counterPlayer2++;
            jumpSound(isMusic);
        }
        break;
    }
    PlayerBody1->SetLinearVelocity(vel);
    PlayerBody2->SetLinearVelocity(vel2);
}



void Scene::keyReleaseEvent(QKeyEvent * event)
{
    // Player 1
       b2Vec2 pos = PlayerBody1->GetPosition();
       b2Vec2 vel = PlayerBody1->GetLinearVelocity();

    // Player 2
       b2Vec2 pos2 = PlayerBody2->GetPosition();
       b2Vec2 vel2 = PlayerBody2->GetLinearVelocity();


       // player 1
       if(event->key() == Qt::Key_A && !keyDPressed) {
           vel.x = 0;
       }
       else if(event->key() == Qt::Key_D && !keyAPressed) {
           if(pos.x > 0 && pos.x < 4.3658) {   // Что бы он не смог двигаться по перегородке
                vel.x = 0;
           }
       }

       // player 2
       if(event->key() == Qt::Key_Left && !keyRightPressed) {
           if(pos2.x < 10 && pos2.x > 5.63) {   // Что бы он не смог двигаться по перегородке
               vel2.x = 0;
           }
       }
       else if(event->key() == Qt::Key_Right && !keyLeftPressed) {
            vel2.x = 0;
       }

       switch(event->key()) {
       case Qt::Key_A:
                keyAPressed = false;
            break;
       case Qt::Key_D:
                keyDPressed = false;
            break;
       case Qt::Key_Left:
           keyLeftPressed = false;
           break;
       case Qt::Key_Right:
           keyRightPressed = false;
           break;

        }
    PlayerBody1->SetLinearVelocity(vel);
    PlayerBody2->SetLinearVelocity(vel2);
}


Scene::Scene(qreal x, qreal y, qreal width, qreal height, b2World *world)
    : QGraphicsScene(fromB2(x), fromB2(y), fromB2(width), fromB2(height))
{
    this->world = world;
}


void Scene::advance() {
    world->Step(1.00f/60.00, 6, 2);
    QGraphicsScene::advance();
}


void GameScene::on_startNewGame_clicked()
{
    startGame();
}


b2Vec2 pastBallVel;
b2Vec2 pastPlayer1Vel;
b2Vec2 pastPLayer2Vel;

void GameScene::on_pauseGame_clicked()
{
    if(PlayerBody1->GetType() == b2_dynamicBody && PlayerBody2->GetType() == b2_dynamicBody) {      // paused
        is_paused = true;
        pastBallVel = ballBody->GetLinearVelocity();
        pastPlayer1Vel = PlayerBody1->GetLinearVelocity();
        pastPLayer2Vel = PlayerBody2->GetLinearVelocity();

        PlayerBody1->SetType(b2_staticBody);
        PlayerBody2->SetType(b2_staticBody);
        ballBody->SetType(b2_staticBody);

        if(bonusCreated)
        gameBonus->yspeed = 0;
//Alex
        hide();
        pause = new Pause();
        pause->setModal(true);
        pause->exec();
       if(exitBool) {
           player_1_Score = 0;
           player_2_Score = 0;
           ui->scorePlayer1->setText(QString::number(player_1_Score));
           ui->scorePlayer2->setText(QString::number(player_2_Score));
           scene->clear();
           exitBool= false;
           is_paused = false;
       this->close();

}

if(continueBool) {
    this->show();
//Alex
        is_paused = false;
               PlayerBody1->SetType(b2_dynamicBody);
               PlayerBody2->SetType(b2_dynamicBody);
               ballBody->SetType(b2_dynamicBody);

               ballBody->SetLinearVelocity(pastBallVel);
               PlayerBody1->SetLinearVelocity(pastPlayer1Vel);
               PlayerBody2->SetLinearVelocity(pastPLayer2Vel);

               if(bonusCreated)
               gameBonus->yspeed = 2;
               continueBool = false;

               //Alex

}
    }
        }

void GameScene::generateNewBonus()
{
    if(!is_paused) {
        gameBonus = new Bonus(scene->sceneRect().width());
        scene->addItem(gameBonus);
        bonusCreated = true;
    }
}








