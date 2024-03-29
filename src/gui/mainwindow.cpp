#include "mainwindow.h"

MainWindow::MainWindow(QApplication *app, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->app = app;

    const QByteArray logo_base64_ba = QByteArray::fromBase64(logo_base64);

    QPixmap pixmap;
    pixmap.loadFromData(logo_base64_ba);

    setWindowIcon(QIcon(pixmap));

    // connects keybind CTRL + S
    keyCtrlS = new QShortcut(this);
    keyCtrlS->setKey(Qt::CTRL + Qt::Key_S);
    connect(keyCtrlS, SIGNAL(activated()), this, SLOT(slotShortcutCtrlS()));
    // connects keybind CTRL + O
    keyCtrlO = new QShortcut(this);
    keyCtrlO->setKey(Qt::CTRL + Qt::Key_O);
    connect(keyCtrlO, SIGNAL(activated()), this, SLOT(slotShortcutCtrlO()));
    // connects keybind CTRL + N
    keyCtrlN = new QShortcut(this);
    keyCtrlN->setKey(Qt::CTRL + Qt::Key_N);
    connect(keyCtrlN, SIGNAL(activated()), this, SLOT(slotShortcutCtrlN()));
    // connects keybind CTRL + SHIFT + X
    CtrlShiftX = new QShortcut(this);
    CtrlShiftX->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_X);
    connect(CtrlShiftX, SIGNAL(activated()), this, SLOT(slotShortcutCtrlShiftX()));
    // connects keybind CTRL + Tab
    CtrlTab = new QShortcut(this);
    CtrlTab->setKey(Qt::CTRL + Qt::Key_Tab);
    connect(CtrlTab, SIGNAL(activated()), this, SLOT(slotShortcutCtrlTab()));
    // connects keybind F5
    CtrlSpace = new QShortcut(this);
    CtrlSpace->setKey(Qt::Key_F5);
    connect(CtrlSpace, SIGNAL(activated()), this, SLOT(slotShortcutCtrlSpace()));
    // connects keybind CTRL + D
    CtrlD = new QShortcut(this);
    CtrlD->setKey(Qt::CTRL + Qt::Key_D);
    connect(CtrlD, SIGNAL(activated()), this, SLOT(slotShortcutCtrlD()));

    // seting settings
    QCoreApplication::setOrganizationName("Bulak R&D");
    QCoreApplication::setOrganizationDomain("kaifolog.github.io");
    QCoreApplication::setApplicationName("TME");

    MainWindow::readSettings();
}

/* settings */

void MainWindow::versionReplyHandler(QNetworkReply *reply)
{
    if (QNetworkReply::NoError == reply->error())
    {
        QSettings settings;
        QString html = reply->readAll();
        if (settings.value("global/version").toString() != html)
        {
            ui->logwindow->appendPlainText("\nNew version " + html +
                                           " is available. Download it from the product website.\n");

            QMessageBox::about(this, "Update notification",
                               "<h3> Нова версія емулятора (" + html +
                                   ") вже доступна! </h3>"

                                   "<h4> Завантажити її можна на сайті проекту:"
                                   "<div><a href=\"https://kaifolog.github.io/TME-website/\" "
                                   "style=\"color:#0057b7\">https://kaifolog.github.io/TME-website/</a>");

            settings.setValue("global/update_notified", "true");
            settings.setValue("global/new_version", html);
        }
    }
    else
    {
        ui->logwindow->appendPlainText("\nUnable to check for updates: network is unavalable.\n");
    }
}

