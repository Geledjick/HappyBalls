#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

class Scene : public sf::Drawable, public sf::Transformable {
public:
    Scene(const sf::Vector2u size, const sf::Color backgroundColor = sf::Color::Black) : renderTexture(size), backgroundColor(backgroundColor) {
        renderTexture.setSmooth(true);
    }

    void virtual run(sf::RenderWindow *window) {}

    void virtual render(sf::RenderWindow *window) = 0;

    void virtual mouseClick(sf::RenderWindow *window) {};

    void virtual resize() {}

    void reRender() {
        neededRender = true;
    }

    const sf::Vector2u getSize() const {
        return renderTexture.getSize();
    }

    const bool getNeededRender() const {
        return neededRender;
    }

protected:
    sf::RenderTexture renderTexture;
    bool neededRender = true;
    sf::Color backgroundColor;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        states.texture = &renderTexture.getTexture();
        const sf::Sprite sprite(renderTexture.getTexture());
        target.draw(sprite, states);
    };
};