// Copyright 2021 DimaZzZz101

#include "AIField.hpp"

AIField::AIField(std::array<std::array<char, 10>, 10> &ai_field) : field_(newwin(height_, width_, x_, y_)),
                                                                   private_ai_field_(ai_field) {
    for (size_t i = 0; i < 10; ++i) {
        for (size_t j = 0; j < 10; ++j) {
            if (private_ai_field_[i][j] == elements_of_interface::empty) {
                private_ai_field_[i][j] = elements_of_interface::sea;
            }
        }
    }

    for (size_t i = 0; i < 10; ++i) {
        for (size_t j = 0; j < 10; ++j) {
            public_ai_field_[i][j] = elements_of_interface::sea;
        }
    }
}

AIField::~AIField() {
    delwin(field_);
}

void AIField::Display() const {
    refresh();
    box(field_, 0, 0);
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (i == curr_x_ && j == curr_y_) {
                start_color();
                init_pair(10, COLOR_WHITE, COLOR_WHITE);
                wattron(field_, COLOR_PAIR(10));
                mvwprintw(field_, 2 * i + 2, 4 * j + 2, "%c", public_ai_field_[i][j]);
                wattroff(field_, COLOR_PAIR(10));
                use_default_colors();

            } else if (public_ai_field_[i][j] == elements_of_interface::damage) {
                start_color();
                init_pair(11, COLOR_RED, COLOR_BLACK);
                wattron(field_, COLOR_PAIR(11));
                mvwprintw(field_, 2 * i + 2, 4 * j + 2, "%c", public_ai_field_[i][j]);
                wattroff(field_, COLOR_PAIR(11));
                use_default_colors();

            } else if (public_ai_field_[i][j] == elements_of_interface::sea) {
                start_color();
                init_pair(12, COLOR_BLUE, COLOR_BLACK);
                wattron(field_, COLOR_PAIR(12));
                mvwprintw(field_, 2 * i + 2, 4 * j + 2, "%c", public_ai_field_[i][j]);
                wattroff(field_, COLOR_PAIR(12));
                use_default_colors();

            } else if (public_ai_field_[i][j] == elements_of_interface::miss) {
                start_color();
                init_pair(13, COLOR_WHITE, COLOR_BLACK);
                wattron(field_, COLOR_PAIR(13));
                mvwprintw(field_, 2 * i + 2, 4 * j + 2, "%c", public_ai_field_[i][j]);
                wattroff(field_, COLOR_PAIR(13));
                use_default_colors();

            } else {
                mvwprintw(field_, 2 * i + 2, 4 * j + 2, "%c", public_ai_field_[i][j]);
            }
        }
    }

    wrefresh(field_);
}

void AIField::Interface() {
    while (true) {
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
            case 114:
                if (MoveAbility(curr_x_, curr_y_)) {
                    if (PlayerMove(curr_x_, curr_y_)) {
                        if (IsKilled(curr_x_, curr_y_)) {
                            AutoShoot();
                            break;
                        }
                    } else {
                        Display();
                        return;
                    }
                }
                break;
        }
        Display();
    }
}

bool AIField::CanPlace() const {
    for (size_t i = 0; i < 10; ++i) {
        for (size_t j = 0; j < 10; ++j) {
            if (private_ai_field_[i][j] == elements_of_interface::ship) {
                return true;
            }
        }
    }

    return false;
}

// private methods
void AIField::MoveUp() {
    if (curr_x_ != 0) --curr_x_;
}

void AIField::MoveDown() {
    if (curr_x_ != 9) ++curr_x_;
}

void AIField::MoveLeft() {
    if (curr_y_ != 0) --curr_y_;
}

void AIField::MoveRight() {
    if (curr_y_ != 9) ++curr_y_;
}

bool AIField::MoveAbility(int x, int y) {
    return public_ai_field_[x][y] == elements_of_interface::sea;
}

bool AIField::PlayerMove(int x, int y) {
    if (private_ai_field_[x][y] == elements_of_interface::ship) {
        public_ai_field_[x][y] = elements_of_interface::damage;
        private_ai_field_[x][y] = elements_of_interface::damage;
        private_ai_field_[x][y] = elements_of_interface::damage;
        return true;
    }
    public_ai_field_[x][y] = elements_of_interface::miss;
    private_ai_field_[x][y] = elements_of_interface::miss;
    return false;
}

