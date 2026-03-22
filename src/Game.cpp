#include "inc/Game.hpp"

Game::Game(sf::Vector2u size, 
           sf::Vector2u fieldSize, 
           sf::Color backgroundColor) : 
    Scene(size, backgroundColor), 
    field(size - sf::Vector2u(0, size.y * SCORE_BAR_SIZE_PERCENTAGE), 
          fieldSize, 
          FIELD_COLOR),
    scoreBar({size.x, uint32_t(size.y * SCORE_BAR_SIZE_PERCENTAGE)}, 
             FIELD_COLOR),
    gameOverScene(size, BLUR_COLOR)
{
    scoreBar.setPosition({0, size.y - size.y * SCORE_BAR_SIZE_PERCENTAGE});
    gameOverScene.setPosition({0, 0});

    grabedBall = BALL_NONE;

    grabedBallShape.setRadius(field.getBallSize());
    grabedBallShapeRing.setRadius(field.getBallSize());

    grabedBallShapeRing.setFillColor(sf::Color::Transparent);
    grabedBallShapeRing.setOutlineThickness(4);
}

const bool Game::checkField() {
    Matrix2<Ball> &data = field.getData();

    bool checked = false;
    for (unsigned int r = 0; r < data.getRows(); r++) {
        for (unsigned int c = 0; c < data.getCols(); c++) {
            Ball &targetBall = data.at(r, c);
            if (targetBall.type == BALL_NONE_TYPE) {
                continue;
            }
            
            if (
                data.check(r + 1, c) &&
                data.check(r + 2, c) &&
                targetBall.type == data.at(r + 1, c).type &&
                targetBall.type == data.at(r + 2, c).type
            ) {
                checked = true;
                data.at(r + 1, c).type = BALL_NONE_TYPE;
                data.at(r + 2, c).type = BALL_NONE_TYPE;

                int len = 3;
                while (
                    data.check(r + len, c) &&
                    targetBall.type == data.at(r + len, c).type
                ) {
                    data.at(r + len, c).type = BALL_NONE_TYPE;
                    len++;
                }

                targetBall.type = BALL_NONE_TYPE;
                scoreBar.addScore(len);
                scoreBar.incCombo();

            }
            
            else if (
                data.check(r, c + 1) &&
                data.check(r, c + 2) &&
                targetBall.type == data.at(r, c + 1).type &&
                targetBall.type == data.at(r, c + 2).type
            ) {
                checked = true;
                data.at(r, c + 1).type = BALL_NONE_TYPE;
                data.at(r, c + 2).type = BALL_NONE_TYPE;

                int len = 3;
                while (
                    data.check(r, c + len) &&
                    targetBall.type == data.at(r, c + len).type
                ) {
                    data.at(r, c + len).type = BALL_NONE_TYPE;
                    len++;
                }

                targetBall.type = BALL_NONE_TYPE;
                scoreBar.addScore(len);
                scoreBar.incCombo();

            } 

            else if (
                data.check(r + 1, c + 1) &&
                data.check(r + 2, c + 2) &&
                targetBall.type == data.at(r + 1, c + 1).type &&
                targetBall.type == data.at(r + 2, c + 2).type
            ) {
                checked = true;
                data.at(r + 1, c + 1).type = BALL_NONE_TYPE;
                data.at(r + 2, c + 2).type = BALL_NONE_TYPE;

                int len = 3;
                while (
                    data.check(r + len, c + len) &&
                    targetBall.type == data.at(r + len, c + len).type
                ) {
                    data.at(r + len, c + len).type = BALL_NONE_TYPE;
                    len++;
                }

                targetBall.type = BALL_NONE_TYPE;
                scoreBar.addScore(len);
                scoreBar.incCombo();

            } 

            else if (
                data.check(r - 1, c + 1) &&
                data.check(r - 2, c + 2) &&
                targetBall.type == data.at(r - 1, c + 1).type &&
                targetBall.type == data.at(r - 2, c + 2).type
            ) {
                checked = true;
                data.at(r - 1, c + 1).type = BALL_NONE_TYPE;
                data.at(r - 2, c + 2).type = BALL_NONE_TYPE;

                int len = 3;
                while (
                    data.check(r - len, c + len) &&
                    targetBall.type == data.at(r - len, c + len).type
                ) {
                    data.at(r - len, c + len).type = BALL_NONE_TYPE;
                    len++;
                }

                targetBall.type = BALL_NONE_TYPE;
                scoreBar.addScore(len);
                scoreBar.incCombo();
            }
        }
    }

    lastChecked = checked;
    return checked;
}

