#include "mainwindow.h"
#include <iostream>

#include <QApplication>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // sets style
    QFile styleSheetFile("./MaterialDark.qss");
    styleSheetFile.open(QFile::ReadOnly);
    if (styleSheetFile.isOpen())
    {
        QString styleSheet = QLatin1String(styleSheetFile.readAll());
        a.setStyleSheet(styleSheet);
        w.isDarkMode = 1;
    }

    w.show();
    return a.exec();
}
