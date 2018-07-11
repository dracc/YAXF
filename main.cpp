#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("XQEMU team");
    QCoreApplication::setOrganizationDomain("xqemu.com");
    QCoreApplication::setApplicationName("YAXF");
    MainWindow w;
    w.show();

    return a.exec();
}
