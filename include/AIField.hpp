// Copyright 2021 DimaZzZz101

#ifndef WARSHIPS_AIFIELD_HPP
#define WARSHIPS_AIFIELD_HPP

#include <ncurses.h>
#include <cstdlib>
#include <array>
#include <string>

#include "../include/GameVisualElements.hpp"

class AIField {
public:
    // Конструктор для скрытого и публичного полей ИИ
    explicit AIField(std::array<std::array<char, 10>, 10> &ai_field);

    ~AIField();
    // Отображение поля ИИ и действий на нем
    void Display() const;

    // Управление на поле ИИ
    void Interface();

    // Проверка
    bool CanPlace() const;

private:
    void MoveUp();

    void MoveDown();

    void MoveLeft();

    void MoveRight();

    // Проверка на возможность хода
    bool MoveAbility(int x, int y);

    // Обработка хода игрока на поле ИИ
    bool PlayerMove(int x, int y);

    // Проверка на уничтоженный корабль
    bool IsKilled(int x, int y, const std::string &direction = "");

    // Автообстрел
    void AutoShoot();

    // Высота и ширина поля
    const int height_ = 23;
    const int width_ = 42;

    // координаты смещения поля ИИ
    const int x_ = 3;
    const int y_ = 50;

    WINDOW *field_;

    std::array<std::array<char, 10>, 10> private_ai_field_; // Видимое поле ИИ
    std::array<std::array<char, 10>, 10> public_ai_field_; // Скрытое по ИИ

    // Задание начальных координат
    int curr_x_ = 0;
    int curr_y_ = 0;
};


#endif //WARSHIPS_AIFIELD_HPP
