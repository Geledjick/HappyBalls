#pragma once

#include "Scene.cpp"
#include "Field.cpp"
#include <SFML/System/Vector2.hpp>

class Game : public Scene {
public:
    Game(sf::Vector2u size, sf::Vector2u fieldSize, sf::Color backgroundColor = sf::Color::Black) : Scene(size, backgroundColor), field(size - sf::Vector2u(0, size.y / fieldSize.y), fieldSize, {0, 0}) {}

    void run() override {
        field.run();
    }

    void render() override {
        field.render();
        if (neededRender) {
            neededRender = false;
            renderTexture.clear(backgroundColor);
            
            renderTexture.draw(field);

            renderTexture.display();
        }
    }

private:
    Field field;
};