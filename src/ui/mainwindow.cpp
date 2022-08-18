#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMenuBar>
#include <QTextStream>
#include <QFile>
#include <fstream>
#include "../emulator/App.hpp"
#include "../emulator/vendor/sqlite3/sqlite3.h"
#include <QMessageBox>
#include <iostream>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    keyCtrlS = new QShortcut(this);         // Инициализируем объект
    keyCtrlS->setKey(Qt::CTRL + Qt::Key_S); // Устанавливаем сочетание клавиш
    // подключаем обработчик нажатия клавиши
    connect(keyCtrlS, SIGNAL(activated()), this, SLOT(slotShortcutCtrlS()));

    keyCtrlO = new QShortcut(this);         // Инициализируем объект
    keyCtrlO->setKey(Qt::CTRL + Qt::Key_O); // Устанавливаем сочетание клавиш
    // подключаем обработчик нажатия клавиши
    connect(keyCtrlO, SIGNAL(activated()), this, SLOT(slotShortcutCtrlO()));

    keyCtrlN = new QShortcut(this);         // Инициализируем объект
    keyCtrlN->setKey(Qt::CTRL + Qt::Key_N); // Устанавливаем сочетание клавиш
    // подключаем обработчик нажатия клавиши
    connect(keyCtrlN, SIGNAL(activated()), this, SLOT(slotShortcutCtrlN()));

    CtrlShiftX = new QShortcut(this);                     // Инициализируем объект
    CtrlShiftX->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_X); // Устанавливаем сочетание клавиш
    // подключаем обработчик нажатия клавиши
    connect(CtrlShiftX, SIGNAL(activated()), this, SLOT(slotShortcutCtrlShiftX()));

    CtrlTab = new QShortcut(this);           // Инициализируем объект
    CtrlTab->setKey(Qt::CTRL + Qt::Key_Tab); // Устанавливаем сочетание клавиш
    // подключаем обработчик нажатия клавиши
    connect(CtrlTab, SIGNAL(activated()), this, SLOT(slotShortcutCtrlTab()));

    CtrlSpace = new QShortcut(this); // Инициализируем объект
    CtrlSpace->setKey(Qt::Key_F5);   // CTRL + Qt::Key_Space); // Устанавливаем сочетание клавиш
    // подключаем обработчик нажатия клавиши
    connect(CtrlSpace, SIGNAL(activated()), this, SLOT(slotShortcutCtrlSpace()));

    CtrlD = new QShortcut(this);         // Инициализируем объект
    CtrlD->setKey(Qt::CTRL + Qt::Key_D); // Устанавливаем сочетание клавиш
    // подключаем обработчик нажатия клавиши
    connect(CtrlD, SIGNAL(activated()), this, SLOT(slotShortcutCtrlD()));
}

void MainWindow::slotShortcutCtrlD()
{
    if (ui->rawnumberlabel->text().length())
    {
        int rawNumber = ui->rawnumberlabel->text().toInt();

        if (ui->datacheckBox->isChecked())
        {
            rawNumber -= 2;
        }

        QString str = ui->mainTextField->toPlainText();
        QStringList strList = str.split('\n'); // разбиваем строку из текстедита на отдельные строки
        str = strList.at(rawNumber - 1);       // берем вторую строку

        strList = str.split(";#d");

        QTextCursor text_cursor = QTextCursor(ui->mainTextField->document());

        for (int i = 0; i < rawNumber - 1; i++)
            text_cursor.movePosition(QTextCursor::Down);

        text_cursor.movePosition(QTextCursor::EndOfLine);

        if (strList.size() == 1)
        {
            text_cursor.insertText("\t;#d");
        }
        else
        {
            for (int i = 0; i < strList[(strList.size() - 1)].size() + 4; i++)
            {
                text_cursor.movePosition(QTextCursor::Left);
                text_cursor.deleteChar();
            }
        }
    }
}

void MainWindow::slotShortcutCtrlSpace()
{
    on_debugnextbtn_clicked();
}

void MainWindow::slotShortcutCtrlTab()
{
    if (ui->rawnumberlabel->text().length())
    {
        int rawNumber = ui->rawnumberlabel->text().toInt();

        if (ui->datacheckBox->isChecked())
        {
            rawNumber -= 2;
        }

        QString str = ui->mainTextField->toPlainText();
        QStringList strList = str.split('\n'); // разбиваем строку из текстедита на отдельные строки
        str = strList.at(rawNumber - 1);       // берем вторую строку

        strList = str.split(',');
        QStringList strList1 = str.split("->");

        QTextCursor text_cursor = QTextCursor(ui->mainTextField->document());

        for (int i = 0; i < rawNumber - 1; i++)
            text_cursor.movePosition(QTextCursor::Down);

        text_cursor.movePosition(QTextCursor::EndOfLine);

        if (strList.length() == 1)
            text_cursor.insertText(",");
        if (strList.length() == 2 && strList1.length() == 1)
            text_cursor.insertText("->");
        if (strList.length() == 2 && strList1.length() == 2)
            text_cursor.insertText(",");
        if (strList.length() == 3 && strList1.length() == 2)
            text_cursor.insertText(",");
        if (strList.length() == 4 && strList1.length() == 2)
            text_cursor.insertText("\n");
    }
}

