#pragma once

#include "Scene.cpp"
#include <SFML/System/Vector2.hpp>

class Menu : public Scene {
public:
    Menu(sf::Vector2u size, sf::Color backgroundColor = sf::Color::Black) : Scene(size, backgroundColor) {}

    void run() override {

    }

    void render() override {
        if (neededRender) {
            neededRender = false;
            renderTexture.clear(backgroundColor);
            


            renderTexture.display();
        }
    }

private:
    
};