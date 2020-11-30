#ifndef MYRECT_H
#define MYRECT_H

#include <QGraphicsRectItem>
#include <QGraphicsObject>
#include "spaceship.h"
#include "consts.h"
#include "keymanager.h"


class Hero: public SpaceShip{

    Q_OBJECT
public:
    Hero(QString imagePath = nullptr, KeyManager* keyManager = nullptr);
    int type() const override {return TypeIndex::hero;}  
    void resume() override final;
    void pause() override final;

signals:
    void heroKilled();

public slots:
    void onTimer() override;
    void groupCheckTextInfo() override;
    void heroKeyPressed(int key);
    void heroKeyReleased(int key);

private:
    bool enemyCollisionCheck();
    int calculateMovement(char coord);
    bool checkScreenBorders(int distance);
    KeyManager* keyManager;
    CoordPair engineAccel = CoordPair(0,0);
    QMap<int, bool> keyPressed = {{Qt::Key_Left, false}, {Qt::Key_Right, false}, {Qt::Key_Up, false}, {Qt::Key_Down, false}};
    QMap<int, int> oppositeKey = {{Qt::Key_Left, Qt::Key_Right}, {Qt::Key_Right, Qt::Key_Left},
                                  {Qt::Key_Up, Qt::Key_Down}, {Qt::Key_Down, Qt::Key_Up}};
    bool active = false;

    struct Movement {
        const CoordPair maxVelocity = CoordPair(200, 150);         //pixels per second
        const CoordPair engineAccel = CoordPair(1000, 500);
        const CoordPair friction = CoordPair(0, 200);
    };
    static const Movement movement;

};

#endif // MYRECT_H
