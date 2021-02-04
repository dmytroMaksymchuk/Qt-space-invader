#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include "keymanager.h"
#include "consts.h"
#include "utils/timer.h"
#include "utils/coordpair.h"
#include "checktext.h"
#include "animatedobject.h"

class SpaceShip : public AnimatedObject
{
    Q_OBJECT
public:
    explicit SpaceShip(QObject *parent = nullptr, QString imagePath = nullptr);
    ~SpaceShip() override;
    virtual void addToScene(QGraphicsScene* scene);
    Side side;
    static bool checkTextVisible;

public slots:
    virtual void onTimer() = 0;
    void toggleCheckText();

signals:

protected:
    QGraphicsItem* collisionCheck(int typeIndex);
    void createBullet();
    int bulletCollisionCheck();

    void setUpCheckText();

    virtual void groupCheckTextInfo() = 0;

    CoordPair speed = CoordPair(0,0);
    CoordPair prevSpeed = CoordPair(0,0);
    float bulletSpeed;
    const int baseShootDelay = 2500;
    bool shootAvl = false;
    CheckText *checkText;
    Timer* mainTimer;
    int shootDelay;

private:
    bool checkTextOnScene = false;

private slots:
    void shootIsAvl();
};

#endif // SPACESHIP_H
