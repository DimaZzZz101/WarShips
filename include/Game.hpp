// Copyright 2021 DimaZzZz101

#ifndef WARSHIPS_GAME_HPP
#define WARSHIPS_GAME_HPP

#include <iostream>
#include <ncurses.h>
#include <ctime>
#include <unistd.h>
#include <string>


#include "AI.hpp"
#include "AIField.hpp"
#include "PlayerField.hpp"
#include "PlacingShips.hpp"
#include "Menu.hpp"

void AIvsAI();

void AIvsHuman();

void Help();

void Exit();

void GameOver1(int value);

void GameOver2(int value);

void Game() {
    /* <----------Menu_begin----------> */
    initscr();
    noecho();
    keypad(stdscr, true);

    Menu menu = Menu(stdscr);
    menu.SetOpts(4, "AI vs AI", "AI vs Human", "Instructions", "Exit");
    menu.AddStrings(27, "==============================================\n",
                    "||                                           ||\n",
                    "||        XX   XX XXXXX XX  XX XX  XX        ||\n",
                    "||        XXX XXX XX    XXX XX XX  XX        ||\n",
                    "||        XX X XX XXXX  XX XXX XX  XX        ||\n",
                    "||        XX   XX XX    XX  XX XX  XX        ||\n",
                    "||        XX   XX XXXXX XX  XX  XXXX         ||\n",
                    "||                                           ||\n",
                    "||             WarShips The Game             ||\n",
                    "||         by  Dmitry Zabotin IU8-21         ||\n",
                    "||===========================================||\n",
                    "||         *      |---||---|          *      ||\n",
                    "||     *              ||                     ||\n",
                    "||            *      _||_         *    *     ||\n",
                    "||     *           /  ||  \\                  ||\n",
                    "||            \\\\__|__-||-__|__//             ||\n",
                    "||              \\\\\\___||___///               ||\n",
                    "||~~~~~~~~~~~~~~~~~\\\\_||_//~~~~~~~~~~~~~~~~~~||\n",
                    "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n",
                    "||===========================================||\n",
                    "||                                           ||\n",
                    "||                                           ||\n",
                    "||                                           ||\n",
                    "||                                           ||\n",
                    "||                                           ||\n",
                    "||                                           ||\n",
                    "===============================================\n");

    refresh();

    int choice = menu.RunMenu();
    // Выбор опции игры
    switch (choice) {
        case 0:
            AIvsAI(); // ИИ против ИИ
            break;
        case 1:
            AIvsHuman(); // ИИ против игрока
            break;
        case 2:
            Help(); // Показ правил и элементов игры + управление
            break;
        case 3:
            Exit(); // Выход из игры
            break;
    }

    endwin();
    /* <----------Menu_end----------> */
}

void AIvsAI() {
    clear(); // Очистка консольного окна

    srand(time(nullptr)); // Устанавливается отсчет ГПСЧ

    // Выполнение проверки на корректность открытия окна
    if (!initscr()) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(1);
    }

    initscr(); // Инициализация окна (начало оконного блока)

    AI ai_player1; // Игрок ИИ-1
    AI ai_player2; // Игрок ИИ-2

    ai_player1.ShipsLocation(); // ИИ-1 расставляет корабли на своем поле
    ai_player2.ShipsLocation(); // ИИ-2 расставляет корабли на своем поле

    PlayerField Pl_win1(ai_player1.GetAiField()); // Создание поля ИИ-1
    PlayerField Pl_win2(ai_player2.GetAiField(), 3, 50); // Создание поля ИИ-2

    Pl_win1.Display(); // Вывод на экран поля первого игрока (левое поле)
    Pl_win2.Display(); // Вывод на экран поля второго игрока (правое поле)

    // Процесс игры
    // Первым ходит ИИ-1
    while (Pl_win1.CanPlace() && Pl_win2.CanPlace()) {
        while (true) {
            std::pair<size_t, size_t> point = ai_player1.Shot(); // ИИ-1 производит выстрел

            usleep(100000); // Задержка в 1 секунду, чтобы показать процесс игры

            Pl_win2.Move(ai_player1.GetCells(), point.first, point.second); // Отображение хода ИИ-1 на поле ИИ-2

            // Если ИИ-1 не попал, то ходит ИИ-2
            if (!Pl_win2.Damage(point.first, point.second)) {
                break;
            }
        }

        // Аналогичные действия для ИИ-2
        while (true) {
            std::pair<size_t, size_t> point = ai_player2.Shot();

            usleep(100000);

            Pl_win1.Move(ai_player2.GetCells(), point.first, point.second);

            if (!Pl_win1.Damage(point.first, point.second)) {
                break;
            }
        }

        // Проверка, кто победил
        if (!Pl_win2.CanPlace()) {
            int value = 0;
            GameOver1(value);
        } else if (!Pl_win1.CanPlace()) {
            int value = 1;
            GameOver1(value);
        }
    }

    endwin(); // Конец оконного блока
}

