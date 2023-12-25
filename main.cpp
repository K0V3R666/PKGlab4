#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QLinearGradient gradient(0, 0, 0, w.height());
    gradient.setColorAt(0, QColor("#FFC0CB"));
    gradient.setColorAt(1, QColor("#FF69B4"));


    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(gradient));
    w.setPalette(palette);

    w.show();
    return a.exec();
}
