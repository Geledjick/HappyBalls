#include "inc/AssetsManager.hpp"

sf::Font AssetsManager::font;
const std::string AssetsManager::assetsDirectory = "./assets/";

void AssetsManager::openFontFrom(const std::string fileName) {
    if (!font.openFromFile(assetsDirectory + fileName)) {
        DEBUG_PRINTF("AssetsManager not found font with name: \'%s\'\n", fileName.c_str());
        exit(42);
    } else {
        DEBUG_PRINTF("AssetsManager loaded font with name: \'%s\'\n", fileName.c_str());
    }
}

const sf::Font &AssetsManager::getFont() {
    return font;
}