#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>

class Scaler {
public:
    Scaler(const sf::Vector2u newStartResolution) {
        startResolution = newStartResolution;
        scaleMul = {1.f, 1.f};
    }

    template<typename T>
    static const T scaleX(const T value) {
        return value * scaleMul.x;
    }
    template<typename T>
    static const T scaleY(const T value) {
        return value * scaleMul.y;
    }
    template<typename T>
    static const sf::Vector2<T> scaleV(const sf::Vector2<T> vec) {
        return {vec.x * scaleMul.x, vec.y * scaleMul.y};
    }

    template<typename T>
    static const T restoreX(const T value) {
        return value / scaleMul.x;
    }
    template<typename T>
    static const T restoreY(const T value) {
        return value / scaleMul.y;
    }
    template<typename T>
    static const sf::Vector2<T> restoreV(const sf::Vector2<T> vec) {
        return {vec.x / scaleMul.x, vec.y / scaleMul.y};
    }

    static void rescale(const sf::Vector2u newResolution) {
        scaleMul = {
            (float)newResolution.x / startResolution.x,
            (float)newResolution.y / startResolution.y
        };
    }

    static const sf::Vector2f getScaleMul() {
        return scaleMul;
    }
    static const float getScaleMulX() {
        return scaleMul.x;
    }
    static const float getScaleMulY() {
        return scaleMul.y;
    }
    
private:
    static sf::Vector2u startResolution;
    static sf::Vector2f scaleMul;
};