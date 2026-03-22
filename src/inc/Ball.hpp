#pragma once

#include <SFML/Graphics/Color.hpp>

#define BALL_NONE Ball{Ball::Type::None}
#define BALL_RED Ball{Ball::Type::Red}
#define BALL_GREEN Ball{Ball::Type::Green}
#define BALL_BLUE Ball{Ball::Type::Blue}
#define BALL_YELLOW Ball{Ball::Type::Yellow}

#define BALL_NONE_TYPE Ball::Type::None
#define BALL_RED_TYPE Ball::Type::Red
#define BALL_GREEN_TYPE Ball::Type::Green
#define BALL_BLUE_TYPE Ball::Type::Blue
#define BALL_YELLOW_TYPE Ball::Type::Yellow

#define BALL_TYPES_COUNT ((unsigned int)Ball::Type::Count)

struct Ball {
    enum Type {
        None,
        Red,
        Green,
        Blue,
        Yellow,
        Count
    };

    Ball();
    Ball(Ball::Type newType);

    Ball::Type type;

    const sf::Color getColor() const;
};