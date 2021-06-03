// Copyright 2021 DimaZzZz2021

#ifndef WARSHIPS_PLAYERFIELD_HPP
#define WARSHIPS_PLAYERFIELD_HPP

#include <ncurses.h>
#include <cstdlib>
#include <array>
#include <string>

#include "../include/GameVisualElements.hpp"

class PlayerField {
public:
    // Конструктор поля игрока
    explicit PlayerField(std::array<std::array<char, 10>, 10> player_field);

    // Конструктор поля игрока со смещением
    PlayerField(std::array<std::array<char, 10>, 10>, int x, int y);

    // Деструктор
    ~PlayerField();

    // Метод для отображения выстрелов/промахов/кораблей на поле
    void Display() const;

    // Метод для отображения действий игрока на поле противника
    void Move(std::array<std::array<int, 10>, 10> &cells, int x, int y);

    // Выстрел по кораблю
    bool Damage(int x, int y);

    // Метод для проверки можно ли поставить корабль
    bool CanPlace() const;

private:
    // Проверка уничтожен ли корабль
    bool IsKilled(std::array<std::array<int, 10>, 10> &cells, int x, int y, const std::string &direction = "");

    // Автообстрел корабля
    void AutoShoot(std::array<std::array<int, 10>, 10> &cells, int x, int y);

    // Здание высоты и ширины поля
    const int height_ = 23;
    const int width_ = 42;

    // задание начального смещения поля относительно границ консольного окна
    const int x_ = 3;
    const int y_ = 3;

    // Объект окно
    WINDOW *field_;

    // Двумерный массив символов (поле)
    std::array<std::array<char, 10>, 10> player_field_;
};

#endif //WARSHIPS_PLAYERFIELD_HPP
