#include "oventimer.h"

const double DegreesPerMinute = 7.0;
const double DegreesPerSecond = DegreesPerMinute / 60;
const int MaxMinutes = 45;
const int MaxSeconds = MaxMinutes * 60;
const int UpdateInterval = 1;


OvenTimer::OvenTimer( QWidget *parent)
            :QWidget(parent)
{
        finishTime = QDateTime::currentDateTime();//currentDateTime()返回当前的系统时间
        updateTimer = new QTimer(this);
        connect(updateTimer,SIGNAL(timeout()),this,SLOT(update()));


        finishTimer = new QTimer();
        finishTimer -> setSingleShot(true);
        connect(finishTimer, SIGNAL(timeout()),this,SIGNAL(timeout()));
        connect(finishTimer,SIGNAL(timeout()),updateTimer,SLOT(stop()));
        QFont font;
        font.setPointSize(7);
        setFont(font);

}

void OvenTimer::setDuration(int secs){      //setDuration 函数设定了烤箱持续的时间为给定的秒数

    secs = qBound( 0, secs,MaxSeconds);//限制secs的区间，secs只能在0~MaxSecond之间，如果给secs赋值大于Max...则返回为Max...

    finishTime = QDateTime::currentDateTime().addSecs(secs);
    //currentDateTime()返回当前系统时钟。而addSecs(int a) ,
    //则返回的值为此对象（程序）开始执行之后a秒的系统时间即一个QDateTime对象

    if( secs > 0){
        updateTimer -> start(UpdateInterval   );
        finishTimer -> start(secs * 1000);
        }else{

                updateTimer -> stop();
                finishTimer -> stop();

    }
    update();
}

int OvenTimer::duration()const     //返回定时器完成前的剩余秒数。如果定时器未激活则返回0
{
    int secs = QDateTime::currentDateTime().secsTo(finishTime);
    if( secs < 0)
        secs = 0;
    return secs;
}


//当用户点击窗口时，我们使用一个数学公式，使之找到最近的刻度，并且使用这个结果来设置新的持续时间，然后安排一个重绘，
void OvenTimer::mousePressEvent(QMouseEvent *event){
    QPointF point = event->pos() - rect().center();
    double theta = qAtan2(-point.x(),-point.y())* 180.0 /M_PI;
    setDuration(duration() + int(theta / DegreesPerSecond));
    update();

}

void OvenTimer::paintEvent( QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing , true);
    int side = qMin(width() , height());
    painter.setViewport((width() - side ) /2 , (height() - side) /2 , side ,side);
    painter.setWindow( -50, -50 , 100 ,100);
    draw(&painter);

}
void OvenTimer::draw(QPainter *painter)
{
    static const int triangle[3][2] = {
        {-2,-49},{+2 , -49},{0 ,-47}
        };
    QPen  thickPen( palette().foreground(),1.5);
    QPen thinPen(palette().foreground(),0.5);
    QColor niceBlue(150,150,200);

    painter->setPen(thinPen);
    painter->setBrush(palette().foreground());
    painter->drawPolygon(QPolygon(3,&triangle[0][0]));


    //....................绘制外部的圆，用锥形渐变填充，锥形的渐变点位于( 0, 0),角度是-90
QConicalGradient coneGradient (0 , 0 ,-90.0);
coneGradient.setColorAt(0.0,Qt::darkGray);
coneGradient.setColorAt(0.2,niceBlue);
coneGradient.setColorAt(0.5,Qt::white);
coneGradient.setColorAt(1.0,Qt::darkGray);
painter -> setBrush(coneGradient);
painter-> drawEllipse(-46,-46,92,92);



//用辐射渐变填充内部的圆，渐变的中心点和焦点( 0,0),渐变的半径20
QRadialGradient haloGradient ( 0,0,20,0,0);
haloGradient.setColorAt(0.0,Qt::lightGray);
haloGradient.setColorAt(0.8,Qt::darkGray);
haloGradient.setColorAt(0.9,Qt::white);
haloGradient.setColorAt(1.0,Qt::black);


//调用rotate()来绘制旋转绘图器的坐标系统。在旧的坐标系统，
//0-minute 的标记在最上面：现在将这个标记移到对应的这剩余事件的位置，，在
//这个旋转之后我们绘制矩形把手，因为它的方位依赖于旋转的角度
QLinearGradient knobGradient ( -7,-25 , -7 , -25);
knobGradient.setColorAt(0.0 , Qt::black);
knobGradient.setColorAt(0.2, niceBlue);
knobGradient.setColorAt ( 0.3 , Qt::lightGray);
knobGradient.setColorAt ( 0.8 , Qt::white);
knobGradient.setColorAt ( 1.0 , Qt::black );

painter ->rotate ( duration() * DegreesPerSecond );
painter -> setBrush ( knobGradient );
painter -> setPen ( thinPen);
painter -> drawRoundRect ( -7 , -25 , 14 , 50 ,99 ,49);

for ( int i =0 ; i <= MaxMinutes ;i++){
//    painter -> drawText ( -15 ,-41 ,30 , 30 ,
//                          Qt::AlignHCenter | Qt::AlignTop,
//                          QString::number( i ));
    if ( i % 5 ==0){
        painter -> setPen ( thickPen);
        painter -> drawLine ( 0 , -41 ,0 , -44 );
        painter -> drawText ( -15 ,-41 ,30 , 30 ,
                              Qt::AlignHCenter | Qt::AlignTop,
                              QString::number( i ));

    }else {
        painter -> setPen( thinPen );
        painter -> drawLine ( 0 , -42 , 0 , -44 );

    }

    painter -> rotate ( -DegreesPerMinute);
}
}
