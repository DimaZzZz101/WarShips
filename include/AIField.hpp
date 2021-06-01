// Copyright 2021 DimaZzZz101

#ifndef WARSHIPS_AIFIELD_HPP
#define WARSHIPS_AIFIELD_HPP

#include <ncurses.h>
#include <cstdlib>
#include <array>
#include <string>

#include "GameVisualElements.hpp"

class AIField {
public:
    explicit AIField(std::array<std::array<char, 10>, 10> &ai_field);

    ~AIField();

    void Display() const;

    void Interface();

    bool CanPlace() const;

private:
    void MoveUp();

    void MoveDown();

    void MoveLeft();

    void MoveRight();

    bool MoveAbility(int x, int y);

    bool PlayerMove(int x, int y);

    bool IsKilled(int x, int y, const std::string &direction = "");

    void AutoShoot();

    const int height_ = 23;
    const int width_ = 42;
    const int x_ = 3;
    const int y_ = 50;
    WINDOW *field_;
    std::array<std::array<char, 10>, 10> private_ai_field_;
    std::array<std::array<char, 10>, 10> public_ai_field_;
    int curr_x_ = 0;
    int curr_y_ = 0;
};


#endif //WARSHIPS_AIFIELD_HPP
