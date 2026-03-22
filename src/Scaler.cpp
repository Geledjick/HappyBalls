#include "inc/Scaler.hpp"

sf::Vector2u Scaler::startResolution = {};
sf::Vector2f Scaler::scaleMul = {};

Scaler::Scaler(const sf::Vector2u newStartResolution) {
    startResolution = newStartResolution;
    scaleMul = {1.f, 1.f};
}

void Scaler::rescale(const sf::Vector2u newResolution) {
    scaleMul = {
        (float)newResolution.x / startResolution.x,
        (float)newResolution.y / startResolution.y
    };
}

const sf::Vector2f Scaler::getScaleMul() {
    return scaleMul;
}

const float Scaler::getScaleMulX() {
    return scaleMul.x;
}

const float Scaler::getScaleMulY() {
    return scaleMul.y;
}