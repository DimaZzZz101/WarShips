// Copyright 2021 DimaZzZz101

#ifndef WARSHIPS_AI_HPP
#define WARSHIPS_AI_HPP

#include <iostream>
#include <array>

#include "GameVisualElements.hpp"

class AI {
public:
    AI();

    ~AI() = default;

    void ShipsLocation();

    std::pair<int, int> Shot();

    std::array<std::array<int, 10>, 10> &GetCells();

    std::array<std::array<char, 10>, 10> &GetAiField();

private:
    std::pair<int, int> ChoosingCell(int x, int y);

    int PosOfShot(int x, int y);

    bool CheckShip(int curr_x_, int curr_y_, size_t ship_size, bool pos);

    void CreateShip(int curr_x_, int curr_y_, size_t ship_size, bool pos);

    std::array<std::array<int, 10>, 10> cells_;
    std::array<std::array<char, 10>, 10> ai_field_;
    size_t ships_[10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
};


#endif //WARSHIPS_AI_HPP
