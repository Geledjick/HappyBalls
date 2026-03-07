#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

template<typename T>
class Matrix2 {
public:
    Matrix2<T>(const int r, const int c) : rows(r), cols(c), data(r * c) {
        data.shrink_to_fit();

        for (T &ele : data) {
            ele = T();
        }
    }

    const bool check(const int r, const int c) const {
        return (
            r < rows &&
            c < cols &&
            r >= 0 &&
            c >= 0
        );
    }
    const bool check(const sf::Vector2i index) const {
        return (
            index.x < rows &&
            index.y < cols &&
            index.x >= 0 &&
            index.y >= 0
        );
    }

    const T &at(const int r, const int c) const {
        return data[r * cols + c];
    }
    T &at(const int r, const int c) {
        return data[r * cols + c];
    }

    const T &at(const sf::Vector2i index) const {
        return data[index.x * cols + index.y];
    }
    T &at(const sf::Vector2i index) {
        return data[index.x * cols + index.y];
    }

    const int getRows() const {
        return rows;
    }
    const int getCols() const {
        return cols;
    }
    const int getSize() const {
        return rows * cols;
    }

private:
    std::vector<T> data;
    const int rows, cols;
};