#include "mainwindow.h"
#include "../emulator/App.hpp"
#include <iostream>

#include <QApplication>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // setting style
    QFile styleSheetFile("./MaterialDark.qss");
    styleSheetFile.open(QFile::ReadOnly);
    if (styleSheetFile.isOpen())
    {
        QString styleSheet = QLatin1String(styleSheetFile.readAll());
        a.setStyleSheet(styleSheet);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
