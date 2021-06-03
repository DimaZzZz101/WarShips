// Copyright 2021 DimaZzZz101

#include "../include/AI.hpp"

AI::AI() {
    // Поле целей изначально заполняется нулями
    for (auto &row: cells_) {
        std::fill(row.begin(), row.end(), 0);
    }

    // Игровое поле заполняется "морем"
    for (auto &row: ai_field_) {
        std::fill(row.begin(), row.end(), elements_of_interface::sea);
    }
}

// ИИ располагает корабли случайным образом
void AI::ShipsLocation() {
    for (size_t i = 0; i < 10; ++i) {
        int _x = rand() % 10;
        int _y = rand() % 10;
        bool cell = rand() % 2;

        while (!CheckShip(_x, _y, ships_[i], cell)) {
            _x = rand() % 10;
            _y = rand() % 10;
            cell = rand() % 2;
        }

        CreateShip(_x, _y, ships_[i], cell);
    }
}

std::pair<int, int> AI::Shot() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (cells_[i][j] == 1) {
                std::pair<int, int> point = ChoosingCell(i, j);
                return point;
            }
        }
    }

    int x = rand() % 10;
    int y = rand() % 10;

    while (cells_[x][y] != 0) {
        x = rand() % 10;
        y = rand() % 10;
    }

    std::pair<int, int> point(x, y);

    return point;
}

std::array<std::array<int, 10>, 10> &AI::GetCells() { return cells_; }

std::array<std::array<char, 10>, 10> &AI::GetAiField() { return ai_field_; }

std::pair<int, int> AI::ChoosingCell(int x, int y) {
    int direction = PosOfShot(x, y);

    if (direction == 1) {
        try {
            if (cells_.at(x - 1).at(y) == 0) {
                std::pair<int, int> point(x - 1, y);
                return point;
            }
        }
        catch (std::out_of_range &err) {}

        while (cells_[x][y] != 0) ++x;

        std::pair<int, int> point(x, y);
        return point;
    }

    if (direction == 2) {
        try {
            if (cells_.at(x).at(y - 1) == 0) {
                std::pair<int, int> point(x, y - 1);
                return point;
            }
        }
        catch (std::out_of_range &err) {}

        while (cells_[x][y] != 0) ++y;

        std::pair<int, int> point(x, y);
        return point;
    }

    if (direction == 3) {
        try {
            if (cells_.at(x + 1).at(y) == 0) {
                std::pair<int, int> point(x + 1, y);
                return point;
            }
        }
        catch (std::out_of_range &err) {}

        while (cells_[x][y] != 0) --x;

        std::pair<int, int> point(x, y);
        return point;
    }

    if (direction == 4) {
        try {
            if (cells_.at(x).at(y + 1) == 0) {
                std::pair<int, int> point(x, y + 1);
                return point;
            }
        }
        catch (std::out_of_range &err) {}

        while (cells_[x][y] != 0) --y;

        std::pair<int, int> point(x, y);
        return point;
    }

    try {
        if (cells_.at(x - 1).at(y) == 0) {
            std::pair<int, int> point(x - 1, y);
            return point;
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (cells_.at(x).at(y - 1) == 0) {
            std::pair<int, int> point(x, y - 1);
            return point;
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (cells_.at(x + 1).at(y) == 0) {
            std::pair<int, int> point(x + 1, y);
            return point;
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (cells_.at(x).at(y + 1) == 0) {
            std::pair<int, int> point(x, y + 1);
            return point;
        }
    }
    catch (std::out_of_range &err) {}

}

int AI::PosOfShot(int x, int y) {
    try {
        if (cells_.at(x + 1).at(y) == 1) {
            return 1;
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (cells_.at(x - 1).at(y) == 1) {
            return 3;
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (cells_.at(x).at(y + 1) == 1) {
            return 2;
        }
    }
    catch (std::out_of_range &err) {}

    try {
        if (cells_.at(x).at(y - 1) == 1) {
            return 4;
        }
    }
    catch (std::out_of_range &err) {}

    return 0;
}

bool AI::CheckShip(int curr_x_, int curr_y_, size_t ship_size, bool pos) {
    if (ai_field_[curr_x_][curr_y_] == elements_of_interface::ship ||
        ai_field_[curr_x_][curr_y_] == elements_of_interface::empty) {
        return false;
    }

    if (!pos) {
        if (10 - curr_x_ >= ship_size) {
            for (size_t i = 0; i < ship_size; ++i) {
                if (ai_field_[curr_x_ + i][curr_y_] == elements_of_interface::ship ||
                    ai_field_[curr_x_ + i][curr_y_] == elements_of_interface::empty) {
                    return false;
                }
            }
            return true;
        }
    } else {
        if (10 - curr_y_ >= ship_size) {
            for (size_t i = 0; i < ship_size; ++i) {
                if (ai_field_[curr_x_][curr_y_ + i] == elements_of_interface::ship ||
                    ai_field_[curr_x_][curr_y_ + i] == elements_of_interface::empty) {
                    return false;
                }
            }
            return true;
        }
    }

    return false;
}

void AI::CreateShip(int curr_x_, int curr_y_, size_t ship_size, bool pos) {
    // горизонтальное расположение
    if (pos) {
        for (size_t i = 0; i < ship_size; ++i) {
            ai_field_[curr_x_][curr_y_ + i] = elements_of_interface::ship;
        }

        for (size_t i = 0; i < ship_size + 2; ++i) {
            try {
                ai_field_.at(curr_x_ + 1).at(curr_y_ + i - 1) = elements_of_interface::empty;
            }
            catch (std::out_of_range &err) {}

            try {
                ai_field_.at(curr_x_ - 1).at(curr_y_ + i - 1) = elements_of_interface::empty;
            }
            catch (std::out_of_range &err) {}
        }

        try {
            ai_field_.at(curr_x_).at(curr_y_ - 1) = elements_of_interface::empty;
        }
        catch (std::out_of_range &err) {}

        try {
            ai_field_.at(curr_x_).at(curr_y_ + ship_size) = elements_of_interface::empty;
        }
        catch (std::out_of_range &err) {}

        // вертикальное расположение
    } else {
        for (size_t i = 0; i < ship_size; ++i) {
            ai_field_[curr_x_ + i][curr_y_] = elements_of_interface::ship;
        }

        for (size_t i = 0; i < ship_size + 2; ++i) {
            try {
                ai_field_.at(curr_x_ + i - 1).at(curr_y_ + 1) = elements_of_interface::empty;
            }
            catch (std::out_of_range &err) {}

            try {
                ai_field_.at(curr_x_ + i - 1).at(curr_y_ - 1) = elements_of_interface::empty;
            }
            catch (std::out_of_range &err) {}
        }

        try {
            ai_field_.at(curr_x_ - 1).at(curr_y_) = elements_of_interface::empty;
        }
        catch (std::out_of_range &err) {}

        try {
            ai_field_.at(curr_x_ + ship_size).at(curr_y_) = elements_of_interface::empty;
        }
        catch (std::out_of_range &err) {}
    }
}
