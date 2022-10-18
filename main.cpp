#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.resize(400, 600);
    w.setWindowTitle(QString::number(2048));
    w.show();
    return a.exec();
}
