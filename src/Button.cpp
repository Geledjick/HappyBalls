#include "inc/Button.hpp"

    Button::Button(const sf::Vector2u size, const sf::Color defaultColor, const sf::Color checkedColor) :
        defaultColor(defaultColor),
        checkedColor(checkedColor),
        Scene(size)
    {}

    void Button::mouseClick(sf::RenderWindow *window) {
        if (state == ButtonStates::CHECKED) {
            state = ACTIVED;
            reRender();
        }
    }

    void Button::run(sf::RenderWindow *window) {
        const sf::Vector2i mousePosition = Scaler::scaleV(sf::Mouse::getPosition(*window));
        const sf::Vector2f buttonPosition = Scaler::scaleV<float>(getPosition());
        const sf::Vector2u buttonSize = Scaler::scaleV(renderTexture.getSize());

        ButtonStates lastState = state;

        if (
            mousePosition.x >= buttonPosition.x &&
            mousePosition.y >= buttonPosition.y &&
            mousePosition.x <= buttonPosition.x + buttonSize.x &&
            mousePosition.y <= buttonPosition.y + buttonSize.y
        ) {
            state = ButtonStates::CHECKED;
        } else {
            state = ButtonStates::DEFAULT;
        }

        if (lastState != state) {
            reRender();
        }
    }

    void Button::render(sf::RenderWindow *window) {
        if (neededRender) {
            neededRender = false;

            sf::Color targetColor;

            if (state == ButtonStates::DEFAULT) {
                targetColor = defaultColor;
            } else {
                targetColor = checkedColor;
            }

            renderTexture.clear(targetColor);

            renderTexture.display();
        }        
    }

    const bool Button::isClicked() const {
        return state == ButtonStates::ACTIVED;
    }