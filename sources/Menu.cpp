// Copyright 2021 DimaZzZz101

#include "../include/Menu.hpp"

#include <cstdlib>
#include <ncurses.h>
#include <cstdarg>
#include <cstring>

/* Конструктор для меню */
Menu::Menu(WINDOW *win) {
    this->addOpts = false;
    this->win = win;

    getmaxyx(this->win, this->h, this->w);

    optPtrUsed = false;
    argPtrUsed = false;
}

/* Деструктор для меню */
Menu::~Menu() {
    if (argPtrUsed) {
        free(this->argPtr);
    }

    if (optPtrUsed) {
        free(this->optPtr);
    }
}

/* Установка опций в меню */
void Menu::SetOpts(int numOpts, ...) {
    this->numArgs = numOpts;
    this->optPtr = (char **) calloc(this->numArgs, sizeof(char *) * this->numArgs);

    va_list valList;
    va_start(valList, numOpts);

    /* Сохранение опций в массив */
    for (int i = 0; i < this->numArgs; ++i) {
        this->optPtr[i] = va_arg(valList, char * );
    }

    va_end(valList);
    optPtrUsed = true;
}

/* Установка дополнительной надстройки для меню (название/описание) */
void Menu::AddStrings(int numOpts, ...) {
    this->addOpts = true;
    this->numOpts = numOpts;
    this->argPtr = (char **) calloc(this->numArgs, sizeof(char *) * this->numArgs);

    va_list valList;
    va_start(valList, numOpts);

    /* Сохранение надстроек в массив */
    for (int i = 0; i < this->numOpts; ++i) {
        this->argPtr[i] = va_arg(valList, char * );
    }

    va_end(valList);
    argPtrUsed = true;
}

/* Запуск меню */
int Menu::RunMenu() {
    if (this->addOpts) {
        for (int i = 0; i < this->numOpts; ++i) {
            mvwaddstr(this->win,
                      1 + i,
                      this->w / 2 - strlen(this->argPtr[i]) / 2,
                      this->argPtr[i]);
        }
    }

    /* Устанавливаем, чтобы опции были в приоритете */
    /* Необходимо для закольцовывания списка опций, т.к. является индикатором */

    this->option = 0;

    /* Работа меню */
    while (true) {
        /* Вывод меню на экран, чтобы было видно что выбирает пользователь, используется REVERSE */
        for (int i = 0; i < this->numArgs; ++i) {
            if (i == this->option) {
                attron(A_REVERSE);
                mvwaddstr(this->win,
                          this->h / 2 + i,
                          this->w / 2 - strlen(this->optPtr[i]) / 2, this->optPtr[i]);
                attroff(A_REVERSE);
            } else
                mvwaddstr(this->win,
                          this->h / 2 + i,
                          this->w / 2 - strlen(this->optPtr[i]) / 2,
                          this->optPtr[i]);
        }

        /* Считываем состояние опций меню */
        int ch = getch();

        // Если нажата q то произойдет выход
        if (ch == 'q') break;

        // Для перемещения выбора вверх нужно нажимать стрелку вверх
        if (ch == KEY_UP) {
            this->option--;
            if (this->option < 0) {
                this->option = this->numArgs - 1;
            }
        }

        // Для перемещения выбора вверх нужно нажимать стрелку вверх
        if (ch == KEY_DOWN) {
            this->option++;
            if (this->option >= this->numArgs) {
                this->option = 0;
            }
        }

        // Enter при нажатии сделает выбор
        if (ch == '\n') {
            return this->option;
        }
    }

    // Если была нажата q, то произойдет выход
    return -1;
}
