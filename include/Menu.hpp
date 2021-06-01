// Copyright 2021 DimaZzZz101

#ifndef WARSHIPS_MENU_HPP
#define WARSHIPS_MENU_HPP

#include <ncurses.h>

#include <string>
#include <vector>

class Menu {
public:
    Menu(WINDOW *win);
    ~Menu();
    void SetOpts(int numOpts, ... );
    void AddStrings(int numOpts, ...);
    int RunMenu();
private:
    char **optPtr, **argPtr;
    bool optPtrUsed, argPtrUsed;
    int numArgs, numOpts, option, h, w;
    bool addOpts;
    WINDOW *win;
};
#endif //WARSHIPS_MENU_HPP
