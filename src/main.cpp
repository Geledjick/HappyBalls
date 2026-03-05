#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <optional>

#include "Scene.cpp"
#include "Game.cpp"
#include "debug.cpp"
#include "Scaler.cpp"
#include "ColorSceme.hpp"

sf::Vector2u Scaler::startResolution = {};
sf::Vector2f Scaler::scaleMul = {};

int main() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode({900, 1000}), "HappyBalls", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    sf::Vector2u startResolution = window.getSize();
    Scaler(window.getSize());

    Game game(startResolution, {16, 16}, FIELD_COLOR);
    Scene *currentScene = &game;

    bool lastMouseLeftClickState = false;

    while (window.isOpen()) {
        std::optional<sf::Event> event;
        while ((event = window.pollEvent())) { 
            if (event->is<sf::Event::Resized>()) {
                currentScene->resize();

                Scaler::rescale(window.getSize());

                DEBUG_PRINTF("Resized | New resolution: W - %d, H - %d\n", window.getSize().x, window.getSize().y);

            } else if (event->is<sf::Event::Closed>()) {
                window.close();
                break;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !lastMouseLeftClickState) {
                lastMouseLeftClickState = true;
                
            } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && lastMouseLeftClickState) {
                currentScene->mouseClick(&window);
                lastMouseLeftClickState = false;
            }
        }

        currentScene->run();
        currentScene->render(&window);

        window.clear();
        window.draw(*currentScene);
        window.display();
    }

    return 0;
}