void AIvsHuman() {
    clear(); // Очистка консольного окна

    srand(time(nullptr)); // Устанавливается отсчет ГПСЧ

    // Выполнение проверки на корректность открытия окна
    if (!initscr()) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(1);
    }

    initscr(); // Инициализация окна (начало оконного блока)

    noecho(); // Отключение вывода вводимых с клавиатуры команд
    keypad(stdscr, true); // Подключение спец-символов

    PlacingShips human_player; // Игрок-1 (человек)
    AI ai_player; // Игрок ИИ

    // Перед игрой человек заполняет свое поле сам
    if (human_player.Location()) {
        ai_player.ShipsLocation(); // ИИ заполняет свое поле

        PlayerField human_win(human_player.GetField()); // Создание поля игрока
        AIField ai_win(ai_player.GetAiField()); // Создание поля ИИ

        human_win.Display(); // Вывод поля игрока на экран
        ai_win.Display(); // Вывод поля ИИ на экран

        // Пока есть живые корабли продолжается игра
        do {
            ai_win.Interface(); // Ход игрока (пока не промажет)

            // Действия ИИ полностью аналогичны действиям в опции ИИ против ИИ
            while (true) {
                std::pair<size_t, size_t> point = ai_player.Shot();

                usleep(1000000);

                human_win.Move(ai_player.GetCells(), point.first, point.second);

                if (!human_win.Damage(point.first, point.second)) {
                    break;
                }
            }

        } while (human_win.CanPlace() && ai_win.CanPlace());

        // Определение победителя
        if (!human_win.CanPlace()) {
            int value = 0;
            GameOver2(value);
        } else if (!ai_win.CanPlace()) {
            int value = 1;
            GameOver2(value);
        }
    }

    endwin(); // Конец оконного блока

}

// Справка по игре
void Help() {
    clear();

    initscr();

    int height = 21;
    int width = 87;
    int x = 30;
    int y = 10;

    WINDOW *help_win = newwin(height, width, y, x);
    refresh();
    box(help_win, 0, 0);

    mvwprintw(help_win, 1, 1, "--------------------------------------- Rules ---------------------------------------");
    mvwprintw(help_win, 2, 1, "1) The player has 4 types of ships:                                                 ");
    mvwprintw(help_win, 3, 1, "   single-deck, double-deck, three-deck and four-deck:                               ");
    mvwprintw(help_win, 4, 1, "   @, @@, @@@, @@@@ - designation of the ships                                       ");
    mvwprintw(help_win, 5, 1, "2) The distance between the ships must be at least 1 cell, including diagonally     ");
    mvwprintw(help_win, 6, 1, "3) You can move ships using the keys: w - up, a - left, s - down, d - right         ");
    mvwprintw(help_win, 7, 1, "4) To change the orientation of the ship, you need to press R                       ");
    mvwprintw(help_win, 8, 1, "5) To put the ship on the field, you need to press F                                ");
    mvwprintw(help_win, 9, 1, "6) After the player has placed his ships, two fields will appear in front of him:   ");
    mvwprintw(help_win, 10, 1, "  the left one -his, right one - the enemy's field                                  ");
    mvwprintw(help_win, 11, 1, "7) To fire a shot at the opponent's field, the player moves the cursor with wasd   ");
    mvwprintw(help_win, 12, 1, "   and fires a shot using the R key                                                 ");
    mvwprintw(help_win, 13, 1, "8) ! The player's task is to sink all enemy ships first !                          ");
    mvwprintw(help_win, 14, 1, "-------------------------------------- Symbols --------------------------------------");
    mvwprintw(help_win, 15, 1, "1) '~'  - water, free space,                                                       ");
    mvwprintw(help_win, 16, 1, "2) '@'  - ship (part of a ship),                                                   ");
    mvwprintw(help_win, 17, 1, "3) 'O'  - miss,                                                                    ");
    mvwprintw(help_win, 18, 1, "4) '*'  - occupied space, in these places you cannot put,                          ");
    mvwprintw(help_win, 19, 1, "5) 'X'  - shot at the ship (on the part of the ship)                               ");

    wrefresh(help_win);

    while (true) {
        int ch = getch();
        if (ch == 'q') //Quit if q is pressed
            break;
    }

    endwin();

    clear();

    Game();

}

