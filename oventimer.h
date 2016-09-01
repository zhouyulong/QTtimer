#ifndef OVENTIMER_H
#define OVENTIMER_H

#include<QTimer>
#include<QDateTime>
#include <QMouseEvent>
#include <QtCore/qmath.h>
#include <QPainter>
//#include <QPainEvent>
//#include < QFont >
//#include < QRadiaGradient >
//#include < QConicalGradient >
//#include < QPen >
//#include < QColor >
//#include <QBrush >
//#include < QPainter >
//#include < QPolygon >
//#include < math.h>
#include "widget.h"

class OvenTimer : public QWidget
 {
            Q_OBJECT
    public :
    OvenTimer(QWidget *parent = 0);
    void setDuration( int secs);
    int duration() const;  //error1、此粗报错
    void draw(QPainter *painter);

    signals:
    void timeout();

    protected:
    void paintEvent(QPaintEvent *event);//此处报错 ，error2
    void mousePressEvent(QMouseEvent *event);

    private:
    QDateTime finishTime;
    QTimer *updateTimer;
    QTimer *finishTimer;
};

#endif // OVENTIMER_H
