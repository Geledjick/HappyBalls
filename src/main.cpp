#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <optional>
#include <ctime>

#include "Scene.cpp"
#include "Game.cpp"

sf::Vector2f scale;

int main() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode({800, 900}), "HappyBalls", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);
    sf::Vector2u resolution = window.getSize();

    Game game(resolution, {14, 14});
    Scene *currentScene = &game;

    srand(time(NULL));

    while (window.isOpen()) {
        std::optional<sf::Event> event;
        while ((event = window.pollEvent())) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                break;
            } else if (event->is<sf::Event::Resized>()) {
                resolution = window.getSize();
            }
        }

        currentScene->run();
        currentScene->render();

        window.clear();
        window.draw(*currentScene);
        window.display();
    }

    return 0;
}