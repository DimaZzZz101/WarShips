// Copyright 2021 DimaZzZz101

#include "../include/PlacingShips.hpp"

PlacingShips::PlacingShips() : interface(newwin(height_, width_, window_y, window_x)) {
    for (auto &row: field_) {
        std::fill(row.begin(), row.end(), elements_of_interface::sea);
    }
}

PlacingShips::~PlacingShips() {
    delwin(interface);
}

void PlacingShips::Display() const {
    refresh();
    box(interface, 0, 0);
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (i == curr_x_ && j == curr_y_) {
                start_color();
                init_pair(20, COLOR_WHITE, COLOR_WHITE);
                wattron(interface, COLOR_PAIR(20));
                mvwprintw(interface, 2 * i + 2, 4 * j + 2, "%c", field_[i][j]);
                wattroff(interface, COLOR_PAIR(20));
                use_default_colors();
            } else if (field_[i][j] == elements_of_interface::ship) {
                start_color();
                init_pair(21, COLOR_GREEN, COLOR_BLACK);
                wattron(interface, COLOR_PAIR(21));
                mvwprintw(interface, 2 * i + 2, 4 * j + 2, "%c", field_[i][j]);
                wattroff(interface, COLOR_PAIR(21));
                use_default_colors();
            } else if (field_[i][j] == elements_of_interface::sea) {
                start_color();
                init_pair(22, COLOR_BLUE, COLOR_BLACK);
                wattron(interface, COLOR_PAIR(22));
                mvwprintw(interface, 2 * i + 2, 4 * j + 2, "%c", field_[i][j]);
                wattroff(interface, COLOR_PAIR(22));
                use_default_colors();
            }

            else {
                mvwprintw(interface, 2 * i + 2, 4 * j + 2, "%c", field_[i][j]);
            }
        }
    }

    wrefresh(interface);
}

bool PlacingShips::Location() {
    bool pos = true;
    size_t p = 0;
    while (p < 10) {
        Display();
        keypad(stdscr, true);

        int key = getch();
        switch (key) {
            case 119:
                MoveUp();
                break;
            case 115:
                MoveDown();
                break;
            case 100:
                MoveRight();
                break;
            case 97:
                MoveLeft();
                break;
            case 102:
                if (CheckShip(ships_[p], pos)) {
                    CreateShip(ships_[p], pos);
                    ++p;
                }
                break;
            case 114:
                pos = !pos;
                break;
            default:
                break;
        }
    }
    clear();

    return true;
}

std::array<std::array<char, 10>, 10> PlacingShips::GetField() const {
    return field_;
}

bool PlacingShips::CheckShip(size_t ship_size, bool pos) {
    if (field_[curr_x_][curr_y_] == elements_of_interface::ship ||
        field_[curr_x_][curr_y_] == elements_of_interface::empty) {
        return false;
    }

    if (!pos) {
        if (10 - curr_x_ >= ship_size) {
            for (size_t i = 0; i < ship_size; ++i) {
                if (field_[curr_x_ + i][curr_y_] == elements_of_interface::ship ||
                    field_[curr_x_ + i][curr_y_] == elements_of_interface::empty) {
                    return false;
                }
            }
            return true;
        }
    } else {
        if (10 - curr_y_ >= ship_size) {
            for (size_t i = 0; i < ship_size; ++i) {
                if (field_[curr_x_][curr_y_ + i] == elements_of_interface::ship ||
                    field_[curr_x_][curr_y_ + i] == elements_of_interface::empty) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

void PlacingShips::MoveUp() {
    if (curr_x_ != 0) --curr_x_;
}

void PlacingShips::MoveDown() {
    if (curr_x_ != 9) ++curr_x_;
}

void PlacingShips::MoveLeft() {
    if (curr_y_ != 0) --curr_y_;
}

void PlacingShips::MoveRight() {
    if (curr_y_ != 9) ++curr_y_;
}

void PlacingShips::CreateShip(size_t ship_size, bool direction) {
    // горизонтальное расположение
    if (direction) {
        for (size_t i = 0; i < ship_size; ++i) {
            field_[curr_x_][curr_y_ + i] = elements_of_interface::ship;
        }

        for (size_t i = 0; i < ship_size + 2; ++i) {
            try {
                field_.at(curr_x_ + 1).at(curr_y_ + i - 1) = elements_of_interface::empty;
            }
            catch (std::out_of_range &err) {}

            try {
                field_.at(curr_x_ - 1).at(curr_y_ + i - 1) = elements_of_interface::empty;
            }
            catch (std::out_of_range &err) {}
        }

        try {
            field_.at(curr_x_).at(curr_y_ - 1) = elements_of_interface::empty;
        }
        catch (std::out_of_range &err) {}

        try {
            field_.at(curr_x_).at(curr_y_ + ship_size) = elements_of_interface::empty;
        }
        catch (std::out_of_range &err) {}

        // вертикальное расположение
    } else {
        for (size_t i = 0; i < ship_size; ++i) {
            field_[curr_x_ + i][curr_y_] = elements_of_interface::ship;
        }

        for (size_t i = 0; i < ship_size + 2; ++i) {
            try {
                field_.at(curr_x_ + i - 1).at(curr_y_ + 1) = elements_of_interface::empty;
            }
            catch (std::out_of_range &err) {}

            try {
                field_.at(curr_x_ + i - 1).at(curr_y_ - 1) = elements_of_interface::empty;
            }
            catch (std::out_of_range &err) {}
        }

        try {
            field_.at(curr_x_ - 1).at(curr_y_) = elements_of_interface::empty;
        }
        catch (std::out_of_range &err) {}

        try {
            field_.at(curr_x_ + ship_size).at(curr_y_) = elements_of_interface::empty;
        }
        catch (std::out_of_range &err) {}
    }
}