void Game::run(sf::RenderWindow *window) {
    if (gameOver) {
        gameOverScene.run(window);
        if (gameOverScene.getNeededRender()) {
            reRender();
        }
    } else {
        field.run(window);
        scoreBar.run(window);

        if (!lastChecked && ballPlaced) {
            gameOver = field.generate();
            checkField();

            scoreBar.resetCombo();

            ballPlaced = false;
        }
    }
}

void Game::mouseClick(sf::RenderWindow *window) {
    if (gameOver) {
        gameOverScene.mouseClick(window);
        return;
    }

    const sf::Vector2i clickPosition = sf::Mouse::getPosition(*window);

    const sf::Vector2i targetPosition = {
        int(clickPosition.x / field.getTileLogicWidth()),
        int(clickPosition.y / field.getTileLogicHeight())
    };

    DEBUG_PRINTF("Click on: X - %d, Y - %d\n", targetPosition.x, targetPosition.y);

    if (!field.getData().check(targetPosition)) {
        return;
    }

    const Ball &targetBall = field.getData().at(targetPosition);

    if (grabedBall.type == BALL_NONE_TYPE && targetBall.type != BALL_NONE_TYPE) {
        window->setMouseCursorVisible(false);

        grabedBall = targetBall;
        
        grabedBallShapeRing.setPosition({
            targetPosition.x * Scaler::restoreX(field.getTileLogicWidth()) + 
            Scaler::restoreX(field.getBallLogicOffsetWidth()),
            targetPosition.y * Scaler::restoreY(field.getTileLogicHeight()) + 
            Scaler::restoreY(field.getBallLogicOffsetHeight())
        });

        const sf::Color targetColor = targetBall.getColor();

        grabedBallShapeRing.setOutlineColor(targetColor);
        grabedBallShape.setFillColor(targetColor);

        field.getData().at(targetPosition) = BALL_NONE;

        lastGrabedBallPostition = targetPosition;

        DEBUG_PUTS("GRAB!");

    } 

    else if (grabedBall.type != BALL_NONE_TYPE && targetBall.type == BALL_NONE_TYPE) {
        window->setMouseCursorVisible(true);

        field.getData().at(targetPosition) = grabedBall;
        grabedBall = BALL_NONE;

        ballPlaced = lastGrabedBallPostition != targetPosition;

        checkField();

        DEBUG_PUTS("PUT!");
    }

    field.reRender();
    reRender();
}


void Game::resize() {
    field.resize();
    scoreBar.resize();
    gameOverScene.resize();
}

void Game::render(sf::RenderWindow *window) {
    field.render(window);
    scoreBar.render(window);

    if (gameOver) {
        gameOverScene.render(window);
    }

    if (neededRender) {
        neededRender = false;
        renderTexture.clear(backgroundColor);
        
        renderTexture.draw(field);
        renderTexture.draw(scoreBar);

        if (grabedBall.type != BALL_NONE_TYPE) {
            const sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

            grabedBallShape.setPosition({
                Scaler::restoreX(mousePosition.x) - 
                Scaler::restoreX(field.getBallLogicOffsetWidth() * 1.5f),
                Scaler::restoreY(mousePosition.y) - 
                Scaler::restoreY(field.getBallLogicOffsetHeight() * 1.5f)
            });

            renderTexture.draw(grabedBallShape);
            renderTexture.draw(grabedBallShapeRing);

            reRender();
        }

        if (gameOver) {
            renderTexture.draw(gameOverScene);
        }

        renderTexture.display();
    }
}

const Field &Game::getField() const {
    return field;
}

Field &Game::getField() {
    return field;
}

const ScoreBar &Game::getScoreBar() const {
    return scoreBar;
}

ScoreBar &Game::getScoreBar() {
    return scoreBar;
}

const GameOverScene &Game::getGameOverScene() const {
    return gameOverScene;
}

GameOverScene &Game::getGameOverScene() {
    return gameOverScene;
}