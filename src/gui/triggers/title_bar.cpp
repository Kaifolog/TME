#include "../mainwindow.h"

void MainWindow::on_action_triggered()
{
    QMessageBox::about(
        0, "Про мову цього емулятора",
        "Синтаксис:\n"
        "Має два заразервованих стани, 3 ключові вирази та один символ.\n"
        "В мові елементарною одиницею є рядок, який завжди має вид: \n\n statement1, word1->statement2, word2, "
        "direction\n\n"
        "В ідентифікаторах станів та алфавіту допустимі лише латинські букви будь-якого регістру, цифри та нижні "
        "підкреслення \"_\"\n"
        "Мова не має обмежень двжини ідентификаторів, рядків та не залежить від кількости чи наявності відступів, "
        "табуляцій, пропусків.\n\n\n"
        "section\n\nЯк і в мовах ассемблерів, мова данного емулятора розділена на секції \nsection .text; section "
        ".data;\nСекції для коду та вхідного рядка відповідно. Їх може бути декілька. З .data обирається остання, "
        ".text зливаються в одну.\n\n"
        "Макроси\n\nМакроси подібні макросам з мови С, #define st1 st2, де st1 заміниться на st2. Макроси "
        "підставляються по черзі з вернього вниз. Можуть маніпулювати всім у секції коду.\n\n"
        "section .data\n\nКожна клітинка стрічки має довільний розмір та відділяється від інших пропуском \" \", "
        "курсор вказується за допомогою обводки \"|\", наприклад:\"|lambda|\".\n\n"
        "Коментарі:\nЗа ассемблерною традицією комментарі мають лише одно-рядковий синтаксис та діють з \";\" ф до "
        "кінця рядка, у довільному рядку программи.\n\n"
        "Остаточно про виконання мови:\nВиконання завжди починається за стану \"start\" та слова, що було виділено "
        "курсором, кінець - стан \"end\", lambda - зарезервоване слово, що позначає пустий символ."
        "\n\nДетальніше на гітхаб Kaifolog/TME,\nтам є ціла нормальна інструкція!");
    QMessageBox msgBox;
    msgBox.setText(
        "<a href=\"https://github.com/Kaifolog/TME\" style=\"color:red\">Інструкція та актуальна версія</a>");
    msgBox.exec();
}
void MainWindow::on_action_2_triggered()
{
    QMessageBox::about(this, "Гарячі клавіші.",
                       "Синтаксис:\n"
                       "Ctrl+S - save\nCtrl+N - New\nCtrl+O - open\nCtrl+Shift+X - Quick Start\nCtrl+Tab - підстановка "
                       "розділових знаків до форми команди \",->,,\"\n F5 - Робить 1 ітерацію у debug режимі");
    QMessageBox msgBox;
    // msgBox.setText("<a href=\"https://github.com/Kaifolog/TME\" style=\"color:red\">Інструкція та актуальна
    // версія</a>"); msgBox.exec();
}

void MainWindow::on_action_3_triggered()
{
    QMessageBox::about(this, "Приклад програми.",
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
    msgBox.setText(
        "<a href=\"https://github.com/Kaifolog/TME\" style=\"color:red\">Інструкція та актуальна версія</a>");
    msgBox.exec();
}

void MainWindow::on_action_5_triggered()
{
    QMessageBox::about(this, "Про емулятор\n\n",
                       "Єдина неприємність - верхній рядок з графічного інтерфейсу є аналогом section .data, куди він "
                       "і збережується.\n"
                       "При відкритті ви побачите, що він в головному полі тексту. Можете сам рядок рядок без вказання "
                       "секції переписати в інпут рядок граф. інтерфейсу. Якщо він пустий - він не буде записаний.\n"
                       "Щоб дізнатися деталі чи зв'язатися з розробником - відкрийте вкладку \"Про программу\""
                       "Читайте інструкцію.");
}

void MainWindow::on_about_triggered()
{
    QMessageBox::about(this, "Про программу\n\n",
                       "Присвячено Україні та всім моїм друзям і подругам, Kanye \"Ye\" West'у.\n"
                       "Якщо мене ще не відрахували та мені ще цікаво - я буду продовжувати розробляти емулятор, маєш "
                       "якісь ідеї або хочеш допомогти? Хочеш отримати консольну версію?\n\n");
    QMessageBox msgBox;
    msgBox.setText("<a href=\"https://github.com/Kaifolog/TME\" style=\"color:red\">Мій Github</a>");
    msgBox.exec();
}

void MainWindow::on_settings_triggered()
{
    SettingsWindow settingswindow;
    settingswindow.exec();
}