// SFML
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Other
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <optional>
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
        window(sf::VideoMode({800, 900}), "HappyBalls", sf::Style::Close | sf::Style::Titlebar), 
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
        gameOverText.setPosition({
            float((resolution.x * 0.5f) - (gameOverText.getCharacterSize() * float(gameOverText.getString().getSize()) * 0.33f)), 
            float(resolution.y - gameOverText.getCharacterSize() * 1.75),
        });

        scoreText.setCharacterSize(48);
        scoreText.setFillColor(sf::Color(60, 62, 80));
        scoreText.setPosition({
            float(scoreText.getCharacterSize() * 0.25f),
            float(resolution.y - scoreText.getCharacterSize() * 1.75),
        });

        comboText.setCharacterSize(48);
        comboText.setFillColor(sf::Color(60, 62, 80));
        comboText.setPosition({
            float(resolution.x - comboText.getCharacterSize() * 2),
            float(resolution.y - comboText.getCharacterSize() * 1.75),
        });

        field.resize(cols);
        for (std::vector<Ball> &row : field) {
            row.resize(rows);
            for (Ball &ball : row) {
                ball.type = Ball::Type::None;
            }
        }

        int rs = resolution.x / rows;
        int cs = resolution.y / cols;
        tileSize = (rs > cs ? cs : rs);
        ballSize = tileSize * 0.33f;
        ballCenterPosition = ballSize / 2.f;
        ball.setRadius(ballSize);

        grabedRing.setRadius(ballSize);
        grabedRing.setOutlineThickness(5);
        grabedRing.setFillColor(sf::Color::Transparent);

        tile.setFillColor(sf::Color(50, 52, 70));
        tile.setSize({tileSize, tileSize});
        tile.setOutlineColor(sf::Color(68, 71, 90));
        tile.setOutlineThickness(outlineSize);
    }

    void run() {
        generateBalls();
        bool checked = false;
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

private:
    sf::RenderWindow window;

    std::vector<std::vector<Ball>> field;

    const int rows = 14;
    const int cols = 14;

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

    void handleEvents() {
        while ((event = window.pollEvent())) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }

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
            ball.setPosition({(float)lastMousePosition.x, (float)lastMousePosition.y});
            ball.setFillColor(grabedBall.getColor());
            window.draw(ball);
            window.draw(grabedRing);
        }

        window.display();
    }

    void mouseClick() {
        lastMousePosition = sf::Mouse::getPosition(window);

        sf::Vector2i targetPosition;
        targetPosition.x = lastMousePosition.x / tileSize;
        targetPosition.y = lastMousePosition.y / tileSize;

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
            (field[targetPosition.x][targetPosition.y].type == Ball::Type::None)
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
        gameOver = false;
        for (std::vector<Ball> &row : field) {
            for (Ball &ball : row) {
                ball.type = Ball::Type::None;
            }
        }
        grabedBall.type = Ball::Type::None;
        generateBalls();
    }

    bool checkField() {
        return (
            checkVertical() ||
            checkHorizont() ||
            checkDiagonalDownRight() ||
            checkDiagonalUpRight()
        );
    }

    bool checkVertical() {
        bool returned = false;
        for (int c = 0; c < cols; c++) {
            for (int r = 0; r < rows; r++) {
                if (
                    (field[c][r].type != Ball::Type::None) &&
                    (r + 1 < rows && r + 2 < rows)
                ) {
                    Ball::Type targetType = field[c][r].type;
                    if (targetType == field[c][r + 1].type && targetType == field[c][r + 2].type) {
                        int len = 3;
                        returned = true;
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
                    }
                }
            }
        }
        return returned;
    }

    bool checkHorizont() {
        bool returned = false;
        for (int c = 0; c < cols; c++) {
            for (int r = 0; r < rows; r++) {
                if (
                    (field[c][r].type != Ball::Type::None) &&
                    (c + 1 < cols && c + 2 < cols)
                ) {
                    Ball::Type targetType = field[c][r].type;
                    if (targetType == field[c + 1][r].type && targetType == field[c + 2][r].type) {
                        int len = 3;
                        returned = true;
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
                    }
                }
            }
        }
        return returned;
    }

    bool checkDiagonalDownRight() {
        bool returned = false;
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
                        returned = true;
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
                    }
                }
            }
        }
        return returned;
    }

    bool checkDiagonalUpRight() {
        bool returned = false;
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
                        returned = true;
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
                    }
                }
            }
        }
        return returned;
    }
};

int main() {
    Game *game = new Game();
    game->run();
    delete game;
    return 0;
}