// Copyright 2021 DimaZzZz101

#ifndef WARSHIPS_PLACINGSHIPS_HPP
#define WARSHIPS_PLACINGSHIPS_HPP

#include <ncurses.h>
#include <cstdlib>
#include <array>

#include "GameVisualElements.hpp"

class PlacingShips {
public:
    PlacingShips();

    ~PlacingShips();

    void Display() const;

    bool Location();

    std::array<std::array<char, 10>, 10> GetField() const;

private:
    bool CheckShip(size_t ship_size, bool pos);

    void MoveUp();

    void MoveDown();

    void MoveLeft();

    void MoveRight();

    void CreateShip(size_t ship_size, bool pos);

    // Поля и данные
    const int width_ = 42;
    const int height_ = 23;

    const int window_x = COLS / 2 - width_ / 2;
    const int window_y = LINES / 2 - height_ / 2;

    WINDOW *interface;

    std::array<std::array<char, 10>, 10> field_;

    size_t ships_[10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};

    int curr_x_ = 0;
    int curr_y_ = 0;
};

#endif //WARSHIPS_PLACINGSHIPS_HPP
