#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "mainwindow.h"
#include "themes.h"
#include "ui_settingswindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDialog>
#include <QSettings>

namespace Ui
{
class SettingsWindow;
}

class MainWindow;

class SettingsWindow : public QDialog
{
    Q_OBJECT

  public:
    explicit SettingsWindow(QApplication *app, MainWindow *mainwindow, QWidget *parent = nullptr);
    ~SettingsWindow();

  private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_reopen_checkbox_clicked();

    void on_moonlight_theme_radioButton_clicked();

    void on_bss_theme_radioButton_clicked();

    void on_hello_kitty_checkBox_clicked();

private:
    Ui::SettingsWindow *ui;
    MainWindow *mainwindow;
    QApplication *app;
};

#endif // SETTINGSWINDOW_H