void MainWindow::slotShortcutCtrlS()
{
    on_actionSave_triggered();
}

void MainWindow::slotShortcutCtrlO()
{
    on_actionOpen_triggered();
}
void MainWindow::slotShortcutCtrlN()
{
    on_actioNew_triggered();
}
void MainWindow::slotShortcutCtrlShiftX()
{
    on_quickstartbtn_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/****************************/

void MainWindow::on_datacheckBox_clicked()
{
    if (ui->datacheckBox->isChecked())
    {
        QString text = ui->mainTextField->toPlainText();
        if (text.left(text.indexOf(QChar('\n'))).remove(QRegularExpression("[\\s]+")).toStdString() == "section.data") //если первая строка .data и нажат чекбокс
        {
            QStringList list = text.split('\n');
            QString inputData = list[1];
            list.removeFirst();
            list.removeFirst();
            text = list.join('\n');
            ui->inputlineEdit->setText(inputData);
            ui->mainTextField->clear();
            ui->mainTextField->setPlainText(text);
        }
        else
            ui->inputlineEdit->clear();
    }
    else
    {
        QString text = ui->mainTextField->toPlainText();
        ui->mainTextField->clear();
        ui->mainTextField->appendPlainText("section .data\n" + ui->inputlineEdit->text());
        ui->inputlineEdit->clear();
        ui->mainTextField->appendPlainText(text);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString newFile;
    newFile = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("Documents (*.tme *.txt)"));
    // if (!fileName.length())
    //     newFile = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("Documents (*.tme *.txt)"));           // ??))
    // else
    // {
    //     newFile = fileName;
    //     QString file__ = newFile.left(newFile.lastIndexOf(QChar('/')));
    //     newFile = QFileDialog::getOpenFileName(this, tr("Open File"), file__, tr("Documents (*.tme *.txt)"));
    // }
    if (newFile.length())
        fileName = newFile;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString text;
        text = file.readAll();

        if (text.left(text.indexOf(QChar('\n'))).remove(QRegularExpression("[\\s]+")).toStdString() == "section.data" && ui->datacheckBox->isChecked()) //если первая строка .data и нажат чекбокс
        {
            QStringList list = text.split('\n');
            QString inputData = list[1];
            list.removeFirst();
            list.removeFirst();
            text = list.join('\n');
            ui->inputlineEdit->setText(inputData);
        }
        else
            ui->inputlineEdit->clear();

        ui->mainTextField->clear();
        ui->mainTextField->setPlainText(text);
        file.close();
        ui->filenamelbl->setText(fileName);
        ui->filestatuslbl->setText("Opened");

        char *argv1[2];
        argv1[1] = new char[fileName.length()];
        string stdfileName = fileName.toStdString();
        for (int i = 0; i < fileName.length(); i++)
        {
            argv1[1][i] = stdfileName.c_str()[i];
        }
        argv = argv1;
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (fileName.size())
    {
        std::ofstream out;
        out.open(fileName.toUtf8());
        if (ui->inputlineEdit->text().size() && ui->datacheckBox->isChecked())
        {
            out << "section .data" << std::endl;
            out << ui->inputlineEdit->text().toStdString() << std::endl;
        }
        out << ui->mainTextField->toPlainText().toStdString();
        ui->filenamelbl->setText(fileName);
        ui->filestatuslbl->setText("Saved");

        out.close();
    }
}

void MainWindow::on_actionClose_triggered()
{

    fileName.clear();
    ui->debuglineEdit->clear();
    ui->mainTextField->clear();
    ui->inputlineEdit->clear();
    ui->debugstatelbl->clear();
    ui->debugwordlbl->clear();
    ui->logwindow->clear();
    ui->filenamelbl->clear();
    ui->filestatuslbl->clear();
    ui->mainTextField->document()->clearUndoRedoStacks();
}

void MainWindow::on_actioNew_triggered()
{
    ui->mainTextField->clear();

    if (!fileName.length())
        fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/", tr("Documents (*.tme *.txt)"));
    else
    {
        QString file__ = fileName.left(fileName.lastIndexOf(QChar('/')));
        fileName = QFileDialog::getSaveFileName(this, tr("Save File"), file__, tr("Documents (*.tme *.txt)"));
    }
    QFile mFile(fileName);
    mFile.open(QIODevice::WriteOnly);
    mFile.close();
    ui->filenamelbl->setText(fileName);
    ui->filestatuslbl->setText("Opened");
}
/********************************/

void MainWindow::on_mainTextField_textChanged()
{
    ui->filestatuslbl->setText("Changed");
    if (!fileName.size())
    {
        ui->logwindow->appendPlainText("Please, open the file.");
    }
}
/****************************/

void MainWindow::on_parsingbtn_clicked()
{

    breakpointHighlightOFF();
    ui->parsingbtn->setEnabled(false);
    ui->analisysbtn->setEnabled(false);
    ui->emulationbtn->setEnabled(false);
    ui->quickstartbtn->setEnabled(false);
    ui->debugbtn->setEnabled(false);

    if (fileName.size())
    {

        debug = 0; //выводим из режима дебаг принудительно
        ui->debuglineEdit->clear();
        ui->debugstatelbl->clear();
        ui->debugwordlbl->clear();

        on_actionSave_triggered();

        string logs_path = fileName.toStdString();

        if (logs_path.substr(logs_path.find_last_of(".") + 1) == "tme" || logs_path.substr(logs_path.find_last_of(".") + 1) == "txt")
        {
            logs_path.pop_back();
            logs_path.pop_back();
            logs_path.pop_back();
            logs_path.pop_back();
        }

        logs_path.append("_log.txt");

        // logger configuring
        el::Configurations defaultConf;
        defaultConf.setToDefault();
        defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime :: %level %msg");
        defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
        defaultConf.setGlobally(el::ConfigurationType::Filename, logs_path);

        // logs file clearing
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "1");
        el::Loggers::reconfigureLogger("default", defaultConf);
        LOG(INFO) << "Log file cleared";
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "104857600");
        el::Loggers::reconfigureLogger("default", defaultConf);

        App app(fileName.toStdString());
        app.setLambda((bool)ui->lambdacheckBox->isChecked());
        try
        {
            app.context_free_analysis_and_parsing();
        }
        catch (...)
        {
        }

        QFile file(QString::fromUtf8(logs_path.c_str()));
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            QString text;
            text = file.readAll();
            ui->debuglineEdit->clear();
            ui->logwindow->clear();
            ui->logwindow->appendPlainText(text);
        }
        file.close();
    }
    ui->parsingbtn->setEnabled(true);
    ui->analisysbtn->setEnabled(true);
    ui->emulationbtn->setEnabled(true);
    ui->quickstartbtn->setEnabled(true);
    ui->debugbtn->setEnabled(true);
}