void Exit() {}

// Конец игры для режима ИИ против ИИ
void GameOver1(int value) {
    if (value == 0) {
        clear();

        initscr();

        int height = 17;
        int width = 74;
        int x = 45;
        int y = 10;

        WINDOW *help_win = newwin(height, width, y, x);
        refresh();
        box(help_win, 0, 0);


        mvwprintw(help_win, 1, 1, "   d888b   .d8b.  .88b  d88. d88888b  .d88b.  db    db d88888b d8888b.  ");
        mvwprintw(help_win, 2, 1, "  88' Y8b d8' `8b 88'YbdP`88 88'     .8P  Y8. 88    88 88'     88  `8D  ");
        mvwprintw(help_win, 3, 1, "  88      88ooo88 88  88  88 88ooooo 88    88 Y8    8P 88ooooo 88oobY'  ");
        mvwprintw(help_win, 4, 1, "  88  ooo 88~~~88 88  88  88 88~~~~~ 88    88 `8b  d8' 88~~~~~ 88`8b    ");
        mvwprintw(help_win, 5, 1, "  88. ~8~ 88   88 88  88  88 88.     `8b  d8'  `8bd8'  88.     88 `88.  ");
        mvwprintw(help_win, 6, 1, "   Y888P  YP   YP YP  YP  YP Y88888P  `Y88P'     YP    Y88888P 88   YD  ");
        mvwprintw(help_win, 7, 1, "========================================================================");
        mvwprintw(help_win, 8, 1, "                                                                        ");
        mvwprintw(help_win, 9, 1, "     .d8b.  d888888b   db     db   d8b   db d888888b d8b   db .d8888.   ");
        mvwprintw(help_win, 10, 1, "   d8' `8b   `88'    o88     88   I8I   88   `88'   888o  88 88'  YP   ");
        mvwprintw(help_win, 11, 1, "   88ooo88    88      88     88   I8I   88    88    88V8o 88 `8bo.     ");
        mvwprintw(help_win, 12, 1, "   88~~~88    88      88     Y8   I8I   88    88    88 V8o88   `Y8b.   ");
        mvwprintw(help_win, 13, 1, "   88   88   .88.     88     `8b d8'8b d8'   .88.   88  V888 db   8D   ");
        mvwprintw(help_win, 14, 1, "   YP   YP Y888888P   VP      `8b8' `8d8'  Y888888P VP   V8P `8888Y'   ");
        mvwprintw(help_win, 15, 1, "=======================================================================");

        wrefresh(help_win);

        while (true) {
            int ch = getch();
            if (ch == 'q') //Quit if q is pressed
                break;
        }

        endwin();

        clear();

        Game();
    } else if (value == 1) {
        clear();

        initscr();

        int height = 17;
        int width = 75;
        int x = 30;
        int y = 10;

        WINDOW *help_win = newwin(height, width, y, x);
        refresh();
        box(help_win, 0, 0);


        mvwprintw(help_win, 1, 1, "   d888b   .d8b.  .88b  d88. d88888b  .d88b.  db    db d88888b d8888b.   ");
        mvwprintw(help_win, 2, 1, "  88' Y8b d8' `8b 88'YbdP`88 88'     .8P  Y8. 88    88 88'     88  `8D   ");
        mvwprintw(help_win, 3, 1, "  88      88ooo88 88  88  88 88ooooo 88    88 Y8    8P 88ooooo 88oobY'   ");
        mvwprintw(help_win, 4, 1, "  88  ooo 88~~~88 88  88  88 88~~~~~ 88    88 `8b  d8' 88~~~~~ 88`8b     ");
        mvwprintw(help_win, 5, 1, "  88. ~8~ 88   88 88  88  88 88.     `8b  d8'  `8bd8'  88.     88 `88.   ");
        mvwprintw(help_win, 6, 1, "   Y888P  YP   YP YP  YP  YP Y88888P  `Y88P'     YP    Y88888P 88   YD   ");
        mvwprintw(help_win, 7, 1, "=========================================================================");
        mvwprintw(help_win, 8, 1, "                                                                         ");
        mvwprintw(help_win, 9, 1, "   .d8b.  d888888b .d888b.      db   d8b   db d888888b d8b   db .d8888.  ");
        mvwprintw(help_win, 10, 1, " d8' `8b   `88'   VP  `8D      88   I8I   88   `88'   888o  88 88'  YP  ");
        mvwprintw(help_win, 11, 1, " 88ooo88    88       odD'      88   I8I   88    88    88V8o 88 `8bo.    ");
        mvwprintw(help_win, 12, 1, " 88~~~88    88     .88'        Y8   I8I   88    88    88 V8o88   `Y8b.  ");
        mvwprintw(help_win, 13, 1, " 88   88   .88.   j88.         `8b d8'8b d8'   .88.   88  V888 db   8D  ");
        mvwprintw(help_win, 14, 1, " YP   YP Y888888P 888888D       `8b8' `8d8'  Y888888P VP   V8P `8888Y'  ");
        mvwprintw(help_win, 15, 1, "========================================================================");

        wrefresh(help_win);

        while (true) {
            int ch = getch();
            if (ch == 'q') //Quit if q is pressed
                break;
        }

        endwin();

        clear();

        Game();
    }
}

