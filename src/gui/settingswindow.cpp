#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    QSettings settings;

    ui->version_label->setText(QString::fromStdString("version ") + settings.value("global/version").toString());

    ui->reopen_checkbox->setChecked(settings.value("editor/save_last_path").toBool());
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_buttonBox_accepted()
{
    QSettings settings;

    settings.setValue("editor/save_last_path", QString::number(ui->reopen_checkbox->isChecked()));
}

void SettingsWindow::on_buttonBox_rejected()
{
}

void SettingsWindow::on_reopen_checkbox_clicked()
{
}