void MainWindow::on_analisysbtn_clicked()
{

    breakpointHighlightOFF();
    ui->parsingbtn->setEnabled(false);
    ui->analisysbtn->setEnabled(false);
    ui->emulationbtn->setEnabled(false);
    ui->quickstartbtn->setEnabled(false);
    ui->debugbtn->setEnabled(false);

    if (fileName.size())
    {

        debug = 0; //выводим из режима дебаг принудительно
        ui->debuglineEdit->clear();
        ui->debugstatelbl->clear();
        ui->debugwordlbl->clear();

        string logs_path = fileName.toStdString();

        if (logs_path.substr(logs_path.find_last_of(".") + 1) == "tme" || logs_path.substr(logs_path.find_last_of(".") + 1) == "txt")
        {
            logs_path.pop_back();
            logs_path.pop_back();
            logs_path.pop_back();
            logs_path.pop_back();
        }

        logs_path.append("_log.txt");

        // logger configuring
        el::Configurations defaultConf;
        defaultConf.setToDefault();
        defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime :: %level %msg");
        defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
        defaultConf.setGlobally(el::ConfigurationType::Filename, logs_path);

        // logs file clearing
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "1");
        el::Loggers::reconfigureLogger("default", defaultConf);
        LOG(INFO) << "Log file cleared";
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "104857600");
        el::Loggers::reconfigureLogger("default", defaultConf);

        App app(fileName.toStdString());
        app.setLambda((bool)ui->lambdacheckBox->isChecked());
        try
        {
            app.semantic_analysis();
        }
        catch (...)
        {
        }

        QFile file(QString::fromUtf8(logs_path.c_str()));
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            QString text = "ddfdf"; // ?)
            text = file.readAll();
            ui->debuglineEdit->clear();
            ui->logwindow->clear();
            ui->logwindow->appendPlainText(text);
        }
        file.close();
    }
    ui->parsingbtn->setEnabled(true);
    ui->analisysbtn->setEnabled(true);
    ui->emulationbtn->setEnabled(true);
    ui->quickstartbtn->setEnabled(true);
    ui->debugbtn->setEnabled(true);
}

void MainWindow::on_emulationbtn_clicked()
{
    breakpointHighlightOFF();
    ui->parsingbtn->setEnabled(false);
    ui->analisysbtn->setEnabled(false);
    ui->emulationbtn->setEnabled(false);
    ui->quickstartbtn->setEnabled(false);
    ui->debugbtn->setEnabled(false);

    if (fileName.size())
    {

        debug = 0; //выводим из режима дебаг принудительно
        ui->debuglineEdit->clear();
        ui->debugstatelbl->clear();
        ui->debugwordlbl->clear();

        string logs_path = fileName.toStdString();

        if (logs_path.substr(logs_path.find_last_of(".") + 1) == "tme" || logs_path.substr(logs_path.find_last_of(".") + 1) == "txt")
        {
            logs_path.pop_back();
            logs_path.pop_back();
            logs_path.pop_back();
            logs_path.pop_back();
        }

        string out_path = logs_path;

        logs_path.append("_log.txt");

        // logger configuring
        el::Configurations defaultConf;
        defaultConf.setToDefault();
        defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime :: %level %msg");
        defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
        defaultConf.setGlobally(el::ConfigurationType::Filename, logs_path);

        // logs file clearing
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "1");
        el::Loggers::reconfigureLogger("default", defaultConf);
        LOG(INFO) << "Log file cleared";
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "104857600");
        el::Loggers::reconfigureLogger("default", defaultConf);

        App app(fileName.toStdString());
        app.setLambda((bool)ui->lambdacheckBox->isChecked());
        try
        {
            app.emulator_executing_procedure();
        }
        catch (...)
        {
        }

        QFile file(QString::fromUtf8(logs_path.c_str()));
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            QString text = "ddfdf"; // ?)
            text = file.readAll();
            ui->logwindow->clear();
            ui->logwindow->appendPlainText(text);
            file.close();
        }

        QFile file1(QString::fromUtf8((out_path + "_out.txt").c_str()));
        if (file1.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file1);
            QString text = file1.readAll();

            ui->debuglineEdit->clear();
            ui->debuglineEdit->setText(text);
            file.close();
        }
    }
    ui->parsingbtn->setEnabled(true);
    ui->analisysbtn->setEnabled(true);
    ui->emulationbtn->setEnabled(true);
    ui->quickstartbtn->setEnabled(true);
    ui->debugbtn->setEnabled(true);
}

