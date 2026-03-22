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

#include "Scene.hpp"
#include "Scaler.hpp"

class Button : public Scene {
public:
    Button(const sf::Vector2u size, const sf::Color defaultColor, const sf::Color checkedColor);

    void mouseClick(sf::RenderWindow *window) override;

    void run(sf::RenderWindow *window) override;

    void render(sf::RenderWindow *window) override;

    const bool isClicked() const;

protected:
    const sf::Color defaultColor;
    const sf::Color checkedColor;

    enum ButtonStates {
        DEFAULT,
        CHECKED,
        ACTIVED
    } state;
};