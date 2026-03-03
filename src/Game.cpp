#pragma once

#include "Scene.cpp"
#include "Field.cpp"
#include "debug.cpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

class Game : public Scene {
public:
    Game(sf::Vector2u size, sf::Vector2u fieldSize, sf::Color backgroundColor = sf::Color::Black) : Scene(size, backgroundColor), field(size - sf::Vector2u(0, size.y / fieldSize.y), fieldSize, {0, 0}) {
        grabedBall = BALL_NONE;

        grabedBallShape.setRadius(field.getBallSize());
        grabedBallShapeRing.setRadius(field.getBallSize());
    }

    void run() override {
        field.run();
    }

    void mouseClick(sf::RenderWindow *window) override {
        const sf::Vector2i clickPosition = sf::Mouse::getPosition(*window);

        const sf::Vector2i targetPosition = {
            int(clickPosition.x / field.getTileLogicWidth()),
            int(clickPosition.y / field.getTileLogicHeight())
        };

        DEBUG_PRINTF("Click on: X - %d, Y - %d\n", targetPosition.x, targetPosition.y);
        
        if (field.get(targetPosition) == nullptr) {
            return;
        }

        // CONTINUE <---
    }

    void resize(sf::Vector2u lastResolution, sf::Vector2u newResolution) override {
        field.resize(lastResolution, newResolution);
    }

    void render(sf::RenderWindow *window) override {
        field.render(window);
        if (neededRender) {
            neededRender = false;
            renderTexture.clear(backgroundColor);
            
            renderTexture.draw(field);

            if (grabedBall.type != BALL_NONE_TYPE) {
                const sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
                grabedBallShape.setPosition({(float)mousePosition.x, (float)mousePosition.y});

                renderTexture.draw(grabedBallShape);
                renderTexture.draw(grabedBallShapeRing);
            }

            renderTexture.display();
        }
    }

private:
    Field field;
    Ball grabedBall;
    
    sf::CircleShape grabedBallShape;
    sf::CircleShape grabedBallShapeRing;
};