void MainWindow::on_quickstartbtn_clicked()
{
    breakpointHighlightOFF();
    ui->parsingbtn->setEnabled(false);
    ui->analisysbtn->setEnabled(false);
    ui->emulationbtn->setEnabled(false);
    ui->quickstartbtn->setEnabled(false);
    ui->debugbtn->setEnabled(false);

    if (fileName.size())
    {
        debug = 0; //выводим из режима дебаг принудительно
        ui->debuglineEdit->clear();
        ui->debugstatelbl->clear();
        ui->debugwordlbl->clear();

        on_actionSave_triggered();

        string logs_path = fileName.toStdString();

        if (logs_path.substr(logs_path.find_last_of(".") + 1) == "tme" || logs_path.substr(logs_path.find_last_of(".") + 1) == "txt")
        {
            logs_path.pop_back();
            logs_path.pop_back();
            logs_path.pop_back();
            logs_path.pop_back();
        }

        string out_path = logs_path;

        logs_path.append("_log.txt");

        // logger configuring
        el::Configurations defaultConf;
        defaultConf.setToDefault();
        defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime :: %level %msg");
        defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
        defaultConf.setGlobally(el::ConfigurationType::Filename, logs_path);

        // logs file clearing
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "1");
        el::Loggers::reconfigureLogger("default", defaultConf);
        LOG(INFO) << "Log file cleared";
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "104857600");
        el::Loggers::reconfigureLogger("default", defaultConf);

        App app(fileName.toStdString());
        app.setLambda((bool)ui->lambdacheckBox->isChecked());
        try
        {
            app.context_free_analysis_and_parsing();
            app.emulator_executing_procedure();
        }
        catch (...)
        {
        }

        QFile file(QString::fromUtf8(logs_path.c_str()));
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            QString text = "ddfdf";
            text = file.readAll();
            ui->logwindow->clear();
            ui->logwindow->appendPlainText(text);
            file.close();
        }

        QFile file1(QString::fromUtf8((out_path + "_out.txt").c_str()));
        if (file1.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file1);
            QString text = file1.readAll();

            ui->debuglineEdit->clear();
            ui->debuglineEdit->setText(text);
            file.close();
        }
        file.close();
    }
    ui->parsingbtn->setEnabled(true);
    ui->analisysbtn->setEnabled(true);
    ui->emulationbtn->setEnabled(true);
    ui->quickstartbtn->setEnabled(true);
    ui->debugbtn->setEnabled(true);
}

void MainWindow::breakpointHighlightON()
{

    if (fileName.size())
    {

        string dir = fileName.toStdString();

        if (dir.substr(dir.find_last_of(".") + 1) == "tme" || dir.substr(dir.find_last_of(".") + 1) == "txt")
        {
            dir.pop_back();
            dir.pop_back();
            dir.pop_back();
            dir.pop_back();
        }

        sqlite3 *db = 0;
        char *err = 0;
        sqlite3_stmt *ppStmt;
        string a = dir;
        a.append(".db");

        sqlite3_open(a.c_str(), &db);
        sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err);

        string select_command;
        select_command = "SELECT *FROM commands WHERE debug=\"1\"";
        sqlite3_prepare_v2(db, select_command.c_str(), 256, &ppStmt, NULL);
        int resultSqlite3 = sqlite3_step(ppStmt);

        while (resultSqlite3 == SQLITE_ROW)
        {
            QTextCursor cur = ui->mainTextField->textCursor();
            int linenumber = atoi(string((char *)sqlite3_column_text(ppStmt, 6)).c_str());
            cur.movePosition(QTextCursor::Start);
            if (ui->inputlineEdit->text().length())
            {
                linenumber -= 2; //коректировка номера строки, мы ж секцию сверху дописываем
            }
            for (int i = 0; i < linenumber - 1; i++)
                cur.movePosition(QTextCursor::Down);
            QTextBlockFormat f;
            f.setBackground(QBrush("#550e12"));
            cur.select(QTextCursor::LineUnderCursor);
            cur.setBlockFormat(f);

            resultSqlite3 = sqlite3_step(ppStmt);
        }

        sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
        sqlite3_free(err);
        sqlite3_finalize(ppStmt);
        sqlite3_close(db);
    }
    else
    {
        return;
    }
}