void MainWindow::readSettings()
{
    QSettings settings;

    if (settings.value("global/version") != "2.0.0b")
    {
        settings.beginGroup("global");
        settings.setValue("version", "2.0.0b");
        settings.setValue("update_notified", "false");
        settings.setValue("new_version", "2.0.1b");
        settings.endGroup();
        settings.beginGroup("appearance");
        settings.setValue("theme", "moonlight");
        settings.setValue("hello_kitty", "false");
        settings.endGroup();
        settings.beginGroup("editor");
        settings.setValue("save_last_path", "true");
        settings.setValue("last_path", "");
        settings.setValue("clear_tmp_files", "true");
        settings.endGroup();
        settings.beginGroup("machine");
        settings.setValue("max_steps", "1000000");
        settings.endGroup();
    }

    // opens last file of previuos session
    if (settings.value("editor/save_last_path").toBool())
    {
        _pname.setOriginal(settings.value("editor/last_path").toString().toStdString());
        if (not _pname.empty())
        {
            ui->logwindow->document()->setPlainText("");
            ui->logwindow->appendPlainText(
                QString::fromStdString(std::string("The last file of the previous session is open:\n")) +
                settings.value("editor/last_path").toString());
            openInEditor();
        }
        else
        {
            showStartMessage();
        }
    }

    // setting theme
    if (settings.value("appearance/theme").toString().toStdString() == std::string("moonlight"))
    {
        this->app->setStyleSheet(themes::MoonlightTheme);
    }
    if (settings.value("appearance/theme").toString().toStdString() == std::string("blackseasunrise"))
    {
        this->app->setStyleSheet(themes::BlackSeaSunriseTheme);
    }

    // updater request
    if (not settings.value("global/update_notified").toBool())
    {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::versionReplyHandler);
        manager->get(QNetworkRequest(QUrl("https://kaifolog.github.io/TME-website/version.html")));
    }
    else
    {
        ui->logwindow->appendPlainText(
            "\nA new version of the emulator (" + settings.value("global/new_version").toString() +
            ") is available. Download it from the product website, which can be found in Settings/About program.\n");
    }

    setHelloKitty(settings.value("appearance/hello_kitty").toBool());
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.setValue("editor/last_path", QString::fromStdString(_pname.getOriginal()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();

    QSettings settings;
    if (settings.value("editor/clear_tmp_files").toBool() and not _pname.empty())
    {
        try
        {
            QFile datasection(QString("datasection.tmp"));
            datasection.remove();
            QFile db_file(QString::fromStdString(_pname.getDBFile()));
            db_file.remove();
            QFile out_file(QString::fromStdString(_pname.getOutFile()));
            out_file.remove();
        }
        catch (...)
        {
        }
    }
}

/* utility functions */

void MainWindow::setHelloKitty(bool enabled)
{
    if (enabled)
    {
        QByteArray hk = QByteArray::fromBase64(hk_base64);
        QPixmap pix;
        pix.loadFromData(hk, "PNG");
        ui->hello_kitty_label->setPixmap(pix);
    }
    else
    {
        QByteArray none;
        QPixmap pix;
        pix.loadFromData(none, "PNG");
        ui->hello_kitty_label->setPixmap(pix);
    }
}

void MainWindow::debugPanelHighlightEnabled(bool enabled)
{
    QSettings settings;
    if (enabled)
    {
        if (settings.value("appearance/theme").toString().toStdString() == std::string("moonlight"))
        {
            ui->debugnextbtn->setStyleSheet(
                "QPushButton { background-color: #5c73f2;} QPushButton:hover { background-color: #333333 }");
            ui->skipButton->setStyleSheet(
                "QPushButton { background-color: #5c73f2;} QPushButton:hover { background-color: #333333 }");
            ui->debugstatelbl->setStyleSheet("QLabel { border-color: #5c73f2 }");
            ui->debugwordlbl->setStyleSheet("QLabel { border-color: #5c73f2 }");
        }
        else if (settings.value("appearance/theme").toString().toStdString() == std::string("blackseasunrise"))
        {
            ui->debugnextbtn->setStyleSheet(
                "QPushButton { background-color: #5eb5f7;} QPushButton:hover { background-color: #293a4c }");
            ui->skipButton->setStyleSheet(
                "QPushButton { background-color: #5eb5f7;} QPushButton:hover { background-color: #293a4c }");
            ui->debugstatelbl->setStyleSheet("QLabel { border-color: #5eb5f7 }");
            ui->debugwordlbl->setStyleSheet("QLabel { border-color: #5eb5f7 }");
        }
    }
    else
    {
        if (settings.value("appearance/theme").toString().toStdString() == std::string("moonlight"))
        {
            ui->mainTextField->setStyleSheet("QPlainTextEdit { background-color: #202020;}");
            ui->debugnextbtn->setStyleSheet("QPushButton { background-color: #333333 }");
            ui->skipButton->setStyleSheet("QPushButton { background-color: #333333 }");
            ui->debugstatelbl->setStyleSheet("QLabel { border-color: #333333 }");
            ui->debugwordlbl->setStyleSheet("QLabel { border-color: #333333 }");
        }
        else if (settings.value("appearance/theme").toString().toStdString() == std::string("blackseasunrise"))
        {
            ui->mainTextField->setStyleSheet("QPlainTextEdit { background-color: #ffffff;}");
            ui->debugnextbtn->setStyleSheet("QPushButton { background-color: #c2dcf2 }");
            ui->skipButton->setStyleSheet("QPushButton { background-color: #c2dcf2 }");
            ui->debugstatelbl->setStyleSheet("QLabel { border-color: #c2dcf2 }");
            ui->debugwordlbl->setStyleSheet("QLabel { border-color: #c2dcf2 }");
        }
    }
}

void MainWindow::showStartMessage()
{
    ui->mainTextField->document()->setPlainText(
        "Hey! To start you have to open the file.\n\nSome hotkeys:"
        "\nCtrl+N - New file\nCtrl+O - Open the file\nCtrl+S - Save current file");
    ui->logwindow->appendPlainText("Please, open the file.");
    ui->mainTextField->setEnabled(false);
}

void MainWindow::openInEditor()
{
    QFile file(QString::fromUtf8(_pname.getOriginal().c_str()));
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString text;
        text = file.readAll();

        // if user wants to use inputlineEdit we do it
        if (text.left(text.indexOf(QChar('\n'))).remove(QRegularExpression("[\\s]+")).toStdString() == "section.data" &&
            ui->datacheckBox->isChecked())
        {
            QStringList list = text.split('\n');
            QString input_data = list[1];
            list.removeFirst();
            list.removeFirst();
            text = list.join('\n');
            ui->inputlineEdit->setText(input_data);
        }
        else
            ui->inputlineEdit->clear();

        // sets text to the mainfield
        ui->mainTextField->clear();
        ui->mainTextField->setPlainText(text);
        file.close();

        // sets name of the current file
        ui->filenamelbl->setText(QString::fromUtf8(_pname.getOriginal().c_str()));
        ui->filestatuslbl->setText("Opened");

        file.close();

        // logger configuring
        el::Configurations defaultConf;
        defaultConf.setToDefault();
        defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime : %msg");
        defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
        defaultConf.setGlobally(el::ConfigurationType::Filename, _pname.getLogFile());
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "104857600");
        el::Loggers::reconfigureLogger("default", defaultConf);

        clearLogFile();
        ui->mainTextField->setEnabled(true);
    }
    else
    {
        ui->logwindow->appendPlainText("Path error");
    }
}

