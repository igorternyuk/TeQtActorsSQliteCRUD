#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("fusion");
    QRect desktop = QApplication::desktop()->geometry();
    MainWindow w;
    auto dx = (desktop.width() - w.width()) / 2;
    auto dy = (desktop.height() - w.height()) / 2;
    w.move(dx, dy);;
    w.show();

    return app.exec();
}