void MainWindow::currentLineHighlight(int line)
{

    if (ui->inputlineEdit->text().length())
    {
        line -= 2; //коректировка номера строки, мы ж секцию сверху дописываем
    }

    QTextCursor cur = ui->mainTextField->textCursor();
    cur.movePosition(QTextCursor::Start);
    for (int i = 0; i < highlighedLine - 1; i++)
        cur.movePosition(QTextCursor::Down);
    QTextBlockFormat f;
    if (isDarkMode)
    {
        f.setBackground(QBrush("#2a2931"));
    }
    else
    {
        f.setBackground(QBrush("#ffffff"));
    }
    cur.select(QTextCursor::LineUnderCursor);
    cur.setBlockFormat(f);

    highlighedLine = line;

    breakpointHighlightON();

    QTextCursor cur1 = ui->mainTextField->textCursor();
    cur1.movePosition(QTextCursor::Start);
    for (int i = 0; i < line - 1; i++)
        cur1.movePosition(QTextCursor::Down);
    QTextBlockFormat f1;
    f1.setBackground(QBrush("#4b4b18"));
    cur1.select(QTextCursor::LineUnderCursor);
    cur1.setBlockFormat(f1);

    if (line > 24)
    {
        QTextCursor cursor(ui->mainTextField->document()->findBlockByLineNumber(line - 1)); // ln-1 because line number starts from 0
        ui->mainTextField->setTextCursor(cursor);
    }
}

void MainWindow::breakpointHighlightOFF()
{

    QTextCursor cursor = ui->mainTextField->textCursor();
    // cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cursor.select(QTextCursor::Document);
    QTextBlockFormat f1;
    // f.setBackground(Qt::red);
    if (isDarkMode)
    {
        f1.setBackground(QBrush("#2a2931"));
    }
    else
    {
        f1.setBackground(QBrush("#ffffff"));
    }
    // f1.setBackground(QBrush("#2a2931")); // 550e12 //4b4b18
    // cur1.select(QTextCursor::LineUnderCursor);
    cursor.setBlockFormat(f1);
}

void MainWindow::on_debugbtn_clicked()
{
    if (fileName.size())
    {
        ui->debugbtn->setEnabled(false);

        on_actionSave_triggered();
        breakpointHighlightON();
        on_parsingbtn_clicked();

        debug = 1;

        string dir = fileName.toStdString();

        if (dir.substr(dir.find_last_of(".") + 1) == "tme" || dir.substr(dir.find_last_of(".") + 1) == "txt")
        {
            dir.pop_back();
            dir.pop_back();
            dir.pop_back();
            dir.pop_back();
        }

        sqlite3 *db = 0;
        char *err = 0;
        sqlite3_stmt *ppStmt;

        ui->logwindow->appendPlainText("Starting debugger...\n");
        TuringMachine tm;
        ui->debuglineEdit->setReadOnly(1);

        if (tm.load_strip("datasection.tmp"))
        {
            string a = dir;

            /**************/
            a.append(".db");

            sqlite3_open(a.c_str(), &db);
            // sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &err); //SO FUCKING RISKY
            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err);
            string select_command;
            int currentline = 0;
            if (!tm.is_end(dir, (int)ui->lambdacheckBox->isChecked()))
            {

                select_command = "SELECT *FROM commands WHERE initial_state=\"" + tm.get_current_state() + "\" AND initial_word=\"" + tm.get_current_word() + "\"";
                sqlite3_prepare_v2(db, select_command.c_str(), 256, &ppStmt, NULL);

                if (sqlite3_step(ppStmt) != SQLITE_DONE)
                {
                    tm.set_current_state(string((char *)sqlite3_column_text(ppStmt, 2)));
                    tm.set_current_word(string((char *)sqlite3_column_text(ppStmt, 3)));
                    tm.get_step(((char *)sqlite3_column_text(ppStmt, 4))[0]);
                    currentline = atoi(string((char *)sqlite3_column_text(ppStmt, 6)).c_str());
                }
                else
                {
                    breakpointHighlightOFF();
                    ui->logwindow->appendPlainText("EMULATING ERROR : CANT FIND NEXT COMMAND");
                    LOG(ERROR) << "EMULATING ERROR : CANT FIND NEXT COMMAND";
                    sqlite3_finalize(ppStmt);
                    sqlite3_close(db);
                    ui->logwindow->appendPlainText("Database closed");
                    LOG(INFO) << "Database closed";

                    ui->debuglineEdit->setText(tm.get_strip((int)ui->lambdacheckBox->isChecked()).c_str());
                    ui->debugstatelbl->setText(tm.get_current_state().c_str());
                    ui->debugwordlbl->setText(tm.get_current_word().c_str());

                    debug = 0;
                    ui->debugbtn->setEnabled(true);
                    return; //сюда ебашить подсветку
                }

                currentLineHighlight(currentline);

                ui->debuglineEdit->setText(tm.get_strip((int)ui->lambdacheckBox->isChecked()).c_str());
                ui->debugstatelbl->setText(tm.get_current_state().c_str());
                ui->debugwordlbl->setText(tm.get_current_word().c_str());

                ofstream fout;
                fout.open("datasection.tmp");

                for (int i = 0; i < tm.get_strip(0).size(); i++)
                {
                    fout << tm.get_strip(0)[i];
                }
                fout.close();

                sqlite3_finalize(ppStmt);
                sqlite3_close(db);
                ui->logwindow->appendPlainText("Database closed");

                ui->debugbtn->setEnabled(true);
                return; //сюда ебашить подсветку
            }
            else
            {
                debug = 0;
                ui->mainTextField->document()->clearUndoRedoStacks();

                ui->debugbtn->setEnabled(true);
                breakpointHighlightOFF();
                ui->debuglineEdit->setText(tm.get_strip((int)ui->lambdacheckBox->isChecked()).c_str());
                ui->logwindow->appendPlainText("Debugging ended");

                sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
                sqlite3_finalize(ppStmt);
                sqlite3_close(db);
            }
        }
        else
        {
            ui->debugbtn->setEnabled(true);
            breakpointHighlightOFF();
            ui->logwindow->appendPlainText("there are some troubles with .data file");
            debug = 0;
            return;
        }
        breakpointHighlightOFF();
        sqlite3_finalize(ppStmt);
        sqlite3_close(db);
    }
}

