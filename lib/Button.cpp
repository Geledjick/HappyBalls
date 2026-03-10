#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>

#include "Scene.cpp"
#include "Scaler.cpp"
#include "../src/debug.cpp"

class Button : public Scene {
public:
    Button(const sf::Vector2u size, const sf::Color defaultColor, const sf::Color checkedColor, const sf::Color activeColor) :
        defaultColor(defaultColor),
        checkedColor(checkedColor),
        activeColor(activeColor),
        Scene(size)
    {

    }

    void mouseClick(sf::RenderWindow *window) override {
        if (state == ButtonStates::CHECKED) {
            state = ACTIVED;
            DEBUG_PUTS("Button is pressed!");
            reRender();
        }
    }

    void mouseCheck(sf::RenderWindow *window) {
        const sf::Vector2i mousePosition = Scaler::scaleV(sf::Mouse::getPosition(*window));
        const sf::Vector2f buttonPosition = Scaler::scaleV<float>(getPosition());
        const sf::Vector2u buttonSize = Scaler::scaleV(renderTexture.getSize());

        if (
            mousePosition.x >= buttonPosition.x &&
            mousePosition.y >= buttonPosition.y &&
            mousePosition.x <= buttonPosition.x + buttonSize.x &&
            mousePosition.y <= buttonPosition.y + buttonSize.y
        ) {
            state = ButtonStates::CHECKED;
            DEBUG_PUTS("Button is checked!");
            reRender();
        } else {
            state = ButtonStates::DEFAULT;
            reRender();
        }
    }

    void run(sf::RenderWindow *window) override {
        mouseCheck(window);
    }

    void render(sf::RenderWindow *window) override {
        if (neededRender) {
            neededRender = false;

            sf::Color targetColor;
            switch (state) {
                case ButtonStates::CHECKED:
                    targetColor = checkedColor;
                    break;
                case ButtonStates::ACTIVED:
                    targetColor = activeColor;
                    break;
                default:
                    targetColor = defaultColor;
            }

            renderTexture.clear(targetColor);

            renderTexture.display();
        }        
    }

protected:
    const sf::Color defaultColor;
    const sf::Color checkedColor;
    const sf::Color activeColor;

    enum ButtonStates {
        DEFAULT,
        CHECKED,
        ACTIVED
    } state;
};