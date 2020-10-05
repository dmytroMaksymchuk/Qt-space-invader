#include "enemymanager.h"
#include "enemy.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QDebug>
#include "ScoreBar.h"
#include <iostream>
#include <cstdlib>
#include "Cons.h"

EnemyManager::EnemyManager(QGraphicsScene *scene, ScoreBar *scoreBar)
{
    this->scene = scene;
    this->scoreBar = scoreBar;
    QTimer * timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimer()));

    timer->start(2000);
}

void EnemyManager::onEnemyRemoval(Enemy* enemy)
{
    qDebug() << "Came to enemy manager";
    enemyCount--;
    score++;
    onEnemyCountChange(score);

    upForNextDiff--;
    if(upForNextDiff <= 0){
        difficulty++;
        upForNextDiff = levelDifficultyStep;
        changeDifficulty(difficulty);
    }  

    if (score == totalEnemiesToKill){
        allEnemiesDefeated();
    }
}

void EnemyManager::onTimer()
{
    if(enemyCount < maxEnemyAlive && score <= totalEnemiesToKill-maxEnemyAlive){
        createEnemy();
    }

}

void EnemyManager::createEnemy()
{
    Enemy * enemy = new Enemy(this, ImagePaths::enemyImagePath);
    scene->addItem(enemy);
    enemy->setPos(int(std::rand() % 500 + 100), 0);
    enemyCount++;

}

int EnemyManager::getScore()
{
    return score;
}
