#pragma once

#include <SFML/Graphics/Font.hpp>
#include <string>

#include "debug.hpp"

#define FONT_SIZE 72.f

class AssetsManager {
public:
    static void openFontFrom(const std::string fileName);

    static const sf::Font &getFont();

private:
    const static std::string assetsDirectory;

    static sf::Font font;
};