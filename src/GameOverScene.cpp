#include "inc/GameOverScene.hpp"

GameOverScene::GameOverScene(sf::Vector2u size, sf::Color backgroundColor) :
    Scene(size, backgroundColor),
    menuRect({size.x / 2.f, size.y * 0.33f}),
    restartButton(
        {(unsigned int)(menuRect.getSize().x * 0.75f), (unsigned int)(menuRect.getSize().y * 0.2f)},
        sf::Color::White,
        sf::Color::Red,
        "RESTART"
    ),
    exitButton(
        {(unsigned int)(menuRect.getSize().x * 0.75f), (unsigned int)(menuRect.getSize().y * 0.2f)},
        sf::Color::White,
        sf::Color::Red,
        "EXIT"
    )
{
    menuRect.setPosition({
        (getSize().x / 2.f) - (menuRect.getSize().x / 2.f),
        (getSize().y / 2.f) - (menuRect.getSize().y / 2.f),
    });

    menuRect.setFillColor(MENU_COLOR);

    restartButton.setPosition({
        menuRect.getPosition().x + ((menuRect.getSize().x / 2.f) - (restartButton.getSize().x / 2.f)),
        menuRect.getPosition().y + ((menuRect.getSize().y / 4.f) - (restartButton.getSize().y / 2.f))
    });

    exitButton.setPosition({
        menuRect.getPosition().x + ((menuRect.getSize().x / 2.f) - (exitButton.getSize().x / 2.f)),
        menuRect.getPosition().y + ((menuRect.getSize().y / 2.f) - (exitButton.getSize().y / 2.f))
    });
}

void GameOverScene::run(sf::RenderWindow *window) {
    if (restartButton.isClicked()) {

    } else if (exitButton.isClicked()) {

    } 

    restartButton.run(window);
    exitButton.run(window);   

    if (restartButton.getNeededRender() || exitButton.getNeededRender()) {
        reRender();
    } 
};

void GameOverScene::render(sf::RenderWindow *window) {
    restartButton.render(window);
    exitButton.render(window);
    if (neededRender) {
        neededRender = false;
        renderTexture.clear(backgroundColor);
        
        renderTexture.draw(menuRect);

        renderTexture.draw(restartButton);
        renderTexture.draw(exitButton);

        renderTexture.display();
    }
}

void GameOverScene::mouseClick(sf::RenderWindow *window) {
    restartButton.mouseClick(window);
    exitButton.mouseClick(window);

    if (exitButton.getNeededRender() || restartButton.getNeededRender()) {
        reRender();
    }
}