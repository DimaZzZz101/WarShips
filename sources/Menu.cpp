// Copyright 2021 DimaZzZz101

#include "Menu.hpp"
#include <cstdlib> 	//Memory Allocation
#include <ncurses.h>	//Windows and Printing
#include <cstdarg>	//Variable Options (VA_LIST)
#include <cstring>	//String methods

/* Menu Class Constructor */
Menu::Menu(WINDOW *win) {
    this->addOpts = false;
    this->win = win;
    getmaxyx(this->win, this->h, this->w);
    optPtrUsed = false;
    argPtrUsed = false;
}

Menu::~Menu() {
    if(argPtrUsed)
        free(this->argPtr);
    if(optPtrUsed)
        free(this->optPtr);
}

/* Sets options for Menu */
void Menu::SetOpts(int numOpts, ...) {

    this->numArgs = numOpts;

    this->optPtr = (char**)calloc(this->numArgs, sizeof(char*) * this->numArgs);

    /* Save strings inputted to the proper place in the option array */
    va_list valList;
    va_start(valList, numOpts);
    for(int i = 0; i < this->numArgs; i++)
    {
        this->optPtr[i] = va_arg(valList, char*);
    }
    va_end(valList);
    optPtrUsed = true;
}

/* Sets additional options for Menu */
void Menu::AddStrings(int numOpts, ...) {
    this->addOpts = true;
    this->numOpts = numOpts;
    this->argPtr = (char**)calloc(this->numArgs, sizeof(char*) * this->numArgs);

    va_list valList;
    va_start(valList, numOpts);
    for(int i = 0; i < this->numOpts; i++)
        this->argPtr[i] = va_arg(valList, char*);
    va_end(valList);
    argPtrUsed = true;
}

/* Runs Menu after input from setOpts method */
int Menu::RunMenu() {
    if(this->addOpts)
    {
        for(int i = 0; i < this->numOpts; i++)
        {
            mvwaddstr(this->win, 1+i, this->w /2 - strlen(this->argPtr[i]) /2, this->argPtr[i]);
        }
    }

    /* Set option to be first */
    this->option = 0;
    /* Run Menu */
    while(true)
    {
        /* Print Menu, REVERSE colors if is selected */
        for(int i = 0; i < this->numArgs; i++)
        {
            if(i == this->option)
            {
                attron(A_REVERSE);
                mvwaddstr(this->win, this->h /2 + i, this->w/2 - strlen(this->optPtr[i])/2, this->optPtr[i]);
                attroff(A_REVERSE);
            }else
                mvwaddstr(this->win, this->h/2 + i, this->w/2 - strlen(this->optPtr[i])/2, this->optPtr[i]);
        }
        /* Get Input */
        int ch = getch();
        if (ch == 'q') //Quit if q is pressed
            break;
        if (ch == KEY_UP) // Move selection up one if KEY_UP pressed
        {
            this->option--;
            if(this->option < 0)
                this->option = this->numArgs - 1;
        }
        if (ch == KEY_DOWN) // Move selection down one if KEY_DOWN pressed
        {
            this->option++;
            if(this->option >= this->numArgs)
                this->option = 0;
        }
        if (ch == '\n')	// Exit and return selection if enter key is pressed
            return this->option;
    }

    // Return error if q is pressed
    return -1;
}
