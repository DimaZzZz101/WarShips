// Copyright 2021 DimaZzZz101

#ifndef WARSHIPS_AI_HPP
#define WARSHIPS_AI_HPP

#include <iostream>
#include <array>

#include "../include/GameVisualElements.hpp"

class AI {
public:
    AI();

    ~AI() = default;

    void ShipsLocation();

    std::pair<int, int> Shot();

    std::array<std::array<int, 10>, 10> &GetCells();

    std::array<std::array<char, 10>, 10> &GetAiField();

private:
    // Выбор координаты (по анализу ячеек)
    std::pair<int, int> ChoosingCell(int x, int y);

    // Проверка ячеек (анализ куда стрелять)
    int PosOfShot(int x, int y);

    // Проверка можно ли поставить корабль
    bool CheckShip(int curr_x_, int curr_y_, size_t ship_size, bool pos);

    // Метод создания корабля
    void CreateShip(int curr_x_, int curr_y_, size_t ship_size, bool pos);

    std::array<std::array<int, 10>, 10> cells_; // Поле с ячейками
    std::array<std::array<char, 10>, 10> ai_field_; // Поле ИИ

    size_t ships_[10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1}; // Массив кораблей
};


#endif //WARSHIPS_AI_HPP
