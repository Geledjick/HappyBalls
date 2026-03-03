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
#include <vector>

#include "Scene.cpp"
#include "Ball.cpp"

class Field : public Scene {
public:
    Field(sf::Vector2u size, const sf::Vector2u fieldSize, const sf::Vector2f fieldPos) : Scene(size, sf::Color(50, 52, 70)), rows(fieldSize.x), cols(fieldSize.y), maxGenerateIterations((cols + rows) / 2), countGenerateInIteration((cols + rows) / 3) {
        data.resize(rows);
        data.shrink_to_fit();
        for (std::vector<Ball> &row : data) {
            row.resize(cols);
            row.shrink_to_fit();

            for(Ball ball : row) {
                ball = BALL_NONE;
            }
        }

        tileWidth = (float)size.x / rows;
        tileHeight = (float)size.y / cols;

        tileLogicWidth = tileWidth;
        tileLogicHeight = tileHeight;

        ballSize = (tileHeight > tileWidth ? tileWidth : tileHeight) * 0.33f;
        ball.setRadius(ballSize);
        ballOffset = {(tileWidth / 2) - ballSize, (tileHeight / 2) - ballSize};
        ball.setOutlineThickness(2);

        tile.setSize({tileWidth, tileHeight});
        tile.setFillColor(backgroundColor);
        tile.setOutlineColor(sf::Color(68, 71, 90));
        tile.setOutlineThickness(4);

        data[0][0].type = Ball::Type::Blue;

        generate();
    }

    const Ball *get(const sf::Vector2i index) {
        if (index.x >= rows || index.y >= cols) {
            return nullptr;
        }
        return &data[index.x][index.y];
    }

    void set(const sf::Vector2i index, const Ball ball = BALL_NONE) {
        if (index.x >= rows || index.y >= cols) {
            return;
        }
        data[index.x][index.y] = ball;
    }

    const Ball *getusf(const sf::Vector2i index) {
        return &data[index.x][index.y];
    }

    void setusf(const sf::Vector2i index, const Ball ball = BALL_NONE) {
        data[index.x][index.y] = ball;
    }

    void render(sf::RenderWindow *window) override {
        if (neededRender) {
            neededRender = false;
            renderTexture.clear(backgroundColor);

            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    tile.setPosition({r * tileWidth, c * tileHeight});
                    renderTexture.draw(tile);

                    ball.setPosition(tile.getPosition() + ballOffset);
                    const sf::Color ballColor = getusf({r, c})->getColor();
                    ball.setFillColor(ballColor);
                    ball.setOutlineColor(ballColor != sf::Color::Transparent ? sf::Color::Black : sf::Color::Transparent);
                    renderTexture.draw(ball);
                }
            }

            renderTexture.display();
        }
    }

    void run() override {

    }

    bool generate() {
        sf::Vector2i pos;
        for (int i = 0; i < countGenerateInIteration; i++) {
            int j = 0;
            do {
                pos.x = rand() % rows;
                pos.y = rand() % cols;
                j++;
                if (j > maxGenerateIterations) {
                    return true;
                }

            } while (getusf(pos)->type != BALL_NONE_TYPE);

            setusf(pos, {Ball::Type((rand() % (BALL_TYPES_COUNT - 1)) + 1)});
        }

        reRender();
        return false;
    }

    void resize(sf::Vector2u lastResolution, sf::Vector2u newResolution) override {
        tileLogicWidth *= (float)newResolution.x / lastResolution.x;
        tileLogicHeight *= (float)newResolution.y / lastResolution.y;
    }

    // Getters
    const float getBallSize() const {
        return ballSize;
    }

    const sf::Vector2f getTileLogicSize() const {
        return sf::Vector2f(tileLogicWidth, tileLogicHeight);
    }
    const float getTileLogicWidth() const {
        return tileLogicWidth;
    }
    const float getTileLogicHeight() const {
        return tileLogicHeight;
    }

private:
    float tileWidth, tileHeight;
    float tileLogicWidth, tileLogicHeight;
    float ballSize;
    sf::Vector2f ballOffset;
    const unsigned int rows, cols;

    const unsigned int countGenerateInIteration;
    const unsigned int maxGenerateIterations;

    std::vector<std::vector<Ball>> data;
    sf::RectangleShape tile;
    sf::CircleShape ball;
};