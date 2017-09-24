#ifndef SNOWFLAKE_H
#define SNOWFLAKE_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QStyleOption>
#include <QMediaPlayer>


class SnowFlake : public QWidget
{
    Q_OBJECT

public:
    explicit SnowFlake(QWidget *parent = 0);
    SnowFlake(int size, int posX, QWidget *parent);
    ~SnowFlake();

    void setSize(int size);
    int getSize();
    void setPosX(int posX);
    int getPosX();
    void setSpeed(double speed);
    double getSpeed();

    void startAnimation();

    //Necessary for applying style sheets to custom widgets that inherit QWidget
    //(see http://doc.qt.io/qt-4.8/stylesheet-reference.html section "List of Stylable Widgets -> QWidget")
    void paintEvent(QPaintEvent *);

    void enterEvent(QEvent *event);

    static QTimer *timer;
    static void generateSnow(QWidget *parent); //Generate snowflakes inside parent QWidget
    static void generateSnowFlake();
    static void stopSnow();
    static QMediaPlayer *sounds;


private:
    int _size;
    int _posX;
    double _speed;
    QPropertyAnimation *_animation;

public slots:
    void deleteSnowFlake();

};

#endif // SNOWFLAKE_H
