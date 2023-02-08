#include "mainwindow.h"
#include <QApplication>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(&a);
    w.show();
    return a.exec();
}
