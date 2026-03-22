#include "inc/Ball.hpp"

Ball::Ball() : type(BALL_NONE_TYPE) {}
Ball::Ball(Ball::Type newType) : type(newType) {}

const sf::Color Ball::getColor() const {
    switch (type) {
        case BALL_RED_TYPE: return sf::Color::Red;
        case BALL_GREEN_TYPE: return sf::Color::Green;
        case BALL_BLUE_TYPE: return sf::Color::Blue;
        case BALL_YELLOW_TYPE: return sf::Color::Yellow;
        default: return sf::Color::Transparent;
    }
}