void MainWindow::on_debugnextbtn_clicked()
{
    if (ui->debugstatelbl->text() == "end")
    {
        debug = 0;
        breakpointHighlightOFF();
        //        ui->debuglineEdit->clear();
        //        ui->debugstatelbl->clear();
        //        ui->debugwordlbl->clear();
        ui->mainTextField->document()->clearUndoRedoStacks();
        ui->logwindow->appendPlainText("\nDebugging ended...\n");
        ui->mainTextField->setFocus(); //
        return;
    }
    if (debug == 1)
    {
        ui->debugnextbtn->setEnabled(false);

        if (fileName.size())
        {
            string dir = fileName.toStdString();

            if (dir.substr(dir.find_last_of(".") + 1) == "tme" || dir.substr(dir.find_last_of(".") + 1) == "txt")
            {
                dir.pop_back();
                dir.pop_back();
                dir.pop_back();
                dir.pop_back();
            }

            sqlite3 *db = 0;
            char *err = 0;
            sqlite3_stmt *ppStmt;

            TuringMachine tm;
            tm.set_current_state(ui->debugstatelbl->text().toStdString());

            if (tm.load_strip("datasection.tmp"))
            {

                string a = dir;

                /**************/
                a.append(".db");

                sqlite3_open(a.c_str(), &db);
                string select_command;
                int currentline = 0;
                if (!tm.is_end(dir, (int)ui->lambdacheckBox->isChecked()))
                {
                    select_command = "SELECT *FROM commands WHERE initial_state=\"" + tm.get_current_state() + "\" AND initial_word=\"" + tm.get_current_word() + "\"";

                    sqlite3_prepare_v2(db, select_command.c_str(), 256, &ppStmt, NULL);

                    if (sqlite3_step(ppStmt) != SQLITE_DONE)
                    {
                        ui->logwindow->appendPlainText("\nNext step...");
                        tm.set_current_state(string((char *)sqlite3_column_text(ppStmt, 2)));
                        tm.set_current_word(string((char *)sqlite3_column_text(ppStmt, 3)));
                        tm.get_step(((char *)sqlite3_column_text(ppStmt, 4))[0]);
                        currentline = atoi(string((char *)sqlite3_column_text(ppStmt, 6)).c_str());
                        ui->debugstatelbl->setText(QString::fromStdString(tm.get_current_state()));
                        ui->debugwordlbl->setText(QString::fromStdString(tm.get_current_word()));
                        ui->debuglineEdit->setText(QString::fromStdString(tm.get_strip((int)ui->lambdacheckBox->isChecked())));
                        currentLineHighlight(currentline);

                        ui->logwindow->appendPlainText("OK...");
                        /***/
                        ofstream fout;
                        fout.open("datasection.tmp");

                        for (int i = 0; i < tm.get_strip(0).size(); i++)
                        {
                            fout << tm.get_strip(0)[i];
                        }
                        fout.close();
                        ui->debugnextbtn->setEnabled(true);
                        /***/
                    }
                    else
                    {
                        sqlite3_finalize(ppStmt);
                        sqlite3_close(db);
                        ui->logwindow->appendPlainText("emulation error : cant find next command\n");
                        // breakpointHighlightOFF();
                        debug = 0;
                        ui->debuglineEdit->setText(QString::fromStdString(tm.get_strip((int)ui->lambdacheckBox->isChecked())));
                        ui->debuglineEdit->setReadOnly(0);
                        ui->debugnextbtn->setEnabled(true);
                        // currentLineHighlight(currentline);
                        return;
                    }
                    ui->debugnextbtn->setEnabled(true);

                    sqlite3_finalize(ppStmt);

                    ui->debuglineEdit->setReadOnly(0);
                }
                sqlite3_close(db);
            }
            else
            {
                ui->logwindow->appendPlainText("there are some troubles with .data file\n");
                breakpointHighlightOFF();
                debug = 0;
                ui->debugnextbtn->setEnabled(true);
                return;
            }
            ui->debugnextbtn->setEnabled(true);
        }
    }
    /*************************************************************************************************************************/
}

