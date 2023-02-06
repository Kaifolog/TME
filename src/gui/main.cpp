#include "mainwindow.h"
#include <iostream>

#include "themes.h"
#include <QApplication>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    a.setStyleSheet(themes::MoonlightTheme);
    w.isDarkMode = true;

    w.show();
    return a.exec();
}
