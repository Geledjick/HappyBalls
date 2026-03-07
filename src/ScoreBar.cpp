#pragma once

#include "Scene.cpp"
#include "AssetsManager.cpp"
#include "ColorSceme.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

class ScoreBar : public Scene {
public:
    ScoreBar(sf::Vector2u size, sf::Color backgroundColor = sf::Color::Black) :
        Scene(size, backgroundColor),
        scoreText(AssetsManager::getFont()),
        comboText(AssetsManager::getFont()),
        score(0),
        combo(1)
    {
        scoreText.setCharacterSize(FONT_SIZE);
        comboText.setCharacterSize(FONT_SIZE);

        scoreText.setFillColor(TEXT_COLOR);
        comboText.setFillColor(TEXT_COLOR);

        scoreText.setPosition({FONT_SIZE, (size.y / 2.f) - (FONT_SIZE / 1.5f)});
        comboText.setPosition({size.x / 1.25f, scoreText.getPosition().y});

        updateScoreText();
        updateComboText();
    }

    void render(sf::RenderWindow *window) override {
        if (neededRender) {
            neededRender = false;
            renderTexture.clear(backgroundColor);

            renderTexture.draw(scoreText);
            renderTexture.draw(comboText);

            renderTexture.display();
        }
    }

    void updateScoreText() {
        scoreText.setString(std::to_string(score));
        reRender();
    }
    void updateComboText() {
        comboText.setString("x" + std::to_string(combo));
        reRender();
    }

    void addScore(const int amount) {
        score += (amount * combo);
        updateScoreText();
    }
    void resetScore() {
        score = 0;
        updateScoreText();
    }

    void incCombo() {
        combo++;
        updateComboText();
    }
    void resetCombo() {
        combo = 1;
        updateComboText();
    }

    const int getScore() const {
        return score;
    }
    const int getCombo() const {
        return combo;
    }

private:
    int score, combo;
    sf::Text scoreText, comboText;
};