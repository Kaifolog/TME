#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QShortcut>
#include <QMessageBox>
#include <QtGui>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int argc;
    char **argv;
    bool isDarkMode = 0;

private slots:
    void slotShortcutCtrlS();
    void slotShortcutCtrlShiftX();
    void slotShortcutCtrlO();
    void slotShortcutCtrlD();
    void slotShortcutCtrlN();
    void slotShortcutCtrlTab();
    void slotShortcutCtrlSpace();

    void on_parsingbtn_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionClose_triggered();

    void on_actioNew_triggered();

    void on_mainTextField_textChanged();

    void on_analisysbtn_clicked();

    void on_emulationbtn_clicked();

    void on_quickstartbtn_clicked();

    void on_debugbtn_clicked();

    void on_debugnextbtn_clicked();

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_5_triggered();

    void on_action_4_triggered();

    void on_mainTextField_cursorPositionChanged();

    void breakpointHighlightON();

    void breakpointHighlightOFF();

    //    void on_autoDebuggerPushButton_clicked();

    void on_skipButton_clicked();

    void currentLineHighlight(int line);

    void on_datacheckBox_clicked();

private:
    QString fileName;
    bool debug = 0;
    int highlighedLine = 0;

    Ui::MainWindow *ui;

    QShortcut *keyCtrlS;   // объект сочетания клавиш Ctrl + D
    QShortcut *CtrlD;      // объект сочетания клавиш Ctrl + D
    QShortcut *keyCtrlN;   // объект сочетания клавиш Ctrl + D
    QShortcut *keyCtrlO;   // объект сочетания клавиш Ctrl + D
    QShortcut *CtrlShiftX; // объект сочетания клавиш Ctrl + D
    QShortcut *CtrlTab;    // объект сочетания клавиш Ctrl + D
    QShortcut *CtrlSpace;  // объект сочетания клавиш Ctrl + D
};

#endif // MAINWINDOW_H
