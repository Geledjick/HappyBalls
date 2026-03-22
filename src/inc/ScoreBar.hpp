#pragma once

#include "Scene.hpp"
#include "AssetsManager.hpp"
#include "ColorSceme.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

class ScoreBar : public Scene {
public:
    ScoreBar(sf::Vector2u size, sf::Color backgroundColor = sf::Color::Black);

    void render(sf::RenderWindow *window) override;

    void updateScoreText();
    void updateComboText();

    void addScore(const int amount);
    void resetScore();

    void incCombo();
    void resetCombo();

    const int getScore() const;
    const int getCombo() const;

private:
    int score, combo;
    sf::Text scoreText, comboText;
};