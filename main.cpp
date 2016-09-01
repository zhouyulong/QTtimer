#include "widget.h"
#include <QApplication>
#include "oventimer.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // Widget w;
    //w.show();
    OvenTimer zhou;
    zhou.show();
    return a.exec();
}
