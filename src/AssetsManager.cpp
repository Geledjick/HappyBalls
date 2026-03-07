#pragma once

#include <SFML/Graphics/Font.hpp>
#include <string>

#include "debug.cpp"

#define FONT_SIZE 72.f

class AssetsManager {
public:
    static void openFontFrom(const std::string fileName) {
        if (!font.openFromFile(assetsDirectory + fileName)) {
            DEBUG_PRINTF("AssetsManager not found font with name: \'%s\'\n", fileName.c_str());
            exit(42);
        } else {
            DEBUG_PRINTF("AssetsManager loaded font with name: \'%s\'\n", fileName.c_str());
        }
    }

    static const sf::Font &getFont() {
        return font;
    }

private:
    const static std::string assetsDirectory;

    static sf::Font font;
};