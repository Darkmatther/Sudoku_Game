#include "SnowFlake.h"


QTimer *SnowFlake::timer = 0;

QMediaPlayer *SnowFlake::sounds = 0;


void SnowFlake::generateSnow(QWidget *parent)
{
    if (SnowFlake::timer == 0)
    {
        SnowFlake::timer = new QTimer(parent);
        connect(SnowFlake::timer, &QTimer::timeout, SnowFlake::generateSnowFlake);
    }

    if (SnowFlake::sounds == 0)
    {
        SnowFlake::sounds = new QMediaPlayer;
        SnowFlake::sounds->setVolume(75);
        SnowFlake::sounds->setMedia(QUrl::fromLocalFile("./audio/samples/blop_snowFlake.mp3"));
    }

    if (!SnowFlake::timer->isActive())
    {
        SnowFlake::timer->start(500);
    }
}


void SnowFlake::generateSnowFlake()
{
    QWidget *widget = dynamic_cast<QWidget *>(SnowFlake::timer->parent());
    if (widget != 0)
    {
        SnowFlake *snowFlake = new SnowFlake(widget);

        //qDebug() << "SnowFlake: posX = " << snowFlake->getPosX() << ", size = " << snowFlake->getSize() << ", speed = " << snowFlake->getSpeed();

        snowFlake->lower(); //Lowers the widget at the bottom of the parent widget
        snowFlake->show();
    }
    else
    {
        //qDebug() << "Warning: SnowFlake::timer = 0. Animation cannot be played";
    }
}


void SnowFlake::stopSnow()
{
    if (SnowFlake::timer != 0)
    {
        if (SnowFlake::timer->isActive())
        {
            SnowFlake::timer->stop();
        }

        delete SnowFlake::timer;
        SnowFlake::timer = 0;
    }

    if (SnowFlake::sounds != 0)
    {
        delete SnowFlake::sounds;
        SnowFlake::sounds = 0;
    }
}




SnowFlake::SnowFlake(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);

    //Definition of the initial X position and size of the snowflake with respect to the parent widget
    if (parent != 0)
    {
        this->_size = qrand()%12 + 4;
        this->_posX = qrand()%parent->geometry().width();
        this->_speed = qrand()/(double)RAND_MAX;
    }
    else
    {
        this->_size = 16;
        this->_posX = 0;
        this->_speed = 1.0;
    }

    //Set animation ("falling" vertically until it reaches the bottom of the parent widget)
    this->_animation = new QPropertyAnimation(this, "geometry");
    this->_animation->setDuration( 5000*(1 + this->_speed) );
    this->_animation->setStartValue(QRect(this->_posX, 0, this->_size, this->_size));
    this->_animation->setEndValue(QRect(this->_posX, parent->geometry().height(), this->_size, this->_size));
    this->_animation->setEasingCurve(QEasingCurve::Linear);
    connect(this->_animation, SIGNAL(finished()), this, SLOT(deleteSnowFlake()));
    this->_animation->start(QAbstractAnimation::DeleteWhenStopped);
}


SnowFlake::SnowFlake(int size, int posX, QWidget *parent) : QWidget(parent)
{
    this->_size = size;
    this->_posX = posX;
    this->setGeometry(this->_posX, 24, this->_size, this->_size);
}


SnowFlake::~SnowFlake()
{
    delete this->_animation;
}


void SnowFlake::deleteSnowFlake()
{
    //qDebug() << "DESTROY!!";
    if (this->_animation->state() == QAbstractAnimation::Running)
    {
        this->_animation->stop();
    }
    delete this->_animation;
    this->_animation = 0;
    this->close();
}


void SnowFlake::startAnimation()
{
    this->_animation->start(QAbstractAnimation::DeleteWhenStopped);
}


void SnowFlake::setSize(int size)
{
    this->_size = size;
}


int SnowFlake::getSize()
{
    return this->_size;
}


void SnowFlake::setPosX(int posX)
{
    this->_posX = posX;
}


int SnowFlake::getPosX()
{
    return this->_posX;
}


void SnowFlake::setSpeed(double speed)
{
    this->_speed = speed;
}


double SnowFlake::getSpeed()
{
    return this->_speed;
}


void SnowFlake::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void SnowFlake::enterEvent(QEvent *event)
{
    //qDebug() << "PLOFFF!!!";
    if (SnowFlake::sounds != 0)
    {
        SnowFlake::sounds->play();
    }
    this->deleteSnowFlake();

    QWidget::enterEvent(event);
}

