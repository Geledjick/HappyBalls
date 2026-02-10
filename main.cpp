// SFML
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Other
#include <array>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <queue>
#include <stdio.h>
#include <string>
#include <vector>

struct Ball {
    enum Type {
        None,
        Red,
        Green,
        Blue,
        Yellow,
        TypeCount
    } type;

    sf::Color getColor() {
        switch (type) {
            case Red: return sf::Color::Red;
            case Green: return sf::Color::Green;
            case Blue: return sf::Color::Blue;
            case Yellow: return sf::Color::Yellow;
            default: return sf::Color::Transparent;
        }
    }
};

class Game {
public:
    Game() : 
        window(sf::VideoMode({808, 900}), "HappyBalls", sf::Style::Close | sf::Style::Titlebar), 
        gameOverText(font),
        scoreText(font),
        comboText(font)
    {
        window.setFramerateLimit(60);
        sf::Vector2u resolution = window.getSize();
        window.setPosition({(int)resolution.x, (int)resolution.y / 2});        
        
        srand(time(NULL));

        font.setSmooth(true);
        if (!font.openFromFile("fonts/Hack-Regular.ttf")) {
            exit(1);
        }

        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(48);
        gameOverText.setFillColor(sf::Color::Red);

        scoreText.setCharacterSize(48);
        scoreText.setFillColor(sf::Color(60, 62, 80));

        comboText.setCharacterSize(48);
        comboText.setFillColor(sf::Color(60, 62, 80));

        for (std::array<Ball, rows> &row : field) {
            for (Ball &ball : row) {
                ball.type = Ball::Type::None;
            }
        }

        directions[0] = {0, 1};
        directions[1] = {0, -1};
        directions[2] = {1, 0};
        directions[3] = {-1, 0};

        grabedRing.setOutlineThickness(5);
        grabedRing.setFillColor(sf::Color::Transparent);

        tile.setFillColor(sf::Color(50, 52, 70));
        tile.setOutlineColor(sf::Color(68, 71, 90));
        tile.setOutlineThickness(outlineSize);

        resizeWindow();
    }

    void run() {
        generateBalls();
        bool checked = false;
        while (window.isOpen()) {
            handleEvents();
            render();

            checked = (
                checkVertical() ||
                checkHorizont() ||
                checkDiagonalDownRight() ||
                checkDiagonalUpRight()
            );
            if (!gameOver && ballIsTransfer) {
                ballIsTransfer = false;
                if(!checked) {
                    generateBalls();
                    combo = 1;
                }
            }
        }        
    }

private:
    sf::RenderWindow window;

    static const int cols = 14;
    static const int rows = 14;

    std::array<std::array<Ball, rows>, cols> field;

    float tileSize;
    float ballSize;
    float ballCenterPosition;
    sf::RectangleShape tile;
    sf::CircleShape ball;
    sf::CircleShape grabedRing;
    const int outlineSize = 2.f;
    const int spaceOutlineSize = outlineSize * 2;

    const sf::Color backgroundColor = sf::Color(40, 42, 54);

    const int countGenerateBalls = (rows + cols);
    const int maxGeneratorIterations = rows * cols * 0.5f;

    int score = 0;
    bool gameOver = false;
    std::optional<sf::Event> event;
    int combo = 1;

    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text comboText;

    sf::Vector2i lastMousePosition;
    sf::Vector2i lastTargetPosition;
    sf::Vector2i lastGrabedMousePosition;
    bool lastMouseClickState = false;
    Ball grabedBall{Ball::Type::None};

    bool ballIsTransfer = false;
    std::array<sf::Vector2i, 4> directions;

    void handleEvents() {
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

    void resizeWindow() {
        // puts("Resized!");
        sf::Vector2u resolution = window.getSize();
        int rs = (resolution.x - spaceOutlineSize * 2) / rows;
        int cs = (resolution.y - spaceOutlineSize * 2) / cols;
        tileSize = (rs > cs ? cs : rs);
        ballSize = tileSize * 0.33f;
        ballCenterPosition = ballSize / 2.f;

        ball.setRadius(ballSize);
        grabedRing.setRadius(ballSize);
        tile.setSize({tileSize, tileSize});

        gameOverText.setPosition({
            float((resolution.x * 0.5f) -
            (gameOverText.getCharacterSize() *
            float(gameOverText.getString().getSize()) * 0.33f)),
            float(resolution.y - gameOverText.getCharacterSize() * 1.75),
        });

        scoreText.setPosition({
            float(scoreText.getCharacterSize() * 0.25f),
            float(resolution.y - scoreText.getCharacterSize() * 1.75),
        });

        comboText.setPosition({
            float(resolution.x - comboText.getCharacterSize() * 2),
            float(resolution.y - comboText.getCharacterSize() * 1.75),
        });
    }

    void render() {
        window.clear(backgroundColor);

        for (int c = 0; c < cols; c++) {
            for (int r = 0; r < rows; r++) {
                tile.setPosition({(c * tileSize) + spaceOutlineSize, (r * tileSize) + spaceOutlineSize});
                window.draw(tile);

                ball.setPosition({tile.getPosition().x + ballCenterPosition, tile.getPosition().y + ballCenterPosition});
                ball.setFillColor(field[c][r].getColor());
                window.draw(ball);
            }
        }

        scoreText.setString(std::to_string(score));
        comboText.setString('x' + std::to_string(combo));
        window.draw(scoreText);
        window.draw(comboText);

        if (gameOver) {
            window.draw(gameOverText);
        }

        if (grabedBall.type != Ball::Type::None) {
            lastMousePosition = sf::Mouse::getPosition(window);
            ball.setPosition({(float)lastMousePosition.x - ballCenterPosition, (float)lastMousePosition.y - ballCenterPosition});
            ball.setFillColor(grabedBall.getColor());
            window.draw(ball);
            window.draw(grabedRing);
        }

        window.display();
    }

    void mouseClick() {
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
        }
        lastTargetPosition = targetPosition;
    }

