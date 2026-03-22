#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>

template<typename T>
class Matrix2 {
public:
    explicit Matrix2<T>(int r, int c);

    bool check(int r, int c) const;
    bool check(const sf::Vector2i index) const;

    const T& at(int r, int c) const;
    T& at(int r, int c);
    
    const T& at(const sf::Vector2i index) const;
    T& at(const sf::Vector2i index);

    int getRows() const;
    int getCols() const;
    int getSize() const;

private:
    std::vector<T> data;
    const int rows;
    const int cols;
};

template<typename T>
Matrix2<T>::Matrix2(int r, int c) : rows(r), cols(c), data(r * c) {
    for (T& ele : data) {
        ele = T();
    }
}

template<typename T>
bool Matrix2<T>::check(int r, int c) const {
    return (r >= 0 && r < rows && c >= 0 && c < cols);
}

template<typename T>
bool Matrix2<T>::check(const sf::Vector2i index) const {
    return (index.x >= 0 && index.x < rows && index.y >= 0 && index.y < cols);
}

template<typename T>
const T& Matrix2<T>::at(int r, int c) const {
    return data[r * cols + c];
}

template<typename T>
T& Matrix2<T>::at(int r, int c) {
    return data[r * cols + c];
}

template<typename T>
const T& Matrix2<T>::at(const sf::Vector2i index) const {
    return data[index.x * cols + index.y];
}

template<typename T>
T& Matrix2<T>::at(const sf::Vector2i index) {
    return data[index.x * cols + index.y];
}

template<typename T>
int Matrix2<T>::getRows() const {
    return rows;
}

template<typename T>
int Matrix2<T>::getCols() const {
    return cols;
}

template<typename T>
int Matrix2<T>::getSize() const {
    return rows * cols;
}