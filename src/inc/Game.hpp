#pragma once

#include "Scene.hpp"
#include "Field.hpp"
#include "Scaler.hpp"
#include "debug.hpp"
#include "ScoreBar.hpp"
#include "ColorSceme.hpp"
#include "GameOverScene.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdint>

#define SCORE_BAR_SIZE_PERCENTAGE 0.15f

class Game : public Scene {
public:
    Game(sf::Vector2u size, 
         sf::Vector2u fieldSize, 
         sf::Color backgroundColor = sf::Color::Black);

    const bool checkField();

    void run(sf::RenderWindow *window) override;
    void mouseClick(sf::RenderWindow *window) override;
    void resize() override;
    void render(sf::RenderWindow *window) override;

    const Field &getField() const;
    Field &getField();

    const ScoreBar &getScoreBar() const;
    ScoreBar &getScoreBar();

    const GameOverScene &getGameOverScene() const;
    GameOverScene &getGameOverScene();

private:
    Field field;
    ScoreBar scoreBar;
    GameOverScene gameOverScene;

    bool gameOver = false;

    bool lastChecked = true;
    bool ballPlaced = false;

    sf::Vector2i lastGrabedBallPostition = {-1, -1};
    Ball grabedBall;

    sf::CircleShape grabedBallShape;
    sf::CircleShape grabedBallShapeRing;
};