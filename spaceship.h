#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <QObject>
#include <QGraphicsRectItem>


class SpaceShip : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit SpaceShip(QObject *parent = nullptr);
    bool removalCheck();
    void createBullet();

public slots:
    virtual void onTimer() = 0;

signals:

protected:
    void setUpDelay(int delay);

    QString ImagePath;
    int width;
    int height;
    int speed;
    int bulletSpeed;
    int shootDelay;
    const int baseShootDelay = 5000;
    bool shootAvl = true;

private:
    QTimer* timerBullet = nullptr;
private slots:
    void shootIsAvl();

};

#endif // SPACESHIP_H
