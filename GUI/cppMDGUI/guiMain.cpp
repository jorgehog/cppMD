#include "mainwindow.h"
#include <QApplication>

#include "viz/qtplatform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Platform* platform = new QtPlatform(argc, argv, &w);

    return platform->exec() || a.exec();
}
