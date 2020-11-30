#include "hero.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include "bullet.h"
#include "enemy.h"
#include <QTimer>
#include "keymanager.h"

const Hero::Movement Hero::movement;

Hero::Hero(QString imagePath, KeyManager* keyManager) : SpaceShip(nullptr, imagePath)
{
    shootDelay = 1000;
    bulletSpeed = 0.2 * period_ms; // 10 pixels per 50 ms
    speed = CoordPair(0,0);
    side = Side::hero;
    this->pause();
    this->hide();

    setUpDelay(shootDelay);

    this->keyManager = keyManager;
    connect(keyManager, SIGNAL(heroKeyPressed(int)), this, SLOT(heroKeyPressed(int)));
    connect(keyManager, SIGNAL(heroKeyReleased(int)), this, SLOT(heroKeyReleased(int)));
    connect(keyManager, SIGNAL(logKeyPressed(bool)), this, SLOT(toggleCheckText(bool)));
}

void Hero::pause()
{
    SpaceShip::pause();
    active = false;   
    engineAccel.x = 0, engineAccel.y = 0;
}

void Hero::resume()
{
    SpaceShip::resume();
    active = true;
}

void Hero::onTimer()
{
    groupCheckTextInfo();

    if((bulletCollisionCheck() != Side::nobody) || enemyCollisionCheck()){
        emit heroKilled();
    }
    else{       
        setPos(x() + calculateMovement('x'), y() + calculateMovement('y'));
    }

}

void Hero::groupCheckTextInfo()
{
    QString output = "";
    output += "Acceleration: " + QString::number(engineAccel.x) + "\n";
    output += "Speed: x: " + QString::number(speed.x) + " y: " + QString::number(speed.y) + "\n";
    setCheckText(output);
}

void Hero::heroKeyPressed(int key)
{
    if(!active) return;

    if (key == Qt::Key_Left && pos().x() > 0){
        engineAccel.x = -movement.engineAccel.x;
    }
    else if (key == Qt::Key_Right && pos().x() + this->boundingRect().width() < scene()->width()){
        engineAccel.x = movement.engineAccel.x;
    }
    else if (key == Qt::Key_Up && pos().y() > 0){
        engineAccel.y = -movement.engineAccel.y;
    }
    else if (key == Qt::Key_Down && pos().y() + this->boundingRect().height() < scene()->height()){
        engineAccel.y = movement.engineAccel.y;
    }
    if (arrowKeys.contains(key)) {
        keyPressed[key] = true;
        keyPressed[oppositeKey[key]] = false;
    }
    else if (key == Qt::Key_Space && shootAvl){
        createBullet(1);
    }

}

void Hero::heroKeyReleased(int key)
{
    if(!active){
        return;
    }

    if(keyPressed[key]){
        if(horizontalMovementKeys.contains(key)){
            engineAccel.x = 0;
        } else{
            engineAccel.y = 0;
        }

        keyPressed[key] = false;
    }
}

bool Hero::enemyCollisionCheck()
{
    auto item = collisionCheck(TypeIndex::enemy);
    if(item != nullptr){
        Enemy* enemy = static_cast<Enemy*>(item);
        enemy->onHeroCollision();
        return true;
    }
    return false;
}

int Hero::calculateMovement(char coord)
{
    int speedC = speed.get(coord);
    double newSpeed = 0;

    int direction;
    if (speedC == 0) direction = 0;
    else direction = abs(speedC) / speedC;


    newSpeed = speedC + engineAccel.get(coord) * (float(period_ms)/1000);

    if((speedC != 0) && (engineAccel.get(coord) == 0)) {
        newSpeed = newSpeed - movement.friction.get(coord) * (float(period_ms)/1000) * direction;
    }

    if(newSpeed != 0){
        direction = abs(newSpeed) / newSpeed;
    }
    else direction = 0;


    if (abs(newSpeed) > movement.maxVelocity.get(coord)){
        newSpeed = movement.maxVelocity.get(coord) * direction;
    }

    float distanceMoved = 0.5 * (newSpeed + speedC) * (float(period_ms)/1000);

    speed.set(coord, newSpeed);

    if(checkScreenBorders(distanceMoved)){
        speed.set(coord, 0);
        return 0;
    }
    return distanceMoved;
}

bool Hero::checkScreenBorders(int distance)
{
    return ((pos().x() + this->width * 0.75 <= 0 && distance < 0) ||
             (pos().x() + this->width * 0.25 >= scene()->width() && distance > 0));
}

