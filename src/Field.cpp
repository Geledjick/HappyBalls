#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "Ball.hpp"
#include "Scene.cpp"

class Field : public Scene {
public:
    Field(sf::Vector2u size, const sf::Vector2u fieldSize, const sf::Vector2f fieldPos) : Scene(size, sf::Color(50, 52, 70)), rows(fieldSize.x), cols(fieldSize.y), maxGenerateIterations((cols + rows) / 2), countGenerateInIteration((cols + rows) / 3) {
        data.resize(rows);
        data.shrink_to_fit();
        for (std::vector<BALL> &row : data) {
            row.resize(cols);
            row.shrink_to_fit();

            for(BALL ball : row) {
                ball = BALL_NONE;
            }
        }

        tileWidth = (float)size.x / rows;
        tileHeight = (float)size.y / cols;

        ballSize = (tileHeight > tileWidth ? tileWidth : tileHeight) * 0.33f;
        ball.setRadius(ballSize);

        ballOffset = {(tileWidth / 2) - ballSize, (tileHeight / 2) - ballSize};

        tile.setSize({tileWidth, tileHeight});
        tile.setFillColor(backgroundColor);
        tile.setOutlineColor(sf::Color(68, 71, 90));
        tile.setOutlineThickness(3);

        generate();
        reRender();
    }

    const BALL *get(const sf::Vector2u index) {
        if (index.x >= rows || index.y >= cols) {
            return nullptr;
        }
        return &data[index.x][index.y];
    }

    void set(const sf::Vector2u index, const BALL ball = BALL_NONE) {
        if (index.x >= rows || index.y >= cols) {
            return;
        }
        data[index.x][index.y] = ball;
    }

    const BALL *getusf(const sf::Vector2u index) {
        return &data[index.x][index.y];
    }

    void setusf(const sf::Vector2u index, const BALL ball = BALL_NONE) {
        data[index.x][index.y] = ball;
    }

    void run() override {
        reRender();
    }

    void render() override {
        if (neededRender) {
            neededRender = false;
            renderTexture.clear(backgroundColor);

            for (unsigned int r = 0; r < rows; r++) {
                for (unsigned int c = 0; c < cols; c++) {
                    tile.setPosition({r * tileWidth, c * tileHeight});
                    renderTexture.draw(tile);

                    ball.setPosition(tile.getPosition() + ballOffset);
                    ball.setFillColor(*getusf({r, c}));
                    if (ball.getFillColor() == BALL_NONE) {
                        ball.setFillColor(backgroundColor);
                    }

                    renderTexture.draw(ball);
                }
            }

            renderTexture.display();
        }
    }

    bool generate() {
        sf::Vector2u pos;
        for (int i = 0; i < countGenerateInIteration; i++) {
            int j = 0;
            do {
                pos.x = rand() % rows;
                pos.y = rand() % cols;
                j++;
                if (j > maxGenerateIterations) {
                    return true;
                }

            } while (*getusf(pos) != BALL_NONE);

            BALL generated;
            switch (rand() % 4) {
                case 0: generated = BALL_RED; break;
                case 1: generated = BALL_GREEN; break;
                case 2: generated = BALL_BLUE; break;
                case 3: generated = BALL_YELLOW; break;
            }

            setusf(pos, generated);
        }

        return false;
    }

private:
    float tileWidth, tileHeight;
    float ballSize;
    sf::Vector2f ballOffset;
    const unsigned int rows, cols;

    const unsigned int countGenerateInIteration;
    const unsigned int maxGenerateIterations;

    std::vector<std::vector<BALL>> data;
    sf::RectangleShape tile;
    sf::CircleShape ball;
};