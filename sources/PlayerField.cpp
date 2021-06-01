// Copyright 2021 DimaZzZz101

#include "../include/PlayerField.hpp"

PlayerField::PlayerField(std::array<std::array<char, 10>, 10> player_field) :
        field_(newwin(height_, width_, x_, y_)), player_field_(player_field) {
    for (size_t i = 0; i < 10; ++i) {
        for (size_t j = 0; j < 10; ++j) {
            if (player_field_[i][j] == elements_of_interface::empty) {
                player_field_[i][j] = elements_of_interface::sea;
            }
        }
    }
}

PlayerField::PlayerField(std::array<std::array<char, 10>, 10> player_field, int x, int y) :
    x_(x), y_(y), field_(newwin(height_, width_, x_, y_)), player_field_(player_field) {
    for (size_t i = 0; i < 10; ++i) {
        for (size_t j = 0; j < 10; ++j) {
            if (player_field_[i][j] == elements_of_interface::empty) {
                player_field_[i][j] = elements_of_interface::sea;
            }
        }
    }
}



PlayerField::~PlayerField() { delwin(field_); }

void PlayerField::Display() const {
    refresh();

    box(field_, 0, 0);

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (player_field_[i][j] == elements_of_interface::damage) {
                start_color();
                init_pair(1, COLOR_RED, COLOR_BLACK);
                wattron(field_, COLOR_PAIR(1));
                mvwprintw(field_, 2 * i + 2, 4 * j + 2, "%c", player_field_[i][j]);
                wattroff(field_, COLOR_PAIR(1));
                use_default_colors();

            } else if (player_field_[i][j] == elements_of_interface::sea) {
                start_color();
                init_pair(2, COLOR_BLUE, COLOR_BLACK);
                wattron(field_, COLOR_PAIR(2));
                mvwprintw(field_, 2 * i + 2, 4 * j + 2, "%c", player_field_[i][j]);
                wattroff(field_, COLOR_PAIR(2));
                use_default_colors();

            } else if (player_field_[i][j] == elements_of_interface::ship) {
                start_color();
                init_pair(3, COLOR_GREEN, COLOR_BLACK);
                wattron(field_, COLOR_PAIR(3));
                mvwprintw(field_, 2 * i + 2, 4 * j + 2, "%c", player_field_[i][j]);
                wattroff(field_, COLOR_PAIR(3));
                use_default_colors();

            } else if (player_field_[i][j] == elements_of_interface::miss) {
                start_color();
                init_pair(4, COLOR_WHITE, COLOR_BLACK);
                wattron(field_, COLOR_PAIR(4));
                mvwprintw(field_, 2 * i + 2, 4 * j + 2, "%c", player_field_[i][j]);
                wattroff(field_, COLOR_PAIR(4));
                use_default_colors();
            }
        }
    }

    wrefresh(field_);
}

void PlayerField::Move(std::array<std::array<int, 10>, 10> &cells, const int x, const int y) {
    if (player_field_[x][y] == elements_of_interface::ship) {
        player_field_[x][y] = elements_of_interface::damage;

        if (IsKilled(cells, x, y)) {
            cells[x][y] = 2;
            AutoShoot(cells, x, y);
            Display();
            return;
        }

        cells[x][y] = 1;
        Display();
        return;
    }

    player_field_[x][y] = elements_of_interface::miss;
    cells[x][y] = -1;
    Display();
}

bool PlayerField::Damage(int x, int y) {
    return player_field_[x][y] == elements_of_interface::damage;
}

bool PlayerField::CanPlace() const {
    for (size_t i = 0; i < 10; ++i) {
        for (size_t j = 0; j < 10; ++j) {
            if (player_field_[i][j] == elements_of_interface::ship) {
                return true;
            }
        }
    }

    return false;
}

bool PlayerField::IsKilled(std::array<std::array<int, 10>, 10> &cells,
                           const int x, const int y,
                           const std::string &direction) {
    try {
        if (player_field_.at(x + 1)[y] == elements_of_interface::ship) {
            return false;
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (player_field_.at(x - 1)[y] == elements_of_interface::ship) {
            return false;
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (player_field_[x].at(y + 1) == elements_of_interface::ship) {
            return false;
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (player_field_[x].at(y - 1) == elements_of_interface::ship) {
            return false;
        }
    }
    catch (std::out_of_range &err) {}


    if (direction.empty()) {
        try {
            if ( player_field_.at(x + 1)[y] == elements_of_interface::damage) {
                if (!IsKilled(cells, x + 1, y, "right")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

        try {
            if ( player_field_.at(x - 1)[y] == elements_of_interface::damage) {
                if (!IsKilled(cells, x - 1, y, "left")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

        try {
            if ( player_field_[x].at(y - 1) == elements_of_interface::damage) {
                if (!IsKilled(cells, x, y - 1, "up")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

        try {
            if ( player_field_[y].at(y + 1) == elements_of_interface::damage) {
                if (!IsKilled(cells, x, y + 1, "down")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

    } else if (direction == "right") {
        try {
            if (player_field_.at(x + 1)[y] == elements_of_interface::damage) {
                if (!IsKilled(cells, x + 1, y, "right")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

    } else if (direction == "left") {
        try {
            if (player_field_.at(x - 1)[y] == elements_of_interface::damage) {
                if (!IsKilled(cells, x - 1, y, "left")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

    } else if (direction == "up") {
        try {
            if (player_field_[x].at(y - 1) == elements_of_interface::damage) {
                if (!IsKilled(cells, x, y - 1, "up")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

    } else if (direction == "down") {
        try {
            if (player_field_[x].at(y + 1) == elements_of_interface::damage) {
                if (!IsKilled(cells, x, y + 1, "down")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}
    }

    return true;
}

void PlayerField::AutoShoot(std::array<std::array<int, 10>, 10> &cells,const int x, const int y) {
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            try {
                if (player_field_.at(x + i).at(y + j) == elements_of_interface::sea) {
                    player_field_[x + i][y + j] = elements_of_interface::miss;
                    cells[x + i][y + j] = -1;
                }
            }
            catch (std::out_of_range &err) {}
        }
    }

    try {
        if (cells.at(x + 1).at(y) == 1) {
            cells[x + 1][y] = 2;
            AutoShoot(cells, x + 1, y);
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (cells.at(x - 1).at(y) == 1) {
            cells[x - 1][y] = 2;
            AutoShoot(cells, x - 1, y);
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (cells.at(x).at(y + 1) == 1) {
            cells[x][y + 1] = 2;
            AutoShoot(cells, x, y + 1);
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (cells.at(x).at(y - 1) == 1) {
            cells[x][y - 1] = 2;
            AutoShoot(cells, x, y - 1);
        }
    }
    catch (std::out_of_range &err) {}
}