void MainWindow::on_skipButton_clicked()
{
    if (debug == 1)
    {
        string dir = fileName.toStdString();

        if (dir.substr(dir.find_last_of(".") + 1) == "tme" || dir.substr(dir.find_last_of(".") + 1) == "txt")
        {
            dir.pop_back();
            dir.pop_back();
            dir.pop_back();
            dir.pop_back();
        }

        sqlite3 *db = 0;
        char *err = 0;
        sqlite3_stmt *ppStmt;

        TuringMachine tm;
        ui->debuglineEdit->setReadOnly(1);

        // ui->skipButton->setEnabled(false);

        if (tm.load_strip("datasection.tmp"))
        {
            tm.set_current_state(ui->debugstatelbl->text().toStdString());
            tm.set_current_word(ui->debugwordlbl->text().toStdString());

            string a = dir;

            /**************/
            a.append(".db");

            sqlite3_open(a.c_str(), &db);
            // sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &err); //SO FUCKING RISKY
            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err);
            string select_command;
            int currentline = 0;
            if (!tm.is_end(dir, (int)ui->lambdacheckBox->isChecked()) && ui->debugstatelbl->text().toStdString() != "end")
            {
                while (!tm.is_end(dir, (int)ui->lambdacheckBox->isChecked()))
                {
                    select_command = "SELECT *FROM commands WHERE initial_state=\"" + tm.get_current_state() + "\" AND initial_word=\"" + tm.get_current_word() + "\"";
                    sqlite3_prepare_v2(db, select_command.c_str(), 256, &ppStmt, NULL);

                    if (sqlite3_step(ppStmt) != SQLITE_DONE)
                    {
                        tm.set_current_state(string((char *)sqlite3_column_text(ppStmt, 2)));
                        tm.set_current_word(string((char *)sqlite3_column_text(ppStmt, 3)));
                        tm.get_step(((char *)sqlite3_column_text(ppStmt, 4))[0]);
                        currentline = atoi(string((char *)sqlite3_column_text(ppStmt, 6)).c_str());
                    }
                    else
                    {
                        ui->logwindow->appendPlainText("EMULATING ERROR : CANT FIND NEXT COMMAND");
                        sqlite3_finalize(ppStmt);
                        sqlite3_close(db);
                        ui->logwindow->appendPlainText("Database closed");
                        breakpointHighlightOFF();

                        ui->debuglineEdit->setText(tm.get_strip((int)ui->lambdacheckBox->isChecked()).c_str());
                        ui->debugstatelbl->setText(tm.get_current_state().c_str());
                        ui->debugwordlbl->setText(tm.get_current_word().c_str());

                        debug = 0;
                        currentLineHighlight(currentline);
                        ui->skipButton->setEnabled(true);
                        return; //сюда ебашить подсветку
                    }

                    if (string((char *)sqlite3_column_text(ppStmt, 5)) == "1")
                    {
                        ui->logwindow->appendPlainText("Founded breakpoint");
                        ui->debuglineEdit->setText(tm.get_strip((int)ui->lambdacheckBox->isChecked()).c_str());
                        ui->debugstatelbl->setText(tm.get_current_state().c_str());
                        ui->debugwordlbl->setText(tm.get_current_word().c_str());

                        ofstream fout;
                        fout.open("datasection.tmp");

                        for (int i = 0; i < tm.get_strip(0).size(); i++)
                        {
                            fout << tm.get_strip(0)[i];
                        }
                        fout.close();

                        currentLineHighlight(currentline);

                        sqlite3_finalize(ppStmt);
                        sqlite3_close(db);
                        ui->skipButton->setEnabled(true);
                        return; //сюда ебашить подсветку
                    }

                    sqlite3_finalize(ppStmt);
                }
                ui->skipButton->setEnabled(true);
                sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
                sqlite3_close(db);
            }
            ui->debuglineEdit->setText(tm.get_strip((int)ui->lambdacheckBox->isChecked()).c_str());
            ui->debugstatelbl->setText(tm.get_current_state().c_str());
            ui->debugwordlbl->setText(tm.get_current_word().c_str());
            breakpointHighlightOFF();
            debug = 0;
            ui->mainTextField->document()->clearUndoRedoStacks();
            ui->logwindow->appendPlainText("Debugging ended");
        }
        else
        {
            ui->skipButton->setEnabled(true);
            ui->logwindow->appendPlainText("there are some troubles with .data file");
            debug = 0;
            return;
        }
    }
}