void MainWindow::clearLogFile()
{
    std::ofstream ofs;
    ofs.open(_pname.getLogFile(), std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

void MainWindow::breakpointHighlightON()
{
    if (!_pname.empty())
    {

        sqlite3 *db = nullptr;
        char *err = nullptr;
        sqlite3_stmt *ppStmt;

        sqlite3_open(_pname.getDBFile().c_str(), &db);
        sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err);

        std::string select_command;
        select_command = "SELECT * FROM commands WHERE debug=\"1\"";
        sqlite3_prepare_v2(db, select_command.c_str(), 256, &ppStmt, NULL);
        int resultSqlite3 = sqlite3_step(ppStmt);

        while (resultSqlite3 == SQLITE_ROW)
        {
            QTextCursor cur = ui->mainTextField->textCursor();
            int linenumber = atoi(std::string((char *)sqlite3_column_text(ppStmt, 6)).c_str());
            cur.movePosition(QTextCursor::Start);
            if (ui->inputlineEdit->text().length())
            {
                linenumber -= 2; // because of deleted by on_datacheckBox_clicked two rows at the start of a file
            }
            for (int i = 0; i < linenumber - 1; i++)
                cur.movePosition(QTextCursor::Down);
            QTextBlockFormat formater;

            QSettings settings;
            if (settings.value("appearance/theme").toString().toStdString() == std::string("moonlight"))
            {
                formater.setBackground(QBrush("#550e12"));
            }
            else if (settings.value("appearance/theme").toString().toStdString() == std::string("blackseasunrise"))
            {
                formater.setBackground(QBrush("#ff8787"));
            }

            cur.select(QTextCursor::LineUnderCursor);
            cur.setBlockFormat(formater);

            resultSqlite3 = sqlite3_step(ppStmt);
        }

        sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
        sqlite3_free(err);
        sqlite3_finalize(ppStmt);
        sqlite3_close(db);
    }
}

void MainWindow::currentLineHighlight(int line)
{
    if (ui->inputlineEdit->text().length())
    {
        line -= 2; // because of deleted by on_datacheckBox_clicked two rows at the start of a file
    }

    QTextCursor cur = ui->mainTextField->textCursor();
    cur.movePosition(QTextCursor::Start);
    for (int i = 0; i < highlightedLine - 1; i++)
        cur.movePosition(QTextCursor::Down);
    QTextBlockFormat formater;
    QSettings settings;
    if (settings.value("appearance/theme").toString().toStdString() == std::string("moonlight"))
    {
        formater.setBackground(QBrush("#202020"));
    }
    else if (settings.value("appearance/theme").toString().toStdString() == std::string("blackseasunrise"))
    {
        formater.setBackground(QBrush("#ffffff"));
    }
    cur.select(QTextCursor::LineUnderCursor);
    cur.setBlockFormat(formater);

    highlightedLine = line;

    breakpointHighlightON();

    QTextCursor cur1 = ui->mainTextField->textCursor();
    cur1.movePosition(QTextCursor::Start);
    for (int i = 0; i < line - 1; i++)
        cur1.movePosition(QTextCursor::Down);
    QTextBlockFormat formater1;

    if (settings.value("appearance/theme").toString().toStdString() == std::string("moonlight"))
    {
        formater1.setBackground(QBrush("#4b4b18"));
    }
    else if (settings.value("appearance/theme").toString().toStdString() == std::string("blackseasunrise"))
    {
        formater1.setBackground(QBrush("#ffc347"));
    }

    cur1.select(QTextCursor::LineUnderCursor);
    cur1.setBlockFormat(formater1);

    const int EDITOR_WIDTH_IN_ROWS = 24;
    if (line > EDITOR_WIDTH_IN_ROWS)
    {
        QTextCursor cursor(ui->mainTextField->document()->findBlockByLineNumber(
            line - 1)); // (line - 1) because line number is starts from 0
        ui->mainTextField->setTextCursor(cursor);
    }
}

void MainWindow::breakpointHighlightOFF()
{

    QTextCursor cursor = ui->mainTextField->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cursor.select(QTextCursor::Document);
    QTextBlockFormat formater1;
    QSettings settings;
    if (settings.value("appearance/theme").toString().toStdString() == std::string("moonlight"))
    {
        formater1.setBackground(QBrush("#202020"));
    }
    else if (settings.value("appearance/theme").toString().toStdString() == std::string("blackseasunrise"))
    {
        formater1.setBackground(QBrush("#ffffff"));
    }
    // #202020 // #550e12 // #4b4b18
    // cur1.select(QTextCursor::LineUnderCursor);
    cursor.setBlockFormat(formater1);
}

void MainWindow::AllButtonsSetEnabled(bool state)
{
    ui->parsingbtn->setEnabled(state);
    ui->analysisbtn->setEnabled(state);
    ui->emulationbtn->setEnabled(state);
    ui->quickstartbtn->setEnabled(state);
    ui->debugbtn->setEnabled(state);
}

/* keybinds slots */

void MainWindow::slotShortcutCtrlTab()
{
    if (ui->rowNumberLabel->text().length() and ui->mainTextField->isEnabled())
    {
        int raw_number = ui->rowNumberLabel->text().toInt();

        if (ui->datacheckBox->isChecked())
        {
            raw_number -= 2;
        }

        QString main_textField = ui->mainTextField->toPlainText();
        QStringList str_list = main_textField.split('\n');  // splits textEdit text by \n
        QString original_str = str_list.at(raw_number - 1); // gets a second result of the split

        QStringList comma_test = original_str.split(',');
        QStringList arrow_test = original_str.split("->");

        QTextCursor text_cursor = QTextCursor(ui->mainTextField->document());

        // getts cursor to the current row
        for (int i = 0; i < raw_number - 1; i++)
            text_cursor.movePosition(QTextCursor::Down);

        text_cursor.movePosition(QTextCursor::EndOfLine);

        // appends punctuation
        if (comma_test.length() == 1)
            text_cursor.insertText(",");
        if (comma_test.length() == 2 && arrow_test.length() == 1)
            text_cursor.insertText("->");
        if (comma_test.length() == 2 && arrow_test.length() == 2)
            text_cursor.insertText(",");
        if (comma_test.length() == 3 && arrow_test.length() == 2)
            text_cursor.insertText(",");
        if (comma_test.length() == 4 && arrow_test.length() == 2)
            text_cursor.insertText("\n");
    }
}

void MainWindow::slotShortcutCtrlD()
{
    if (ui->rowNumberLabel->text().length())
    {
        int raw_number = ui->rowNumberLabel->text().toInt();

        if (ui->datacheckBox->isChecked())
        {
            raw_number -= 2;
        }

        QString main_textField = ui->mainTextField->toPlainText();
        QStringList str_list = main_textField.split('\n'); // splits textEdit text by \n
        main_textField = str_list.at(raw_number - 1);      // gets a second result of the split

        str_list = main_textField.split(";#d");

        QTextCursor text_cursor = QTextCursor(ui->mainTextField->document());

        // get s cursor to the current row
        for (int i = 0; i < raw_number - 1; i++)
            text_cursor.movePosition(QTextCursor::Down);

        text_cursor.movePosition(QTextCursor::EndOfLine);

        // appends punctuation
        if (str_list.size() == 1)
        {
            text_cursor.insertText("\t;#d");
        }
        else
        {
            for (int i = 0; i < str_list[(str_list.size() - 1)].size() + 4; i++)
            {
                text_cursor.movePosition(QTextCursor::Left);
                text_cursor.deleteChar();
            }
        }
    }
}

/* triggers for files actions */

void MainWindow::on_actionOpen_triggered()
{
    NORMALMIDDLEWARE

    QString newFile;
    QString fileText;

    // if there is a last path -- we start in their directory
    if (_pname.empty())
        newFile = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("Documents (*.tme *.txt)"));
    else
    {
        fileText = QString::fromUtf8(_pname.getOriginal().c_str()).left(newFile.lastIndexOf(QChar('/')));
        newFile = QFileDialog::getOpenFileName(this, tr("Open File"), fileText, tr("Documents (*.tme *.txt)"));
    }
    if (not newFile.isEmpty()) // if newFile is not empty
    {
        _pname.setOriginal(newFile.toStdString());

        ui->logwindow->document()->setPlainText("");

        openInEditor();
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (!_pname.empty())
    {
        std::ofstream out;
        // I think I don't need to check the state of the file. If the file does not exist, then ofstream should
        // create it.
        out.open(_pname.getOriginal());
        if (ui->inputlineEdit->text().size() && ui->datacheckBox->isChecked())
        {
            out << "section .data" << std::endl;
            out << ui->inputlineEdit->text().toStdString() << std::endl;
        }
        out << ui->mainTextField->toPlainText().toStdString();
        ui->filenamelbl->setText(QString::fromUtf8(_pname.getOriginal().c_str()));
        ui->filestatuslbl->setText("Saved");

        out.close();
    }
}

void MainWindow::on_actionClose_triggered()
{
    NORMALMIDDLEWARE

    QSettings settings;
    if (settings.value("editor/clear_tmp_files").toBool() and not _pname.empty())
    {
        try
        {
            QFile datasection(QString("datasection.tmp"));
            datasection.remove();
            QFile db_file(QString::fromStdString(_pname.getDBFile()));
            db_file.remove();
            QFile out_file(QString::fromStdString(_pname.getOutFile()));
            out_file.remove();
        }
        catch (...)
        {
        }
    }
    _pname.clear();

    ui->debuglineEdit->clear();
    ui->mainTextField->clear();
    ui->inputlineEdit->clear();
    ui->debugstatelbl->clear();
    ui->debugwordlbl->clear();
    ui->logwindow->clear();
    ui->filenamelbl->clear();
    ui->filestatuslbl->clear();
    ui->mainTextField->document()->clearUndoRedoStacks();
    showStartMessage();
}

void MainWindow::on_actioNew_triggered()
{
    NORMALMIDDLEWARE

    QString newFile;
    QString fileText;

    if (_pname.empty())
        newFile = QFileDialog::getSaveFileName(this, tr("Save File"), "/", tr("Documents (*.tme *.txt)"));
    else
    {
        fileText = QString::fromUtf8(_pname.getOriginal().c_str())
                       .left(QString::fromUtf8(_pname.getOriginal().c_str()).lastIndexOf(QChar('/')));
        newFile = QFileDialog::getSaveFileName(this, tr("Save File"), fileText, tr("Documents (*.tme *.txt)"));
    }
    if (not newFile.isEmpty()) // if newFile is not empty
    {
        _pname.setOriginal(newFile.toStdString());
        QFile mFile(QString::fromUtf8(_pname.getOriginal().c_str()));
        mFile.open(QIODevice::WriteOnly);
        mFile.close();

        openInEditor();
        ui->logwindow->appendPlainText("File has been created.");
    }
}

/* triggers for textfields */

void MainWindow::on_datacheckBox_clicked()
{
    if (ui->mainTextField->isEnabled())
    {
        if (ui->datacheckBox->isChecked())
        {
            QString text = ui->mainTextField->toPlainText();
            if (text.left(text.indexOf(QChar('\n'))).remove(QRegularExpression("[\\s]+")).toStdString() ==
                "section.data")
            {
                QStringList list = text.split('\n');
                QString input_data = list[1];
                list.removeFirst();
                list.removeFirst();
                text = list.join('\n');
                ui->inputlineEdit->setText(input_data);
                ui->mainTextField->clear();
                ui->mainTextField->setPlainText(text);
                ui->mainTextField->moveCursor(QTextCursor::Start);
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
            ui->mainTextField->moveCursor(QTextCursor::Start);
        }
    }
}

void MainWindow::on_mainTextField_textChanged()
{
    ui->filestatuslbl->setText("Changed");
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
        ui->rowNumberLabel->setText(QString::number(lines + 2));
    else
        ui->rowNumberLabel->setText(QString::number(lines));
}

/* middleware functions */

void MainWindow::NormalMiddleware()
{
    if (debugMode)
    {
        debugMode = 0;
        breakpointHighlightOFF();
        ui->mainTextField->document()->clearUndoRedoStacks();
        ui->logwindow->appendPlainText("Debugging ended...");
        ui->mainTextField->setFocus();
        try
        {
            _debugger.lazyFinalize();
        }
        catch (const char *message)
        {
            ui->logwindow->appendPlainText(message);
        }
        catch (std::string message)
        {
            ui->logwindow->appendPlainText(message.c_str());
        }

        debugPanelHighlightEnabled(false);
    }
    ui->debuglineEdit->clear();
    ui->debugstatelbl->clear();
    ui->debugwordlbl->clear();
}

void MainWindow::DebugMiddleware()
{
    if (ui->debugstatelbl->text() == "end")
    {
        debugMode = 0;
        breakpointHighlightOFF();
        ui->mainTextField->document()->clearUndoRedoStacks();
        ui->logwindow->appendPlainText("Debugging ended...");
        ui->mainTextField->setFocus();
        try
        {
            _debugger.lazyFinalize();
        }
        catch (const char *message)
        {
            ui->logwindow->appendPlainText(message);
        }
        catch (std::string message)
        {
            ui->logwindow->appendPlainText(message.c_str());
        }
        debugPanelHighlightEnabled(false);
    }
}

/* triggers for buttons */

void MainWindow::on_parsingbtn_clicked()
{
    AllButtonsSetEnabled(false);

    if (!_pname.empty())
    {
        NORMALMIDDLEWARE

        on_actionSave_triggered();

        clearLogFile();

        try
        {
            translator::Translator translator;
            LOG(INFO) << "Translating started... ";

            translator.parse(_pname);

            LOG(INFO) << "Translating ended.";
        }
        catch (const char *message)
        {
            LOG(ERROR) << message;
        }
        catch (char *message)
        {
            LOG(ERROR) << message;
        }
        catch (std::string message)
        {
            LOG(ERROR) << message;
        }
        catch (...)
        {
            LOG(ERROR) << "Something went wrong." << std::endl
                       << "If this happens often you should tell the developer about it." << std::endl;
        }

        QFile file(QString::fromUtf8(_pname.getLogFile().c_str()));
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
    AllButtonsSetEnabled(true);
}

void MainWindow::on_analysisbtn_clicked()
{
    AllButtonsSetEnabled(false);

    if (!_pname.empty())
    {
        NORMALMIDDLEWARE

        clearLogFile();

        try
        {
            translator::Translator translator;
            LOG(INFO) << "Starting translator...";

            translator.analyse(_pname);

            LOG(INFO) << "Analysis complete. All set for execution!";
        }
        catch (const char *message)
        {
            LOG(ERROR) << message;
        }
        catch (char *message)
        {
            LOG(ERROR) << message;
        }
        catch (std::string message)
        {
            LOG(ERROR) << message;
        }
        catch (...)
        {
            LOG(ERROR) << "Something went wrong." << std::endl
                       << "Are you sure this file has already been parsed?" << std::endl
                       << "If this happens often you should tell the developer about it." << std::endl;
        }
        QFile file(QString::fromUtf8(_pname.getLogFile().c_str()));
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            QString text = file.readAll();
            ui->debuglineEdit->clear();
            ui->logwindow->clear();
            ui->logwindow->appendPlainText(text);
        }
        file.close();
    }
    AllButtonsSetEnabled(true);
}

void MainWindow::on_emulationbtn_clicked()
{
    AllButtonsSetEnabled(false);

    if (!_pname.empty())
    {
        NORMALMIDDLEWARE

        clearLogFile();

        try
        {
            QSettings settings;
            machine::TuringMachine tm(settings.value("machine/max_steps").toInt());
            LOG(INFO) << "Starting emulator...";

            tm.execute(_pname, ui->lambdacheckBox->isChecked());

            LOG(INFO) << "Run complete";

            QFile file1(QString::fromUtf8(_pname.getOutFile().c_str()));
            if (file1.open(QIODevice::ReadOnly))
            {
                QTextStream in(&file1);
                QString text = file1.readAll();

                ui->debuglineEdit->clear();
                ui->debuglineEdit->setText(text);
                file1.close();
            }
        }
        catch (const char *message)
        {
            LOG(ERROR) << message;
        }
        catch (char *message)
        {
            LOG(ERROR) << message;
        }
        catch (std::string message)
        {
            LOG(ERROR) << message;
        }
        catch (...)
        {
            LOG(ERROR) << "Something went wrong." << std::endl
                       << "Are you sure this file has already been parsed?" << std::endl
                       << "If this happens often you should tell the developer about it." << std::endl;
        }
        QFile file(QString::fromUtf8(_pname.getLogFile().c_str()));
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            QString text;
            text = file.readAll();
            ui->logwindow->clear();
            ui->logwindow->appendPlainText(text);
            file.close();
        }
    }
    AllButtonsSetEnabled(true);
}

void MainWindow::on_quickstartbtn_clicked()
{
    AllButtonsSetEnabled(false);

    if (!_pname.empty())
    {
        NORMALMIDDLEWARE

        on_actionSave_triggered();

        clearLogFile();

        try
        {
            translator::Translator translator;
            LOG(INFO) << "Translating started... ";
            translator.parse(_pname);
            LOG(INFO) << "Translating ended.";

            QSettings settings;
            machine::TuringMachine tm(settings.value("machine/max_steps").toInt());
            LOG(INFO) << "Starting emulator...";
            tm.execute(_pname, (bool)ui->lambdacheckBox->isChecked());
            LOG(INFO) << "Run complete";

            QFile file1(QString::fromUtf8(_pname.getOutFile().c_str()));
            if (file1.open(QIODevice::ReadOnly))
            {
                QTextStream in(&file1);
                QString text = file1.readAll();

                ui->debuglineEdit->clear();
                ui->debuglineEdit->setText(text);
                file1.close();
            }
        }
        catch (const char *message)
        {
            LOG(ERROR) << message;
        }
        catch (char *message)
        {
            LOG(ERROR) << message;
        }
        catch (std::string message)
        {
            LOG(ERROR) << message;
        }
        catch (...)
        {
            LOG(ERROR) << "Something went wrong." << std::endl
                       << "If this happens often you should tell the developer about it." << std::endl;
        }

        QFile file(QString::fromUtf8(_pname.getLogFile().c_str()));
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            QString text = file.readAll();
            ui->logwindow->clear();
            ui->logwindow->appendPlainText(text);
            file.close();
        }
    }
    AllButtonsSetEnabled(true);
}

