#include "Game.hpp"

void Game::redrawField() {
    fieldRenderTexture.clear(backgroundColor);
    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            tile.setPosition({(c * tileSize) + spaceOutlineSize, (r * tileSize) + spaceOutlineSize});
            fieldRenderTexture.draw(tile);

            ball.setPosition({tile.getPosition().x + ballCenterPosition, tile.getPosition().y + ballCenterPosition});
            ball.setFillColor(field[c][r].getColor());
            fieldRenderTexture.draw(ball);
        }
    }
    fieldRenderTexture.display();
    fieldSprite.setTexture(fieldRenderTexture.getTexture());
}

void Game::redrawScoreText() {
    scoreText.setString(std::to_string(score));
}

void Game::redrawCombo() {
    comboText.setString('x' + std::to_string(combo));
}

void Game::render() {
    window.clear(backgroundColor);

    window.draw(fieldSprite);

    window.draw(scoreText);
    window.draw(comboText);

    if (gameOver) {
        window.draw(gameOverText);
    }

    if (grabedBall.type != Ball::Type::None) {
        lastMousePosition = sf::Mouse::getPosition(window);
        grabedBallShape.setPosition({(float)lastMousePosition.x - ballCenterPosition, (float)lastMousePosition.y - ballCenterPosition});
        window.draw(grabedBallShape);
        window.draw(grabedRing);
    }

    window.display();
}