bool AIField::IsKilled(int x, int y, const std::string &direction) {
    try {
        if (private_ai_field_.at(x + 1)[y] == elements_of_interface::ship) {
            return false;
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (private_ai_field_.at(x - 1)[y] == elements_of_interface::ship) {
            return false;
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (private_ai_field_[x].at(y + 1) == elements_of_interface::ship) {
            return false;
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (private_ai_field_[x].at(y - 1) == elements_of_interface::ship) {
            return false;
        }
    }
    catch (std::out_of_range &err) {}

    if (direction.empty()) {
        try {
            if (public_ai_field_.at(x + 1)[y] == elements_of_interface::damage) {
                if (!IsKilled(x + 1, y, "right")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

        try {
            if (public_ai_field_.at(x - 1)[y] == elements_of_interface::damage) {
                if (!IsKilled(x - 1, y, "left")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

        try {
            if (public_ai_field_[x].at(y - 1) == elements_of_interface::damage) {
                if (!IsKilled(x, y - 1, "up")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

        try {
            if (public_ai_field_[x].at(y + 1) == elements_of_interface::damage) {
                if (!IsKilled(x, y + 1, "down")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

    } else if (direction == "right") {
        try {
            if (public_ai_field_.at(x + 1)[y] == elements_of_interface::damage) {
                if (!IsKilled(x + 1, y, "right")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

    } else if (direction == "left") {
        try {
            if (public_ai_field_.at(x - 1)[y] == elements_of_interface::damage) {
                if (!IsKilled(x - 1, y, "left")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

    } else if (direction == "up") {
        try {
            if (public_ai_field_[x].at(y - 1) == elements_of_interface::damage) {
                if (!IsKilled(x, y - 1, "up")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}

    } else if (direction == "down") {
        try {
            if (public_ai_field_[x].at(y + 1) == elements_of_interface::damage) {
                if (!IsKilled(x, y + 1, "down")) {
                    return false;
                }
            }
        }
        catch (std::out_of_range &err) {}
    }

    return true;
}

void AIField::AutoShoot() {
    int _x = curr_x_;
    int _y = curr_y_;

    bool direction = false;

    while (true) {
        --_y;

        try {
            if (private_ai_field_.at(_x).at(_y) == elements_of_interface::sea ||
                private_ai_field_.at(_x).at(_y) == elements_of_interface::miss) {
                break;
            }
        }
        catch (std::out_of_range &err) { break; }
    }
    ++_y;

    while (true) {
        --_x;

        try {
            if (private_ai_field_.at(_x).at(_y) == elements_of_interface::sea ||
                private_ai_field_.at(_x).at(_y) == elements_of_interface::miss) {
                break;
            }
        } catch (std::out_of_range &err) { break; }
    }
    ++_x;

    try {
        if (private_ai_field_.at(_x + 1).at(_y) == elements_of_interface::damage) {
            direction = true;
        }
    } catch (std::out_of_range &err) {}

    if (direction) {
        for (int i = -1; i < 2; ++i) {
            try {
                public_ai_field_.at(_x - 1).at(_y + i) = elements_of_interface::miss;
                private_ai_field_.at(_x - 1).at(_y + i) = elements_of_interface::miss;
            }
            catch (std::out_of_range &err) {}
        }

        while (true) {
            try {
                if (private_ai_field_.at(_x).at(_y) == elements_of_interface::sea ||
                    private_ai_field_.at(_x).at(_y) == elements_of_interface::miss) {
                    break;
                }
            }
            catch (std::out_of_range &err) { break; }

            public_ai_field_[_x][_y] = elements_of_interface::damage;
            private_ai_field_[_x][_y] = elements_of_interface::damage;

            try {
                public_ai_field_.at(_x).at(_y + 1) = elements_of_interface::miss;
                private_ai_field_.at(_x).at(_y + 1) = elements_of_interface::miss;
            }
            catch (std::out_of_range &err) {}

            try {
                public_ai_field_.at(_x).at(_y - 1) = elements_of_interface::miss;
                private_ai_field_.at(_x).at(_y - 1) = elements_of_interface::miss;
            }
            catch (std::out_of_range &err) {}

            ++_x;
        }

        for (int i = -1; i < 2; ++i) {
            try {
                public_ai_field_.at(_x).at(_y + i) = elements_of_interface::miss;
                private_ai_field_.at(_x).at(_y + i) = elements_of_interface::miss;
            }
            catch (std::out_of_range &err) {}
        }
    } else {
        for (int i = -1; i < 2; ++i) {
            try {
                public_ai_field_.at(_x + i).at(_y - 1) = elements_of_interface::miss;
                private_ai_field_.at(_x + i).at(_y - 1) = elements_of_interface::miss;
            }
            catch (std::out_of_range &err) {}
        }

        while (true) {
            try {
                if (private_ai_field_.at(_x).at(_y) == elements_of_interface::sea ||
                    private_ai_field_.at(_x).at(_y) == elements_of_interface::miss) {
                    break;
                }
            }
            catch (std::out_of_range &err) { break; }

            public_ai_field_[_x][_y] = elements_of_interface::damage;
            private_ai_field_[_x][_y] = elements_of_interface::damage;

            try {
                public_ai_field_.at(_x + 1).at(_y) = elements_of_interface::miss;
                private_ai_field_.at(_x + 1).at(_y) = elements_of_interface::miss;
            }
            catch (std::out_of_range &err) {}

            try {
                public_ai_field_.at(_x - 1).at(_y) = elements_of_interface::miss;
                private_ai_field_.at(_x - 1).at(_y) = elements_of_interface::miss;
            }
            catch (std::out_of_range &err) {}

            ++_y;
        }

        for (int i = -1; i < 2; ++i) {
            try {
                public_ai_field_.at(_x + i).at(_y) = elements_of_interface::miss;
                private_ai_field_.at(_x + i).at(_y) = elements_of_interface::miss;
            }
            catch (std::out_of_range &err) {}
        }
    }
}
