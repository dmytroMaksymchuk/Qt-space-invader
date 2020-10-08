#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsRectItem>
#include <QObject>
#include "enemymanager.h"
#include "Const.h"
#include "spaceship.h"

class Enemy: public SpaceShip{
    Q_OBJECT
public:
    Enemy(EnemyManager* manager, QString imagePath);
    ~Enemy();
    int type() const override{
        return Keys::enemyIndex;
    }

public slots:
    void onTimer() override;
    void setDifficulty(int difficulty);

private:
    const int baseBulletSpeed = -10;
    EnemyManager* manager;

    void move();

signals:
    void enemyOnBase();
};

#endif // ENEMY_H
