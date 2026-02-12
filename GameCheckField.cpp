#include "Game.hpp"

bool Game::checkVertical() {
    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            if (
                (field[c][r].type != Ball::Type::None) &&
                (r + 1 < rows && r + 2 < rows)
            ) {
                Ball::Type targetType = field[c][r].type;
                if (targetType == field[c][r + 1].type && targetType == field[c][r + 2].type) {
                    int len = 3;
                    field[c][r].type = Ball::Type::None;
                    field[c][r + 1].type = Ball::Type::None;
                    field[c][r + 2].type = Ball::Type::None;
                    while (
                        (r + len < rows) &&
                        (targetType == field[c][r + len].type)
                    ) {
                        field[c][r + len].type = Ball::Type::None;
                        len++;
                    }
                    r += len;
                    score += len * combo;
                    combo++;
                    return true;
                    redrawCombo();
                    redrawScoreText();
                    redrawField();
                }
            }
        }
    }
    return false;
}

bool Game::checkHorizont() {
    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            if (
                (field[c][r].type != Ball::Type::None) &&
                (c + 1 < cols && c + 2 < cols)
            ) {
                Ball::Type targetType = field[c][r].type;
                if (targetType == field[c + 1][r].type && targetType == field[c + 2][r].type) {
                    int len = 3;
                    field[c][r].type = Ball::Type::None;
                    field[c + 1][r].type = Ball::Type::None;
                    field[c + 2][r].type = Ball::Type::None;
                    while (
                        (c + len < cols) &&
                        (targetType == field[c + len][r].type)
                    ) {
                        field[c + len][r].type = Ball::Type::None;
                        len++;
                    }
                    r += len;
                    score += len * combo;
                    combo++;
                    return true;
                    redrawCombo();
                    redrawScoreText();
                    redrawField();
                }
            }
        }
    }
    return false;
}

bool Game::checkDiagonalDownRight() {
    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            if (
                (field[c][r].type != Ball::Type::None) &&
                (c + 1 < cols && c + 2 < cols) &&
                (r + 1 < rows && r + 2 < rows)
            ) {
                Ball::Type targetType = field[c][r].type;
                if (targetType == field[c + 1][r + 1].type && targetType == field[c + 2][r + 2].type) {
                    int len = 3;
                    field[c][r].type = Ball::Type::None;
                    field[c + 1][r + 1].type = Ball::Type::None;
                    field[c + 2][r + 2].type = Ball::Type::None;
                    while (
                        (c + len < cols) &&
                        (r + len < rows) &&
                        (targetType == field[c + len][r + len].type)
                    ) {
                        field[c + len][r + len].type = Ball::Type::None;
                        len++;
                    }
                    r += len;
                    score += len * combo;
                    combo++;
                    return true;
                    redrawCombo();
                    redrawScoreText();
                    redrawField();
                }
            }
        }
    }
    return false;
}

bool Game::checkDiagonalUpRight() {
    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            if (
                (field[c][r].type != Ball::Type::None) &&
                (c + 1 < cols && c + 2 < cols) &&
                (r - 1 >= 0 && r - 2 >= 0)
            ) {
                Ball::Type targetType = field[c][r].type;
                if (targetType == field[c + 1][r - 1].type && targetType == field[c + 2][r - 2].type) {
                    int len = 3;
                    field[c][r].type = Ball::Type::None;
                    field[c + 1][r - 1].type = Ball::Type::None;
                    field[c + 2][r - 2].type = Ball::Type::None;
                    while (
                        (c + len < cols) &&
                        (r - len >= 0) &&
                        (targetType == field[c + len][r - len].type)
                    ) {
                        field[c + len][r - len].type = Ball::Type::None;
                        len++;
                    }
                    r += len;
                    score += len * combo;
                    combo++;
                    redrawCombo();
                    redrawScoreText();
                    redrawField();
                    return true;
                }
            }
        }
    }
    return false;
}