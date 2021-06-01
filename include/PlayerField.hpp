// Copyright 2021 DimaZzZz2021

#ifndef WARSHIPS_PLAYERFIELD_HPP
#define WARSHIPS_PLAYERFIELD_HPP

#include <ncurses.h>
#include <cstdlib>
#include <array>
#include <string>

#include "GameVisualElements.hpp"

class PlayerField {
public:
    explicit PlayerField(std::array<std::array<char, 10>, 10> player_field);
    PlayerField(std::array<std::array<char, 10>, 10>, int x, int y);

    ~PlayerField();

    void Display() const;

    void Move(std::array<std::array<int, 10>, 10> &cells, int x, int y);

    bool Damage(int x, int y);

    bool CanPlace() const;

private:
    bool IsKilled(std::array<std::array<int, 10>, 10> &cells, int x, int y, const std::string &direction = "");

    void AutoShoot(std::array<std::array<int, 10>, 10> &cells, int x, int y);

    const int height_ = 23;
    const int width_ = 42;

    const int x_ = 3;
    const int y_ = 3;


    WINDOW *field_;

    std::array<std::array<char, 10>, 10> player_field_;
};

#endif //WARSHIPS_PLAYERFIELD_HPP
