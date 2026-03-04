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
    Game(sf::Vector2u size, sf::Vector2u fieldSize, sf::Color backgroundColor = sf::Color::Black) : 
        Scene(size, backgroundColor), 
        field(size - sf::Vector2u(0, (size.y / fieldSize.y) * 2), fieldSize, {0, 0}) 
    {
        grabedBall = BALL_NONE;

        grabedBallShape.setRadius(field.getBallSize());
        grabedBallShapeRing.setRadius(field.getBallSize());

        grabedBallShapeRing.setFillColor(sf::Color::Transparent);
        grabedBallShapeRing.setOutlineThickness(4);
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

        const Ball *targetBall = field.get(targetPosition);
        if (targetBall == nullptr) {
            return;
        }

        if (grabedBall.type == BALL_NONE_TYPE && targetBall->type != BALL_NONE_TYPE) {
            window->setMouseCursorVisible(false);

            grabedBall = *targetBall; // set garbed ball
            
            grabedBallShapeRing.setPosition({
                targetPosition.x * Scaler::restoreX(field.getTileLogicWidth()) + Scaler::restoreX(field.getBallLogicOffsetWidth()),
                targetPosition.y * Scaler::restoreY(field.getTileLogicHeight()) + Scaler::restoreY(field.getBallLogicOffsetHeight())
            });

            const sf::Color targetColor = targetBall->getColor();

            grabedBallShapeRing.setOutlineColor(targetColor);
            grabedBallShape.setFillColor(targetColor);

            field.setusf(targetPosition, BALL_NONE); // clear target ball

            DEBUG_PUTS("GRAB!");

        } else if (grabedBall.type != BALL_NONE_TYPE && targetBall->type == BALL_NONE_TYPE) {
            window->setMouseCursorVisible(true);

            field.setusf(targetPosition, grabedBall);
            grabedBall = BALL_NONE;

            DEBUG_PUTS("PUT!");
        }

        field.reRender();
        reRender();
    }

    void resize() override {
        field.resize();
    }

    void render(sf::RenderWindow *window) override {
        field.render(window);
        if (neededRender) {
            neededRender = false;
            renderTexture.clear(backgroundColor);
            
            renderTexture.draw(field);

            if (grabedBall.type != BALL_NONE_TYPE) {
                const sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

                grabedBallShape.setPosition({
                    Scaler::restoreX(mousePosition.x) - Scaler::restoreX(field.getBallLogicOffsetWidth() * 1.5f),
                    Scaler::restoreY(mousePosition.y) - Scaler::restoreY(field.getBallLogicOffsetHeight() * 1.5f)
                });

                renderTexture.draw(grabedBallShape);
                renderTexture.draw(grabedBallShapeRing);

                reRender();
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