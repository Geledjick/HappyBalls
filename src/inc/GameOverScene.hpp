#pragma once

#include "Scene.hpp"
#include "Button.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "ColorSceme.hpp"

class GameOverScene : public Scene {
public:
    GameOverScene(sf::Vector2u size, sf::Color backgroundColor);

    void run(sf::RenderWindow *window) override;

    void render(sf::RenderWindow *window) override;

    void mouseClick(sf::RenderWindow *window) override;

private:
    sf::RectangleShape menuRect;

    Button restartButton;
    Button exitButton;
};