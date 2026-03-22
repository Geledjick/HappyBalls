#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdlib>
#include <ctime>

#include "Scene.hpp"
#include "Ball.hpp"
#include "Scaler.hpp"
#include "ColorSceme.hpp"
#include "Matrix2.hpp"

class Field : public Scene {
public:
    Field(sf::Vector2u size, const sf::Vector2u fieldSize, const sf::Color backgroundColor = sf::Color::Black);

    void render(sf::RenderWindow *window) override;

    bool generate();

    void resize() override;

    // Getters
    const float getBallSize() const;

    // Tile logic size
    const sf::Vector2f getTileLogicSize() const;
    const float getTileLogicWidth() const;
    const float getTileLogicHeight() const;

    // Ball logic size
    const sf::Vector2f getBallLogicOffset() const;
    const float getBallLogicOffsetWidth() const;
    const float getBallLogicOffsetHeight() const;

    // Data
    const Matrix2<Ball> &getData() const;
    Matrix2<Ball> &getData();

private:
    float tileWidth, tileHeight;
    float tileLogicWidth, tileLogicHeight;
    float ballSize;
    float ballOffset;
    sf::Vector2f ballLogicOffset;

    const unsigned int countGenerateInIteration;
    const unsigned int maxGenerateIterations;

    Matrix2<Ball> data;
    
    sf::RectangleShape tile;
    sf::CircleShape ball;
};