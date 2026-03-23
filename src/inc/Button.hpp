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
#include <string>

#include "Scene.hpp"
#include "Scaler.hpp"
#include "AssetsManager.hpp"
#include "ColorSceme.hpp"

class Button : public Scene {
public:
    Button(const sf::Vector2u size, const sf::Color defaultColor, const sf::Color checkedColor, const std::string newText);

    void mouseClick(sf::RenderWindow *window) override;

    void run(sf::RenderWindow *window) override;

    void render(sf::RenderWindow *window) override;

    const bool isClicked() const;

protected:
    const sf::Color defaultColor;
    const sf::Color checkedColor;

    sf::Text text;
 
    enum ButtonStates {
        DEFAULT,
        CHECKED,
        ACTIVED
    } state;
};