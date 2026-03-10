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
#include <vector>

#include "../lib/Scene.cpp"
#include "Ball.cpp"
#include "../lib/Scaler.cpp"
#include "ColorSceme.hpp"
#include "../lib/Matrix2.cpp"

class Field : public Scene {
public:
    Field(sf::Vector2u size, const sf::Vector2u fieldSize, const sf::Color backgroundColor = sf::Color::Black) : 
        Scene(size, backgroundColor), 
        data(fieldSize.x, fieldSize.y), 
        maxGenerateIterations((fieldSize.x + fieldSize.y) / 4), 
        countGenerateInIteration((fieldSize.x + fieldSize.y))
    {
        tileWidth = (float)size.x / fieldSize.x;
        tileHeight = (float)size.y / fieldSize.y;

        tileLogicWidth = tileWidth;
        tileLogicHeight = tileHeight;

        ballLogicOffset = {
            (tileLogicWidth * 0.33f) / 2,
            (tileLogicHeight * 0.33f) / 2,
        };

        ballSize = (tileHeight > tileWidth ? tileWidth : tileHeight) * 0.33f;
        ball.setRadius(ballSize);
        ballOffset = ballSize / 2;

        tile.setSize({tileWidth, tileHeight});
        tile.setFillColor(CELL_COLOR);
        tile.setOutlineColor(FIELD_COLOR);
        tile.setOutlineThickness(2);

        srand(time(NULL));

        generate();
    }

    void render(sf::RenderWindow *window) override {
        if (neededRender) {
            neededRender = false;
            renderTexture.clear(backgroundColor);

            for (int r = 0; r < data.getRows(); r++) {
                for (int c = 0; c < data.getCols(); c++) {
                    tile.setPosition(
                        {r * tileWidth, c * tileHeight}
                    );

                    renderTexture.draw(tile);

                    ball.setPosition(
                        tile.getPosition() + sf::Vector2f{ballOffset, ballOffset}
                    );

                    const sf::Color ballColor = data.at(r, c).getColor();
                    ball.setFillColor(ballColor);
                    ball.setOutlineColor(ballColor != sf::Color::Transparent ? sf::Color::Black : sf::Color::Transparent);

                    renderTexture.draw(ball);
                }
            }

            renderTexture.display();
        }
    }

    bool generate() {
        sf::Vector2i pos;
        for (int i = 0; i < countGenerateInIteration; i++) {
            int j = 0;
            do {
                pos.x = rand() % data.getRows();
                pos.y = rand() % data.getCols();
                j++;
                if (j > maxGenerateIterations) {
                    return true;
                }

            } while (data.at(pos).type != BALL_NONE_TYPE);

            data.at(pos) = Ball{Ball::Type((rand() % (BALL_TYPES_COUNT - 1)) + 1)};
        }

        reRender();
        return false;
    }

    void resize() override {
        tileLogicWidth = Scaler::scaleX(tileWidth);
        tileLogicHeight = Scaler::scaleY(tileHeight);

        ballLogicOffset = {
            (tileLogicWidth * 0.33f) / 2,
            (tileLogicHeight * 0.33f) / 2,
        };
    }

    // Getters
    const float getBallSize() const {
        return ballSize;
    }

    // Tile logic size
    const sf::Vector2f getTileLogicSize() const {
        return sf::Vector2f(tileLogicWidth, tileLogicHeight);
    }
    const float getTileLogicWidth() const {
        return tileLogicWidth;
    }
    const float getTileLogicHeight() const {
        return tileLogicHeight;
    }

    // Ball logic size
    const sf::Vector2f getBallLogicOffset() const {
        return ballLogicOffset;
    }
    const float getBallLogicOffsetWidth() const {
        return ballLogicOffset.x;
    }
    const float getBallLogicOffsetHeight() const {
        return ballLogicOffset.y;
    }

    // Data
    const Matrix2<Ball> &getData() const {
        return data;
    }
    Matrix2<Ball> &getData() {
        return data;
    }

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