#ifndef HUD_H_
#define HUD_H_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include "Spaceship.h"
#include <iostream>
#include <string>

class HUD {
public:
    sf::Sprite cs;
    sf::Texture t;
    bool csActive;
    std::string csBA; // String used for determining order. 1 to 2, 2 to 3, 1 to 3, and reverse.

    sf::Sprite blinkbarScale;
    sf::Sprite blinkbarStatic;
    sf::Texture bbSc;
    sf::Texture bbSt;
    // sf::Text score;
    // sf::Text Level and Phase

    HUD()
    {
        csActive = false;
        csBA = "";
        cs.setScale(1.5, 1.5);
        t.loadFromFile("spritesheet/CSSpriteSheet.png");
        cs.setTexture(t);
        cs.setTextureRect(sf::IntRect(0, 64, 64, 64));
        cs.setPosition(1280 - 64 - 96, 1280 - 64 - 96);

        bbSc.loadFromFile("img/blinkbarScale.png");
        bbSt.loadFromFile("img/blinkbarStatic.png");
        blinkbarScale.setTexture(bbSc);
        blinkbarStatic.setTexture(bbSt);
        blinkbarScale.setPosition(1280 - 64 - 96 - 32 - 248, 1280 - 64 - 28);
        blinkbarStatic.setPosition(1280 - 64 - 96 - 32 - 256, 1280 - 64 - 36);
    }
    void csValues()
    {
        std::cout << cs.getTextureRect().top << ", " << cs.getTextureRect().left << std::endl;
    }
};

#endif