void MainWindow::on_debugbtn_clicked()
{
    NORMALMIDDLEWARE

    if (!_pname.empty())
    {

        on_actionSave_triggered();
        on_parsingbtn_clicked();
        breakpointHighlightON();

        ui->debugbtn->setEnabled(false);

        debugMode = 1;

        ui->logwindow->appendPlainText("Starting debugger...");

        machine::MachineState result;
        try
        {
            QSettings settings;
            _debugger.setMaxStep(settings.value("machine/max_steps").toInt());
            _debugger.lazyStart(_pname, ui->lambdacheckBox->isChecked());
            result = _debugger.lazyDebug(true);
            currentLineHighlight(std::stoi(result.line));
            ui->debuglineEdit->setText(result.current_strip.c_str());
            ui->debugstatelbl->setText(result.current_state.c_str());
            ui->debugwordlbl->setText(result.current_word.c_str());

            debugPanelHighlightEnabled(true);
        }
        catch (const char *message)
        {
            ui->logwindow->appendPlainText(message);
        }
        catch (std::string message)
        {
            ui->logwindow->appendPlainText(message.c_str());
        }
        catch (...)
        {
            ui->logwindow->appendPlainText("Something went wrong.");
            ui->logwindow->appendPlainText("Please tell about this to the developer.");
        }

        ui->debugbtn->setEnabled(true);
    }
}

