#pragma once

// SFML
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Other
#include <array>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <queue>
#include <stdio.h>
#include <string>
#include <vector>

struct Ball {
    enum Type {
        None,
        Red,
        Green,
        Blue,
        Yellow,
        TypeCount
    } type;

    sf::Color getColor();
};

class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    sf::RenderTexture fieldRenderTexture;
    sf::Sprite fieldSprite;

    static const int cols = 14;
    static const int rows = 14;

    std::array<std::array<Ball, rows>, cols> field;

    float tileSize;
    float ballSize;
    float ballCenterPosition;
    sf::RectangleShape tile;
    sf::CircleShape ball;
    sf::CircleShape grabedRing;
    sf::CircleShape grabedBallShape;
    const int outlineSize = 2.f;
    const int spaceOutlineSize = outlineSize * 2;

    const sf::Color backgroundColor = sf::Color(40, 42, 54);

    const int countGenerateBalls = (rows + cols);
    const int maxGeneratorIterations = rows * cols * 0.5f;

    int score = 0;
    bool gameOver = false;
    std::optional<sf::Event> event;
    int combo = 1;

    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text comboText;

    sf::Vector2i lastMousePosition;
    sf::Vector2i lastTargetPosition;
    sf::Vector2i lastGrabedMousePosition;
    bool lastMouseClickState = false;
    Ball grabedBall{Ball::Type::None};

    bool ballIsTransfer = false;
    std::array<sf::Vector2i, 4> directions;

    void handleEvents();
    void resizeWindow();
    void redrawField();
    void redrawScoreText();
    void redrawCombo();
    void render();
    void mouseClick();
    bool findPath(sf::Vector2i targetPosition);
    void generateBalls();
    void resetGame();
    bool checkVertical();
    bool checkHorizont();
    bool checkDiagonalDownRight();
    bool checkDiagonalUpRight();
};