#include "inc/Button.hpp"
#include "inc/AssetsManager.hpp"
#include "inc/ColorSceme.hpp"

    Button::Button(const sf::Vector2u size, const sf::Color defaultColor, const sf::Color checkedColor, const std::string newText) :
        defaultColor(defaultColor),
        checkedColor(checkedColor),
        text(AssetsManager::getFont(), newText, size.y * 0.5f),
        Scene(size)
    {
        //text.setOrigin(text.getGlobalBounds().size * -0.5f);
        text.setPosition(getPosition() * 0.5f);
        text.move({10, text.getCharacterSize() * 0.5f});
        text.setFillColor(TEXT_COLOR);
    }

    void Button::mouseClick(sf::RenderWindow *window) {
        if (state == ButtonStates::CHECKED) {
            state = ACTIVED;
            reRender();
        }
    }

    void Button::run(sf::RenderWindow *window) {
        const sf::Vector2i mousePosition = Scaler::restoreV(sf::Mouse::getPosition(*window));
        const sf::Vector2f buttonPosition = getPosition();
        const sf::Vector2u buttonSize = renderTexture.getSize();

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

            renderTexture.draw(text);

            renderTexture.display();
        }        
    }

    const bool Button::isClicked() const {
        return state == ButtonStates::ACTIVED;
    }