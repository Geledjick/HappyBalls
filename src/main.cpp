#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <optional>
#include <ctime>

#include "Scene.cpp"
#include "Game.cpp"
#include "debug.cpp"

sf::Vector2f scale;

int main() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode({900, 1000}), "HappyBalls", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);
    sf::Vector2u resolution = window.getSize();

    Game game(resolution, {14, 14});
    Scene *currentScene = &game;

    srand(time(NULL));

    bool lastMouseLeftClickState = false;

    while (window.isOpen()) {
        std::optional<sf::Event> event;
        while ((event = window.pollEvent())) { 
            if (event->is<sf::Event::Resized>()) {
                currentScene->resize(resolution, window.getSize());
                resolution = window.getSize();
                DEBUG_PRINTF("Resized | New resolution: W - %d, H - %d\n", resolution.x, resolution.y);

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