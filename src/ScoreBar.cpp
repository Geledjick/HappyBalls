#include "inc/ScoreBar.hpp"

ScoreBar::ScoreBar(sf::Vector2u size, sf::Color backgroundColor) :
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

    scoreText.setPosition({(size.x / FONT_SIZE) * 2, (size.y / 2.f) - (FONT_SIZE / 1.5f)});
    comboText.setPosition({size.x - (scoreText.getPosition().x * 6), scoreText.getPosition().y});

    updateScoreText();
    updateComboText();
}

void ScoreBar::render(sf::RenderWindow *window) {
    if (neededRender) {
        neededRender = false;
        renderTexture.clear(backgroundColor);

        renderTexture.draw(scoreText);
        renderTexture.draw(comboText);

        renderTexture.display();
    }
}

void ScoreBar::updateScoreText() {
    scoreText.setString(std::to_string(score));
    reRender();
}
void ScoreBar::updateComboText() {
    comboText.setString("x" + std::to_string(combo));
    reRender();
}

void ScoreBar::addScore(const int amount) {
    score += (amount * combo);
    updateScoreText();
}
void ScoreBar::resetScore() {
    score = 0;
    updateScoreText();
}

void ScoreBar::incCombo() {
    combo++;
    updateComboText();
}
void ScoreBar::resetCombo() {
    combo = 1;
    updateComboText();
}

const int ScoreBar::getScore() const {
    return score;
}
const int ScoreBar::getCombo() const {
    return combo;
}