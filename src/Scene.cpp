#include "inc/Scene.hpp"

Scene::Scene(const sf::Vector2u size, const sf::Color backgroundColor) :
    renderTexture(size),
    backgroundColor(backgroundColor),
    neededRender(true)
{
    renderTexture.setSmooth(true);
}

void Scene::run(sf::RenderWindow *window) {}
void Scene::mouseClick(sf::RenderWindow *window) {}

void Scene::resize() {};

void Scene::reRender() {
    neededRender = true;
}

const sf::Vector2u Scene::getSize() const {
    return renderTexture.getSize();
}

const bool Scene::getNeededRender() const {
    return neededRender;
}

void Scene::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &renderTexture.getTexture();
    const sf::Sprite sprite(renderTexture.getTexture());
    target.draw(sprite, states);
};