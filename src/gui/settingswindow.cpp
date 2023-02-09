#include "settingswindow.h"

SettingsWindow::SettingsWindow(QApplication *app, MainWindow *mainwindow, QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    this->app = app;
    this->mainwindow = mainwindow;

    QSettings settings;

    ui->version_label->setText(QString::fromStdString("TME version ") + settings.value("global/version").toString());

    ui->reopen_checkbox->setChecked(settings.value("editor/save_last_path").toBool());
    ui->delete_tmp_checkbox->setChecked(settings.value("editor/clear_tmp_files").toBool());

    ui->max_steps_spinbox->setValue(settings.value("machine/max_steps").toInt());
    if (settings.value("appearance/theme").toString().toStdString() == std::string("moonlight"))
    {
        ui->moonlight_theme_radioButton->setChecked(true);
    }
    if (settings.value("appearance/theme").toString().toStdString() == std::string("blackseasunrise"))
    {
        ui->bss_theme_radioButton->setChecked(true);
    }
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_buttonBox_accepted()
{
    QSettings settings;

    settings.setValue("editor/save_last_path", QString::number(ui->reopen_checkbox->isChecked()));
    settings.setValue("editor/clear_tmp_files", QString::number(ui->delete_tmp_checkbox->isChecked()));
    settings.setValue("machine/max_steps", QString::number(ui->max_steps_spinbox->value()));
}

void SettingsWindow::on_buttonBox_rejected()
{
}

void SettingsWindow::on_reopen_checkbox_clicked()
{
}

void SettingsWindow::on_moonlight_theme_radioButton_clicked()
{
    QSettings settings;
    this->app->setStyleSheet(themes::MoonlightTheme);
    settings.setValue("appearance/theme", "moonlight");

    // an ugly hack that uses mainwindow methods to apply colors to elements that has been changed by debugger
    // highlighting and cant be simply rewritten by setStyleSheet methon
    this->mainwindow->debugPanelHighlightEnabled(false);
    this->mainwindow->breakpointHighlightOFF();
}

void SettingsWindow::on_bss_theme_radioButton_clicked()
{
    QSettings settings;
    this->app->setStyleSheet(themes::BlackSeaSunriseTheme);
    settings.setValue("appearance/theme", "blackseasunrise");

    // an ugly hack that uses mainwindow methods to apply colors to elements that has been changed by debugger
    // highlighting and cant be simply rewritten by setStyleSheet methon
    this->mainwindow->debugPanelHighlightEnabled(false);
    this->mainwindow->breakpointHighlightOFF();
}
