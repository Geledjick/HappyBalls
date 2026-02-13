#include "Game.hpp"

void Game::run() {
    generateBalls();
    bool checked = false;
    redrawField();
    while (window.isOpen()) {
        handleEvents();
        render();

        checked = checkField();
        if (!gameOver && ballIsTransfer) {
            ballIsTransfer = false;
            if(!checked) {
                generateBalls();
                combo = 1;
            }
        }
    }
}

bool Game::checkField() {
    if (
        checkVertical() ||
        checkHorizont() ||
        checkDiagonalDownRight() ||
        checkDiagonalUpRight()
    ) {
        redrawField();
        redrawCombo();
        redrawScoreText();
        return true;
    } else {
        return false;
    }
}

void Game::handleEvents() {
    while ((event = window.pollEvent())) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            return;
        }
        
        //if (event->is<sf::Event::Resized>()) {
        //    resizeWindow();
        //}

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !lastMouseClickState && !gameOver) {
            lastMouseClickState = true;
        } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && lastMouseClickState && !gameOver) {
            lastMouseClickState = false;
            mouseClick();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            resetGame();
        }
    }
}

bool Game::findPath(sf::Vector2i targetPosition) {
    std::queue<sf::Vector2i> queue;
    queue.push(lastGrabedMousePosition);

    std::vector<std::vector<bool>> visited(cols, std::vector<bool>(rows, false));
    visited[lastGrabedMousePosition.x][lastGrabedMousePosition.y] = true;
    
    while (!queue.empty()) {
        sf::Vector2i vec = queue.front();
        queue.pop();

        if (vec.x == targetPosition.x && vec.y == targetPosition.y) {
            return true;
        }

        sf::Vector2i nvec;
        for (int i = 0; i < 4; i++) {
            nvec = {vec.x + directions[i].x, vec.y + directions[i].y};

            if (
                (nvec.x >= 0) && (nvec.y >= 0) &&
                (nvec.x < cols) && (nvec.y < rows) &&
                field[nvec.x][nvec.y].type ==  Ball::Type::None &&
                !visited[nvec.x][nvec.y]
            ) {
                visited[nvec.x][nvec.y] = true;
                queue.push(nvec);
            }
        }
    }
    return false;
}

void Game::generateBalls() {
    sf::Vector2i pos;
    for (int i = 0; i < countGenerateBalls; i++) {
        int j = 0;
        do {
            pos.x = rand() % cols;
            pos.y = rand() % rows;
            j++;
            if(j > maxGeneratorIterations) {
                gameOver = true;
                return;
            }
        } while (field[pos.x][pos.y].type != Ball::Type::None);

        field[pos.x][pos.y].type = Ball::Type((rand() % (Ball::Type::TypeCount - 1)) + 1);
    }
}

void Game::resetGame() {
    score = 0;
    combo = 1;
    gameOver = false;
    for (std::array<Ball, rows> &row : field) {
        for (Ball &ball : row) {
            ball.type = Ball::Type::None;
        }
    }
    grabedBall.type = Ball::Type::None;
    generateBalls();
    redrawField();
    redrawCombo();
    redrawScoreText();
}

void Game::mouseClick() {
    lastMousePosition = sf::Mouse::getPosition(window);

    sf::Vector2i targetPosition;
    targetPosition.x = (lastMousePosition.x - spaceOutlineSize) / tileSize;
    targetPosition.y = (lastMousePosition.y - spaceOutlineSize) / tileSize;

    if ( 
        ((targetPosition.x >= 0) && (targetPosition.y >= 0)) &&
        ((targetPosition.x < cols) && (targetPosition.y < rows)) &&
        (grabedBall.type == Ball::Type::None) &&
        (field[targetPosition.x][targetPosition.y].type != Ball::Type::None)
    ) {
        //puts("Grab!");
        grabedBall.type = field[targetPosition.x][targetPosition.y].type;
        field[targetPosition.x][targetPosition.y].type = Ball::Type::None;
        window.setMouseCursorVisible(false);

        grabedRing.setPosition({
            (targetPosition.x * tileSize) + ballCenterPosition + spaceOutlineSize,
            (targetPosition.y * tileSize) + ballCenterPosition + spaceOutlineSize,
        });
        grabedRing.setOutlineColor(grabedBall.getColor());
        grabedBallShape.setFillColor(grabedBall.getColor());
        redrawField();
        lastGrabedMousePosition = targetPosition;
    } else if (
        ((targetPosition.x >= 0) && (targetPosition.y >= 0)) &&
        ((targetPosition.x < cols) && (targetPosition.y < rows)) &&
        (grabedBall.type != Ball::Type::None) &&
        (field[targetPosition.x][targetPosition.y].type == Ball::Type::None) &&
        (findPath(targetPosition))
    ) {
        //puts("Put!");
        field[targetPosition.x][targetPosition.y].type = grabedBall.type;
        grabedBall.type = Ball::Type::None;
        window.setMouseCursorVisible(true);
        
        if (lastGrabedMousePosition != targetPosition) {
            ballIsTransfer = true;
        }
        redrawField();
    }
    lastTargetPosition = targetPosition;
}