// Конец игры для режима игрок против ИИ
void GameOver2(int value) {
    if (value == 0) {
        clear();

        initscr();

        int height = 17;
        int width = 72;
        int x = 45;
        int y = 10;

        WINDOW *GameOverWin = newwin(height, width, y, x);
        refresh();
        box(GameOverWin, 0, 0);


        mvwprintw(GameOverWin, 1, 1, "  d888b   .d8b.  .88b  d88. d88888b  .d88b.  db    db d88888b d8888b. ");
        mvwprintw(GameOverWin, 2, 1, " 88' Y8b d8' `8b 88'YbdP`88 88'     .8P  Y8. 88    88 88'     88  `8D ");
        mvwprintw(GameOverWin, 3, 1, " 88      88ooo88 88  88  88 88ooooo 88    88 Y8    8P 88ooooo 88oobY' ");
        mvwprintw(GameOverWin, 4, 1, " 88  ooo 88~~~88 88  88  88 88~~~~~ 88    88 `8b  d8' 88~~~~~ 88`8b   ");
        mvwprintw(GameOverWin, 5, 1, " 88. ~8~ 88   88 88  88  88 88.     `8b  d8'  `8bd8'  88.     88 `88. ");
        mvwprintw(GameOverWin, 6, 1, "  Y888P  YP   YP YP  YP  YP Y88888P  `Y88P'     YP    Y88888P 88   YD ");
        mvwprintw(GameOverWin, 7, 1, "======================================================================");
        mvwprintw(GameOverWin, 8, 1, "                                                                      ");
        mvwprintw(GameOverWin, 9, 1, "       .d8b.  d888888b      db   d8b   db d888888b d8b   db .d8888.    ");
        mvwprintw(GameOverWin, 10, 1, "     d8' `8b   `88'        88   I8I   88   `88'   888o  88 88'  YP    ");
        mvwprintw(GameOverWin, 11, 1, "     88ooo88    88         88   I8I   88    88    88V8o 88 `8bo.      ");
        mvwprintw(GameOverWin, 12, 1, "     88~~~88    88         Y8   I8I   88    88    88 V8o88   `Y8b.    ");
        mvwprintw(GameOverWin, 13, 1, "     88   88   .88.        `8b d8'8b d8'   .88.   88  V888 db   8D    ");
        mvwprintw(GameOverWin, 14, 1, "     YP   YP Y888888P       `8b8' `8d8'  Y888888P VP   V8P `8888Y'    ");
        mvwprintw(GameOverWin, 15, 1, "======================================================================");

        wrefresh(GameOverWin);

        while (true) {
            int ch = getch();
            if (ch == 'q') //Quit if q is pressed
                break;
        }

        endwin();

        clear();

        Game();

        } else if (value == 1) {
        clear();

        initscr();

        int height = 17;
        int width = 98;
        int x = 30;
        int y = 10;

        WINDOW *GameOverWin = newwin(height, width, y, x);
        refresh();
        box(GameOverWin, 0, 0);
        
        mvwprintw(GameOverWin, 1, 1, "               d888b   .d8b.  .88b  d88. d88888b  .d88b.  db    db d88888b d8888b.              ");
        mvwprintw(GameOverWin, 2, 1, "              88' Y8b d8' `8b 88'YbdP`88 88'     .8P  Y8. 88    88 88'     88  `8D              ");
        mvwprintw(GameOverWin, 3, 1, "              88      88ooo88 88  88  88 88ooooo 88    88 Y8    8P 88ooooo 88oobY'              ");
        mvwprintw(GameOverWin, 4, 1, "              88  ooo 88~~~88 88  88  88 88~~~~~ 88    88 `8b  d8' 88~~~~~ 88`8b                ");
        mvwprintw(GameOverWin, 5, 1, "              88. ~8~ 88   88 88  88  88 88.     `8b  d8'  `8bd8'  88.     88 `88.              ");
        mvwprintw(GameOverWin, 6, 1, "               Y888P  YP   YP YP  YP  YP Y88888P  `Y88P'     YP    Y88888P 88   YD              ");
        mvwprintw(GameOverWin, 7, 1, "================================================================================================");
        mvwprintw(GameOverWin, 8, 1, "                                                                                                ");
        mvwprintw(GameOverWin, 9, 1, "  d8888b. db       .d8b.  db    db d88888b d8888b.      db   d8b   db d888888b d8b   db .d8888.  ");
        mvwprintw(GameOverWin, 10, 1, " 88  `8D 88      d8' `8b `8b  d8' 88'     88  `8D      88   I8I   88   `88'   888o  88 88'  YP  ");
        mvwprintw(GameOverWin, 11, 1, " 88oodD' 88      88ooo88  `8bd8'  88ooooo 88oobY'      88   I8I   88    88    88V8o 88 `8bo.    ");
        mvwprintw(GameOverWin, 12, 1, " 88~~~   88      88~~~88    88    88~~~~~ 88`8b        Y8   I8I   88    88    88 V8o88   `Y8b.  ");
        mvwprintw(GameOverWin, 13, 1, " 88      88booo. 88   88    88    88.     88 `88.      `8b d8'8b d8'   .88.   88  V888 db   8D  ");
        mvwprintw(GameOverWin, 14, 1, " 88      Y88888P YP   YP    YP    Y88888P 88   YD       `8b8' `8d8'  Y888888P VP   V8P `8888Y'  ");
        mvwprintw(GameOverWin, 15, 1, "================================================================================================");

        wrefresh(GameOverWin);

        while (true) {
            int ch = getch();
            if (ch == 'q') //Quit if q is pressed
                break;
        }

        endwin();

        clear();

        Game();
    }
}

#endif //WARSHIPS_GAME_HPP