    bool findPath(sf::Vector2i targetPosition) {
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

    void generateBalls() {
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

    void resetGame() {
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
    }

    bool checkVertical() {
        for (int c = 0; c < cols; c++) {
            for (int r = 0; r < rows; r++) {
                if (
                    (field[c][r].type != Ball::Type::None) &&
                    (r + 1 < rows && r + 2 < rows)
                ) {
                    Ball::Type targetType = field[c][r].type;
                    if (targetType == field[c][r + 1].type && targetType == field[c][r + 2].type) {
                        int len = 3;
                        field[c][r].type = Ball::Type::None;
                        field[c][r + 1].type = Ball::Type::None;
                        field[c][r + 2].type = Ball::Type::None;
                        while (
                            (r + len < rows) &&
                            (targetType == field[c][r + len].type)
                        ) {
                            field[c][r + len].type = Ball::Type::None;
                            len++;
                        }
                        r += len;
                        score += len * combo;
                        combo++;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool checkHorizont() {
        for (int c = 0; c < cols; c++) {
            for (int r = 0; r < rows; r++) {
                if (
                    (field[c][r].type != Ball::Type::None) &&
                    (c + 1 < cols && c + 2 < cols)
                ) {
                    Ball::Type targetType = field[c][r].type;
                    if (targetType == field[c + 1][r].type && targetType == field[c + 2][r].type) {
                        int len = 3;
                        field[c][r].type = Ball::Type::None;
                        field[c + 1][r].type = Ball::Type::None;
                        field[c + 2][r].type = Ball::Type::None;
                        while (
                            (c + len < cols) &&
                            (targetType == field[c + len][r].type)
                        ) {
                            field[c + len][r].type = Ball::Type::None;
                            len++;
                        }
                        r += len;
                        score += len * combo;
                        combo++;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool checkDiagonalDownRight() {
        for (int c = 0; c < cols; c++) {
            for (int r = 0; r < rows; r++) {
                if (
                    (field[c][r].type != Ball::Type::None) &&
                    (c + 1 < cols && c + 2 < cols) &&
                    (r + 1 < rows && r + 2 < rows)
                ) {
                    Ball::Type targetType = field[c][r].type;
                    if (targetType == field[c + 1][r + 1].type && targetType == field[c + 2][r + 2].type) {
                        int len = 3;
                        field[c][r].type = Ball::Type::None;
                        field[c + 1][r + 1].type = Ball::Type::None;
                        field[c + 2][r + 2].type = Ball::Type::None;
                        while (
                            (c + len < cols) &&
                            (r + len < rows) &&
                            (targetType == field[c + len][r + len].type)
                        ) {
                            field[c + len][r + len].type = Ball::Type::None;
                            len++;
                        }
                        r += len;
                        score += len * combo;
                        combo++;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool checkDiagonalUpRight() {
        for (int c = 0; c < cols; c++) {
            for (int r = 0; r < rows; r++) {
                if (
                    (field[c][r].type != Ball::Type::None) &&
                    (c + 1 < cols && c + 2 < cols) &&
                    (r - 1 >= 0 && r - 2 >= 0)
                ) {
                    Ball::Type targetType = field[c][r].type;
                    if (targetType == field[c + 1][r - 1].type && targetType == field[c + 2][r - 2].type) {
                        int len = 3;
                        field[c][r].type = Ball::Type::None;
                        field[c + 1][r - 1].type = Ball::Type::None;
                        field[c + 2][r - 2].type = Ball::Type::None;
                        while (
                            (c + len < cols) &&
                            (r - len >= 0) &&
                            (targetType == field[c + len][r - len].type)
                        ) {
                            field[c + len][r - len].type = Ball::Type::None;
                            len++;
                        }
                        r += len;
                        score += len * combo;
                        combo++;
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

int main() {
    Game *game = new Game();
    game->run();
    delete game;
    return 0;
}