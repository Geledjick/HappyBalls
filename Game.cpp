#include "Game.hpp"

sf::Color Ball::getColor() {
    switch (type) {
        case Red: return sf::Color::Red;
        case Green: return sf::Color::Green;
        case Blue: return sf::Color::Blue;
        case Yellow: return sf::Color::Yellow;
        default: return sf::Color::Transparent;
    }
}

Game::Game() : 
    window(sf::VideoMode({808, 900}), "HappyBalls", sf::Style::Close | sf::Style::Titlebar), 
    gameOverText(font),
    scoreText(font),
    comboText(font),
    fieldRenderTexture(window.getSize()),
    fieldSprite(fieldRenderTexture.getTexture())
{
    window.setFramerateLimit(60);
    sf::Vector2u resolution = window.getSize();
    window.setPosition({(int)resolution.x, (int)resolution.y / 2});        
    
    srand(time(NULL));

    fieldSprite.setPosition({0, 0});

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

void Game::resizeWindow() {
    // puts("Resized!");
    sf::Vector2u resolution = window.getSize();
    int rs = (resolution.x + spaceOutlineSize) / rows;
    int cs = (resolution.y + spaceOutlineSize) / cols;

    bool err = fieldRenderTexture.resize(resolution);

    tileSize = (rs > cs ? cs : rs);
    ballSize = tileSize * 0.33f;
    ballCenterPosition = ballSize / 2.f;

    ball.setRadius(ballSize);
    grabedBallShape.setRadius(ballSize);
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