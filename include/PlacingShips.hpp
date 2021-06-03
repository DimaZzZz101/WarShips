// Copyright 2021 DimaZzZz101

#ifndef WARSHIPS_PLACINGSHIPS_HPP
#define WARSHIPS_PLACINGSHIPS_HPP

#include <ncurses.h>
#include <cstdlib>
#include <array>

#include "../include/GameVisualElements.hpp"

class PlacingShips {
public:
    PlacingShips();

    ~PlacingShips();

    // Отображение начального поля игрока, которое будет заполняться
    void Display() const;

    // Перемещение по полю при заполнении
    bool Location();

    std::array<std::array<char, 10>, 10> GetField() const;

private:
    // Проверка можно ли поставить корабль
    bool CheckShip(size_t ship_size, bool pos);

    void MoveUp();

    void MoveDown();

    void MoveLeft();

    void MoveRight();

    // Метод для создания корабля
    void CreateShip(size_t ship_size, bool pos);

    // Поля и данные
    const int width_ = 42;
    const int height_ = 23;

    // Установка размеров окна
    const int window_x = COLS / 2 - width_ / 2;
    const int window_y = LINES / 2 - height_ / 2;

    WINDOW *interface;

    // Двумерный массив (поле)
    std::array<std::array<char, 10>, 10> field_;

    // Массив кораблей
    size_t ships_[10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};

    // Задание начальных координат
    int curr_x_ = 0;
    int curr_y_ = 0;
};

#endif //WARSHIPS_PLACINGSHIPS_HPP
