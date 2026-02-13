#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 900}), "HappyBalls");

    while (window.isOpen()) {
        std::optional<sf::Event> event;

        while ((event = window.pollEvent())) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                break;
            }
        }

        window.clear();
        window.display();
    }

    return 0;
}