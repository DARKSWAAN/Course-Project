#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QWidget>
#include <QMainWindow>
#include "Box2D/Box2D.h"
#include "pause.h"
#include "mainwindow.h"

#include "ball.h"
#include "wall.h"
#include "player.h"
#include "bonus.h"

#include <QGraphicsScene>
#include <QGraphicsItem>

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>    // walls

#include <QGraphicsPixmapItem>  // players

#include <QKeyEvent>
#include <QTimer>
#include <QString>
#include "sound.h"

#define SCALE 100;
#define PlayerSpeed 5;
#define PlayerHeightJump 7;


class Scene : public QGraphicsScene, public sound {
    Q_OBJECT;
public:
    Scene(qreal x, qreal y, qreal width, qreal height, b2World *world);
public slots:
    void advance();
private:
    b2World* world;

    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

};



namespace Ui {
class GameScene;
}

class GameScene : public QWidget, public sound
{
    Q_OBJECT

public:
    explicit GameScene(QWidget *parent = nullptr);
    ~GameScene();

public slots:
    void score();
    void ballSleep();
    void endGame();
    void startGame();

private slots:
    void on_startNewGame_clicked();
    void on_pauseGame_clicked();
    void generateNewBonus();
    void Save_record();
    void read_rec();


private:
    Ui::GameScene *ui;
    Pause* pause;
    Scene* scene;
    QTimer* frameTimer;
    QTimer* scoreTimer;
    QTimer* generateBonus;
    b2World* world;

};


#endif // GAMESCENE_H