void MainWindow::on_debugnextbtn_clicked()
{
    DEBUGMIDDLEWARE

    if (debugMode)
    {
        ui->debugnextbtn->setEnabled(false);

        machine::MachineState result;
        try
        {
            _debugger.setLambda(ui->lambdacheckBox->isChecked());
            result = _debugger.lazyDebug(true);
            currentLineHighlight(std::stoi(result.line));
            ui->debuglineEdit->setText(result.current_strip.c_str());
            ui->debugstatelbl->setText(result.current_state.c_str());
            ui->debugwordlbl->setText(result.current_word.c_str());
        }
        catch (const char *message)
        {
            ui->logwindow->appendPlainText(message);
            NORMALMIDDLEWARE
        }
        catch (std::string message)
        {
            ui->logwindow->appendPlainText(message.c_str());
            NORMALMIDDLEWARE
        }
        catch (...)
        {
            ui->logwindow->appendPlainText("Something went wrong.");
            ui->logwindow->appendPlainText("Please tell about this to the developer.");
            NORMALMIDDLEWARE
        }

        ui->debugnextbtn->setEnabled(true);
    }
}

void MainWindow::on_skipButton_clicked()
{
    DEBUGMIDDLEWARE

    if (debugMode)
    {
        ui->debugnextbtn->setEnabled(false);

        ui->logwindow->appendPlainText("Debugger continue.");

        machine::MachineState result;
        try
        {
            _debugger.setLambda(ui->lambdacheckBox->isChecked());
            result = _debugger.lazyDebug();
            currentLineHighlight(std::stoi(result.line));
            ui->debuglineEdit->setText(result.current_strip.c_str());
            ui->debugstatelbl->setText(result.current_state.c_str());
            ui->debugwordlbl->setText(result.current_word.c_str());
        }
        catch (const char *message)
        {
            ui->logwindow->appendPlainText(message);
            NORMALMIDDLEWARE
        }
        catch (std::string message)
        {
            ui->logwindow->appendPlainText(message.c_str());
            NORMALMIDDLEWARE
        }
        catch (...)
        {
            ui->logwindow->appendPlainText("Something went wrong.");
            ui->logwindow->appendPlainText("Please tell about this to the developer.");
            NORMALMIDDLEWARE
        }

        ui->debugnextbtn->setEnabled(true);
    }
}
