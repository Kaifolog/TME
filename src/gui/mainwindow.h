#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QCoreApplication>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QShortcut>
#include <QTextStream>
#include <QtGui>

#include "./ui_mainwindow.h"

extern "C"
{
#include <stdio.h>
}

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

extern "C"
{
#include "../external/sqlite3/sqlite3.h"
}
#include "../external/easyloggingpp/easylogging++.h"

#include "../machine/TuringMachine.hpp"
#include "../tools/ProjectName.hpp"
#include "../translator/Translator.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  private:
    bool debugMode = false;
    int highlightedLine = false;
    tools::ProjectName _pname;
    machine::TuringMachine _debugger;

    Ui::MainWindow *ui;

    QShortcut *keyCtrlS;   //_  keybinds objects
    QShortcut *CtrlD;      // |
    QShortcut *keyCtrlN;   // |
    QShortcut *keyCtrlO;   // |
    QShortcut *CtrlShiftX; // |
    QShortcut *CtrlTab;    // |
    QShortcut *CtrlSpace;  //_|

  private slots:
    // keybinds slots
    void slotShortcutCtrlS()
    {
        on_actionSave_triggered();
    };
    void slotShortcutCtrlO()
    {
        on_actionOpen_triggered();
    };
    void slotShortcutCtrlN()
    {
        on_actioNew_triggered();
    };
    void slotShortcutCtrlD();
    void slotShortcutCtrlShiftX()
    {
        on_quickstartbtn_clicked();
    };
    void slotShortcutCtrlTab();
    void slotShortcutCtrlSpace()
    {
        on_debugnextbtn_clicked();
    };

    void on_actionOpen_triggered();  //_ triggers for files actions
    void on_actionSave_triggered();  // |
    void on_actionClose_triggered(); // |
    void on_actioNew_triggered();    //_|

    void on_parsingbtn_clicked();    //_  triggers for buttons
    void on_analysisbtn_clicked();   // |
    void on_emulationbtn_clicked();  // |
    void on_quickstartbtn_clicked(); // |
    void on_debugbtn_clicked();      // |
    void on_debugnextbtn_clicked();  // |
    void on_skipButton_clicked();    //_|

    void on_mainTextField_textChanged();           //_  triggers for textfields
    void on_mainTextField_cursorPositionChanged(); // |
    void on_datacheckBox_clicked();                //_|

    void on_action_triggered();   //_  trigger for a title bar
    void on_action_2_triggered(); // |
    void on_action_3_triggered(); // |
    void on_action_5_triggered(); // |
    void on_settings_triggered(); // |
    void on_about_triggered();    //_|

    void breakpointHighlightON();          //_  utility functions
    void breakpointHighlightOFF();         // |
    void AllButtonsSetEnabled(bool);       // |
    void currentLineHighlight(int line);   // |
    void openInEditor();                   // |
    void showStartMessage();               // |
    void debugPanelHighlightEnabled(bool); // |
    void clearLogFile();                   //_|

    void closeEvent(QCloseEvent *bar);

    void NormalMiddleware(); //_  middleware functions
    void DebugMiddleware();  //_|

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow()
    {
        delete ui;
    };
    void writeSettings();
    void readSettings();
    bool isDarkMode = 0;
};

#define NORMALMIDDLEWARE NormalMiddleware();
#define DEBUGMIDDLEWARE DebugMiddleware();

#endif // MAINWINDOW_H