void MainWindow::on_action_triggered()
{
    QMessageBox::about(0,
                       "Про мову цього емулятора",
                       "Синтаксис:\n"
                       "Має два заразервованих стани, 3 ключові вирази та один символ.\n"
                       "В мові елементарною одиницею є рядок, який завжди має вид: \n\n statement1, word1->statement2, word2, direction\n\n"
                       "В ідентифікаторах станів та алфавіту допустимі лише латинські букви будь-якого регістру, цифри та нижні підкреслення \"_\"\n"
                       "Мова не має обмежень двжини ідентификаторів, рядків та не залежить від кількости чи наявності відступів, табуляцій, пропусків.\n\n\n"
                       "section\n\nЯк і в мовах ассемблерів, мова данного емулятора розділена на секції \nsection .text; section .data;\nСекції для коду та вхідного рядка відповідно. Їх може бути декілька. З .data обирається остання, .text зливаються в одну.\n\n"
                       "Макроси\n\nМакроси подібні макросам з мови С, #define st1 st2, де st1 заміниться на st2. Макроси підставляються по черзі з вернього вниз. Можуть маніпулювати всім у секції коду.\n\n"
                       "section .data\n\nКожна клітинка стрічки має довільний розмір та відділяється від інших пропуском \" \", курсор вказується за допомогою обводки \"|\", наприклад:\"|lambda|\".\n\n"
                       "Коментарі:\nЗа ассемблерною традицією комментарі мають лише одно-рядковий синтаксис та діють з \";\" ф до кінця рядка, у довільному рядку программи.\n\n"
                       "Остаточно про виконання мови:\nВиконання завжди починається за стану \"start\" та слова, що було виділено курсором, кінець - стан \"end\", lambda - зарезервоване слово, що позначає пустий символ."
                       "\n\nДетальніше на гітхаб Kaifolog/TME,\nтам є ціла нормальна інструкція!");
    QMessageBox msgBox;
    msgBox.setText("<a href=\"https://github.com/Kaifolog/TME\" style=\"color:red\">Інструкція та актуальна версія</a>");
    msgBox.exec();
}
void MainWindow::on_action_2_triggered()
{
    QMessageBox::about(this,
                       "Гарячі клавіші.",
                       "Синтаксис:\n"
                       "Ctrl+S - save\nCtrl+N - New\nCtrl+O - open\nCtrl+Shift+X - Quick Start\nCtrl+Tab - підстановка розділових знаків до форми команди \",->,,\"\n F5 - Робить 1 ітерацію у debug режимі");
    QMessageBox msgBox;
    // msgBox.setText("<a href=\"https://github.com/Kaifolog/TME\" style=\"color:red\">Інструкція та актуальна версія</a>");
    // msgBox.exec();
}

void MainWindow::on_action_3_triggered()
{
    QMessageBox::about(this,
                       "Приклад програми.",
                       "Виконаємо порт з Оніщенка:\n\n"
                       "#define ,: ,lambda: ; на нашому емуляторі недопустимі пусті строки\n"
                       "#define ,, ,lambda, ; на нашому компіляторі недопустимі пусті строки\n"
                       "#define q0 start ; змінюємо точку входу\n"
                       "#define ! end ; змінюємо точку кінця\n"
                       "#define * star ; \n"
                       "#define : -> ; \n"
                       "section .data\n"
                       "|lambda|\n"
                       "section .text\n"
                       "q0,:q1,g,r\n"
                       "q1,:q2,e,r\n"
                       "q2,:q3,o,r\n"
                       "q3,:q4,r,r\n"
                       "q4,:q5,g,r\n"
                       "q5,:q6,e,r\n"
                       "q6,:!,,r\n");
    QMessageBox msgBox;
    msgBox.setText("<a href=\"https://github.com/Kaifolog/TME\" style=\"color:red\">Інструкція та актуальна версія</a>");
    msgBox.exec();
}

void MainWindow::on_action_5_triggered()
{
    QMessageBox::about(this,
                       "Про емулятор\n\n",
                       "Єдина неприємність - верхній рядок з графічного інтерфейсу є аналогом section .data, куди він і збережується.\n"
                       "При відкритті ви побачите, що він в головному полі тексту. Можете сам рядок рядок без вказання секції переписати в інпут рядок граф. інтерфейсу. Якщо він пустий - він не буде записаний.\n"
                       "Щоб дізнатися деталі чи зв'язатися з розробником - відкрийте вкладку \"Про программу\""
                       "Читайте інструкцію.");
}

void MainWindow::on_action_4_triggered()
{
    QMessageBox::about(this,
                       "Про программу\n\n",
                       "Присвячено Україні та всім моїм друзям і подругам, Kanye \"Ye\" West'у.\n"
                       "Якщо мене ще не відрахували та мені ще цікаво - я буду продовжувати розробляти емулятор, маєш якісь ідеї або хочеш допомогти? Хочеш отримати консольну версію?\n\n");
    QMessageBox msgBox;
    msgBox.setText("<a href=\"https://github.com/Kaifolog/TME\" style=\"color:red\">Мій Github</a>");
    msgBox.exec();
}

void MainWindow::on_mainTextField_cursorPositionChanged()
{
    QPlainTextEdit *edit = qobject_cast<QPlainTextEdit *>(sender());
    Q_ASSERT(edit);
    QTextCursor cursor = edit->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);

    int lines = 1;
    while (cursor.positionInBlock() > 0)
    {
        cursor.movePosition(QTextCursor::Up);
        lines++;
    }
    QTextBlock block = cursor.block().previous();

    while (block.isValid())
    {
        lines += block.lineCount();
        block = block.previous();
    }
    if (ui->datacheckBox->isChecked())
        ui->rawnumberlabel->setText(QString::number(lines + 2));
    else
        ui->rawnumberlabel->setText(QString::number(lines));
}
