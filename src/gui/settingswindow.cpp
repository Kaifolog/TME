#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    QSettings settings;

    ui->version_label->setText(QString::fromStdString("TME version ") + settings.value("global/version").toString());

    ui->reopen_checkbox->setChecked(settings.value("editor/save_last_path").toBool());
    ui->delete_tmp_checkbox->setChecked(settings.value("editor/clear_tmp_files").toBool());

    ui->max_steps_spinbox->setValue(settings.value("machine/max_steps").toInt());
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
