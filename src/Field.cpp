#include "inc/Field.hpp"

Field::Field(sf::Vector2u size, const sf::Vector2u fieldSize, const sf::Color backgroundColor) : 
    Scene(size, backgroundColor),
    data(fieldSize.x, fieldSize.y), 
    maxGenerateIterations((fieldSize.x + fieldSize.y) / 4), 
    countGenerateInIteration((fieldSize.x + fieldSize.y))
{
    tileWidth = (float)size.x / fieldSize.x;
    tileHeight = (float)size.y / fieldSize.y;

    tileLogicWidth = tileWidth;
    tileLogicHeight = tileHeight;

    ballLogicOffset = {
        (tileLogicWidth * 0.33f) / 2,
        (tileLogicHeight * 0.33f) / 2,
    };

    ballSize = (tileHeight > tileWidth ? tileWidth : tileHeight) * 0.33f;
    ball.setRadius(ballSize);
    ballOffset = ballSize / 2;

    tile.setSize({tileWidth, tileHeight});
    tile.setFillColor(CELL_COLOR);
    tile.setOutlineColor(FIELD_COLOR);
    tile.setOutlineThickness(2);

    srand(time(NULL));

    generate();
}

void Field::render(sf::RenderWindow *window) {
    if (neededRender) {
        neededRender = false;
        renderTexture.clear(backgroundColor);

        for (int r = 0; r < data.getRows(); r++) {
            for (int c = 0; c < data.getCols(); c++) {
                tile.setPosition(
                    {r * tileWidth, c * tileHeight}
                );

                renderTexture.draw(tile);

                ball.setPosition(
                    tile.getPosition() + sf::Vector2f{ballOffset, ballOffset}
                );

                const sf::Color ballColor = data.at(r, c).getColor();
                ball.setFillColor(ballColor);
                ball.setOutlineColor(ballColor != sf::Color::Transparent ? sf::Color::Black : sf::Color::Transparent);

                renderTexture.draw(ball);
            }
        }

        renderTexture.display();
    }
}

bool Field::generate() {
    sf::Vector2i pos;
    for (int i = 0; i < countGenerateInIteration; i++) {
        int j = 0;
        do {
            pos.x = rand() % data.getRows();
            pos.y = rand() % data.getCols();
            j++;
            if (j > maxGenerateIterations) {
                return true;
            }

        } while (data.at(pos).type != BALL_NONE_TYPE);

        data.at(pos) = Ball{Ball::Type((rand() % (BALL_TYPES_COUNT - 1)) + 1)};
    }

    reRender();
    return false;
}

void Field::resize() {
    tileLogicWidth = Scaler::scaleX(tileWidth);
    tileLogicHeight = Scaler::scaleY(tileHeight);

    ballLogicOffset = {
        (tileLogicWidth * 0.33f) / 2,
        (tileLogicHeight * 0.33f) / 2,
    };
}

// Getters
const float Field::getBallSize() const {
    return ballSize;
}

// Tile logic size
const sf::Vector2f Field::getTileLogicSize() const {
    return sf::Vector2f(tileLogicWidth, tileLogicHeight);
}
const float Field::getTileLogicWidth() const {
    return tileLogicWidth;
}
const float Field::getTileLogicHeight() const {
    return tileLogicHeight;
}

// Ball logic size
const sf::Vector2f Field::getBallLogicOffset() const {
    return ballLogicOffset;
}
const float Field::getBallLogicOffsetWidth() const {
    return ballLogicOffset.x;
}
const float Field::getBallLogicOffsetHeight() const {
    return ballLogicOffset.y;
}

// Data
const Matrix2<Ball> &Field::getData() const {
    return data;
}
Matrix2<Ball> &Field::getData() {
    return data;
}