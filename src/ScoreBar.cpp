#pragma once

#include "Scene.cpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

class ScoreBar : public Scene {
public:
    ScoreBar(sf::Vector2u size, sf::Color backgroundColor = sf::Color::Black) : Scene(size, backgroundColor) {
    
    }

    void render(sf::RenderWindow *window) override {
        if (neededRender) {
            neededRender = false;
            renderTexture.clear(backgroundColor);

            renderTexture.display();
        }
    }

private:
};