#ifndef MENU_H_
#define MENU_H_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include "Spaceship.h"
#include <iostream>
#include <string>
#include <fstream>

// The Class Diagram says the relation is Composition
// So I have two approaches:
// 1) One menu Class with different methods for calling different menus
// 2) One menu class with different objects being different menus
// I'll go with 2.

class sfmlText {
public:

    sf::Text text;
    sf::Font font;

    sfmlText()
    {
        font.loadFromFile("font/SpaceMono-Bold.ttf");
        text.setFont(font);
        text.setCharacterSize(48);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2);
    }

};

class MenuBG {
public:
    static sf::Sprite bg;
    static int bgX;
    static int bgY;
    static sf::Texture tex;
    static sf::Clock time;
    MenuBG()
    {
        tex.loadFromFile("spritesheet/menuBG2.png");
        bg.setTexture(tex);
        bg.setTextureRect(sf::IntRect((256*bgX), (256*bgY), 256, 256));
        bg.setScale(5, 5);
    }

};

class Menu {

public:

    int selection;
    sf::Sprite sel;
    sf::Texture selTex;
    int highlight;
    sf::Sprite hil;
    sf::Texture hilTex;
    static sf::Clock hilTimer;
    static sf::Clock jumpTimer;
    bool hilColorDirection;
    bool isJumping;
    bool jumpDirection;
    float originalY;
    sfmlText* options;
    int numOp; // Number of options
    sf::Clock selectedTimer;

    Menu(int numberOfOptions)
    {
        selection = -1;
        highlight = 0;
        numOp = numberOfOptions;
        options = new sfmlText[numOp];
        selTex.loadFromFile("img/parallelogram_Black.png");
        sel.setTexture(selTex);
        sel.setOrigin(sel.getGlobalBounds().width / 2, sel.getGlobalBounds().height / 2);
        hilTex.loadFromFile("img/parallelogram_White.png");
        hil.setTexture(hilTex);
        hil.setOrigin(hil.getGlobalBounds().width / 2, hil.getGlobalBounds().height / 2);
        jumpDirection = false;
        isJumping = false;
        originalY = 0;
    }
    void MenuTexts(std::string optionList[])
    {
        for(int i = 0; i < numOp; i++)
        {
            options[i].text.setString(optionList[i]);
            options[i].text.setOrigin(options[i].text.getGlobalBounds().width / 2, options[i].text.getGlobalBounds().height / 2);
            // Set Origin to center of bounding box
        }
    }
    void MenuTexts(std::string optionList[], int i)
    {
        for(int i = 0; i < numOp; i++)
        {
            options[i].text.setString(optionList[i]);
            // Set Origin to center of bounding box
        }
    }
};

class MainMenu: public Menu {
public:

    std::string optionList[5];

    MainMenu(sf::RenderWindow& window, int rememberSelection, bool& debugActive, MenuBG& menuBG):
    optionList{"Start", "Options", "Instructions", "High Scores", "Exit"},
    Menu(5)
    {
        Menu::MenuTexts(optionList);
        highlight = rememberSelection;
        jumpTimer.restart();

        while(selection == -1)
        {
            if(menuBG.time.getElapsedTime() > sf::seconds(0.033333)) // 30 fps means 33.33 ms per frame.
            {
                menuBG.bgX++;
                if(menuBG.bgX >= 30){menuBG.bgY++;menuBG.bgX = 0;}
                if(menuBG.bgY >= 8){menuBG.bgY = 0;}
                if(menuBG.bgX >= 29 && menuBG.bgY >= 7){menuBG.bgX = 0;menuBG.bgY = 0;}
                menuBG.time.restart();
            }
            menuBG.bg.setTextureRect(sf::IntRect((256*menuBG.bgX), (256*menuBG.bgY), 256, 256));
            if(hilTimer.getElapsedTime() > sf::seconds(0.001666))
            {
                if(hil.getColor().g >= 253 || hil.getColor().b >= 253)
                    hilColorDirection = false;
                else if(hil.getColor().g <= 2 || hil.getColor().b <= 2)
                    hilColorDirection = true;
                if(hilColorDirection == true) // So forwards
                    hil.setColor(sf::Color(255, hil.getColor().g + 1, hil.getColor().b + 1));
                else
                    hil.setColor(sf::Color(255, hil.getColor().g - 1, hil.getColor().b - 1));
                hilTimer.restart();
            }
            if(isJumping == true)
            {
                if(options[highlight].text.getPosition().y < originalY - 25)
                {    
                    jumpDirection = false;
                    options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y + 2);
                }
                if(jumpTimer.getElapsedTime() > sf::seconds(0.16666))
                {
                    if(jumpDirection == true)
                    {
                        // std::cout << "Up, New: " << options[highlight].text.getPosition().y << ", Original: " << originalY << std::endl;
                        options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y - 2);
                    }
                    else 
                    {
                        // std::cout << "Down, New: " << options[highlight].text.getPosition().y << ", Original: " << originalY << std::endl;
                        options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y + 2);
                    }
                    jumpTimer.restart();
                }
            }
            sf::Event e;
            while (window.pollEvent(e))
            {                  
                if (e.type == sf::Event::Closed)
                {
                    selection = 4;
                    return;
                }
                if (e.type == sf::Event::KeyReleased)
                {
                    if(e.key.code == sf::Keyboard::Up)
                        highlight -= 1;
                    if(e.key.code == sf::Keyboard::Down)
                        highlight += 1;

                    if(highlight == -1)
                        highlight = 4;
                    if(highlight == 5)
                        highlight = 0;

                    if(e.key.code == sf::Keyboard::Enter)
                        selection = highlight;
                    if(e.key.code == sf::Keyboard::Up || e.key.code == sf::Keyboard::Down)
                    {
                        isJumping = true;
                        jumpDirection = true;
                        originalY = 0;
                    }
                }
            }

            for(int i = 0; i < numOp; i++)
            {
                if(!(i == highlight && isJumping == true))
                    options[i].text.setPosition(640, (i+1)*(1280.0/10) + 250);

                sel.setPosition(options[i].text.getPosition().x, options[i].text.getPosition().y + 10);
                hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
                if(i == highlight)
                {
                    window.draw(hil);
                    if(isJumping == true)
                    {
                        if(originalY == 0)
                            originalY = options[highlight].text.getPosition().y;
                        options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y - 2);
                    }
                }
                window.draw(sel);
                window.draw(options[i].text);
            }
            window.display();
            window.clear();
            window.draw(menuBG.bg);
            if(selection != -1)
            {
                isJumping = false;
                selectedTimer.restart();
                sf::Clock flashTimer;
                while(selectedTimer.getElapsedTime().asSeconds() < 0.6)
                {
                    if(menuBG.time.getElapsedTime() > sf::seconds(0.033333)) // 30 fps means 33.33 ms per frame.
                    {
                        menuBG.bgX++;
                        if(menuBG.bgX >= 30){menuBG.bgY++;menuBG.bgX = 0;}
                        if(menuBG.bgY >= 8){menuBG.bgY = 0;}
                        if(menuBG.bgX >= 29 && menuBG.bgY >= 7){menuBG.bgX = 0;menuBG.bgY = 0;}
                        menuBG.time.restart();
                    }
                    menuBG.bg.setTextureRect(sf::IntRect((256*menuBG.bgX), (256*menuBG.bgY), 256, 256));
                    if(hilTimer.getElapsedTime() > sf::seconds(0.001666))
                    {
                        if(hil.getColor().g >= 253 || hil.getColor().b >= 253)
                            hilColorDirection = false;
                        else if(hil.getColor().g <= 2 || hil.getColor().b <= 2)
                            hilColorDirection = true;
                        if(hilColorDirection == true) // So forwards
                            hil.setColor(sf::Color(255, hil.getColor().g + 1, hil.getColor().b + 1));
                        else
                            hil.setColor(sf::Color(255, hil.getColor().g - 1, hil.getColor().b - 1));
                        hilTimer.restart();
                    }
                    for(int i = 0; i < numOp; i++)
                    {
                        options[i].text.setPosition(640, (i+1)*(1280.0/10) + 250);

                        sel.setPosition(options[i].text.getPosition().x, options[i].text.getPosition().y + 10);
                        hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
                        if(i == highlight)
                        {
                            window.draw(hil);
                        }
                        window.draw(sel);
                        if(i == highlight && flashTimer.getElapsedTime().asSeconds() > 0.12)
                        {
                            options[i].text.setPosition(5000, 5000);
                        }
                        if(i == highlight && flashTimer.getElapsedTime().asSeconds() > 0.24)
                        {
                            options[i].text.setPosition(640, (i+1)*(1280.0/10) + 250);
                            flashTimer.restart();
                        }
                        window.draw(options[i].text);
                    }
                    window.display();
                    window.clear();
                    window.draw(menuBG.bg);
                    // std::cout << flashTimer.getElapsedTime().asSeconds() << std::endl;
                }
            }
        }
    }
};
class MainMenu_Start: public Menu {
public:

    std::string optionList[7];

    MainMenu_Start(sf::RenderWindow& window, MenuBG& menuBG, sf::Music& lvl):
    optionList{"Level 1", "Level 2", "Level 3", "Boss 1", "Boss 2", "Boss 3", "Return"},
    Menu(7)
    {
        Menu::MenuTexts(optionList);
        char tempChar;
        int position = 0;

        while(selection == -1)
        {
            if(menuBG.time.getElapsedTime() > sf::seconds(0.033333)) // 30 fps means 33 ms per frame.
            {
                menuBG.bgX++;
                if(menuBG.bgX >= 30){menuBG.bgY++;menuBG.bgX = 0;}
                if(menuBG.bgY >= 8){menuBG.bgY = 0;}
                if(menuBG.bgX >= 29 && menuBG.bgY >= 7){menuBG.bgX = 0;menuBG.bgY = 0;}
                menuBG.time.restart();
            }
            menuBG.bg.setTextureRect(sf::IntRect((256*menuBG.bgX), (256*menuBG.bgY), 256, 256));
            if(hilTimer.getElapsedTime() > sf::seconds(0.001666)) // 60 fps means 16.66 ms per frame
            {
                if(hil.getColor().g >= 253 || hil.getColor().b >= 253)
                    hilColorDirection = false;
                else if(hil.getColor().g <= 2 || hil.getColor().b <= 2)
                    hilColorDirection = true;
                if(hilColorDirection == true) // So forwards
                    hil.setColor(sf::Color(255, hil.getColor().g + 1, hil.getColor().b + 1));
                else
                    hil.setColor(sf::Color(255, hil.getColor().g - 1, hil.getColor().b - 1));
                hilTimer.restart();
            }
            if(isJumping == true)
            {
                if(options[highlight].text.getPosition().y < originalY - 25)
                {    
                    jumpDirection = false;
                    options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y + 2);
                }
                if(jumpTimer.getElapsedTime() > sf::seconds(0.16666))
                {
                    if(jumpDirection == true)
                    {
                        // std::cout << "Up, New: " << options[highlight].text.getPosition().y << ", Original: " << originalY << std::endl;
                        options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y - 2);
                    }
                    else 
                    {
                        // std::cout << "Down, New: " << options[highlight].text.getPosition().y << ", Original: " << originalY << std::endl;
                        options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y + 2);
                    }
                    jumpTimer.restart();
                }
            }
            sf::Event e;
            while (window.pollEvent(e))
            {                  
                if (e.type == sf::Event::Closed)
                {
                    selection = 10;
                    return;
                }
                if (e.type == sf::Event::KeyReleased)
                {
                    if(e.key.code == sf::Keyboard::Up)
                        highlight -= 1;
                    if(e.key.code == sf::Keyboard::Down)
                        highlight += 1;
                    if(e.key.code == sf::Keyboard::Escape)
                        selection = 6;

                    if(highlight == -1){highlight = 6;}
                    if(highlight == 7){highlight = 0;}
                    if(e.key.code == sf::Keyboard::Enter)
                        selection = highlight;
                    if(e.key.code == sf::Keyboard::Up || e.key.code == sf::Keyboard::Down)
                    {
                        isJumping = true;
                        jumpDirection = true;
                        originalY = 0;
                    }
                }
            }

            for(int i = 0; i < numOp; i++)
            {
                if(!(i == highlight && isJumping == true))
                    options[i].text.setPosition(640, (i+1)*(1280.0/8));
                sel.setPosition(options[i].text.getPosition().x, options[i].text.getPosition().y + 10);
                hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
                if(i == highlight)
                {
                    window.draw(hil);
                    if(isJumping == true)
                    {
                        if(originalY == 0)
                            originalY = options[highlight].text.getPosition().y;
                        options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y - 2);
                    }
                }
                window.draw(sel);
                window.draw(options[i].text);
            }
            window.display();
            window.clear();
            window.draw(menuBG.bg);
        }
        if(selection != -1)
        {
            isJumping = false;
            selectedTimer.restart();
            sf::Clock flashTimer;
            while(selectedTimer.getElapsedTime().asSeconds() < 0.6)
            {
                if(menuBG.time.getElapsedTime() > sf::seconds(0.033333)) // 30 fps means 33.33 ms per frame.
                {
                    menuBG.bgX++;
                    if(menuBG.bgX >= 30){menuBG.bgY++;menuBG.bgX = 0;}
                    if(menuBG.bgY >= 8){menuBG.bgY = 0;}
                    if(menuBG.bgX >= 29 && menuBG.bgY >= 7){menuBG.bgX = 0;menuBG.bgY = 0;}
                    menuBG.time.restart();
                }
                menuBG.bg.setTextureRect(sf::IntRect((256*menuBG.bgX), (256*menuBG.bgY), 256, 256));
                if(hilTimer.getElapsedTime() > sf::seconds(0.001666))
                {
                    if(hil.getColor().g >= 253 || hil.getColor().b >= 253)
                        hilColorDirection = false;
                    else if(hil.getColor().g <= 2 || hil.getColor().b <= 2)
                        hilColorDirection = true;
                    if(hilColorDirection == true) // So forwards
                        hil.setColor(sf::Color(255, hil.getColor().g + 1, hil.getColor().b + 1));
                    else
                        hil.setColor(sf::Color(255, hil.getColor().g - 1, hil.getColor().b - 1));
                    hilTimer.restart();
                }
                for(int i = 0; i < numOp; i++)
                {
                    if(!(i == highlight && isJumping == true))
                        options[i].text.setPosition(640, (i+1)*(1280.0/8));
                    sel.setPosition(options[i].text.getPosition().x, options[i].text.getPosition().y + 10);
                    hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
                    if(i == highlight)
                    {
                        window.draw(hil);
                        if(isJumping == true)
                        {
                            if(originalY == 0)
                                originalY = options[highlight].text.getPosition().y;
                            options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y - 2);
                        }
                    }
                    window.draw(sel);
                    if(i == highlight && flashTimer.getElapsedTime().asSeconds() > 0.12)
                    {
                        options[i].text.setPosition(5000, 5000);
                    }
                    if(i == highlight && flashTimer.getElapsedTime().asSeconds() > 0.24)
                    {
                        options[i].text.setPosition(640, (i+1)*(1280.0/10) + 250);
                        flashTimer.restart();
                    }
                    window.draw(options[i].text);
                }

                window.display();
                window.clear();
                window.draw(menuBG.bg);
                // std::cout << flashTimer.getElapsedTime().asSeconds() << std::endl;
            }
        }
        if(selection == 0)
        {
            lvl.openFromFile("audio/03_LEVEL1.ogg");
        }
        else if(selection == 1)
        {
            lvl.openFromFile("audio/04_LEVEL2.ogg");
        }
        else if(selection == 2)
        {
            lvl.openFromFile("audio/05_LEVEL3.ogg");
        }
        else if(selection == 3)
        {
            lvl.openFromFile("audio/06_BOSS1.ogg");
        }
        else if(selection == 4)
        {
            lvl.openFromFile("audio/07_BOSS2.ogg");
        }
        else if(selection == 5)
        {
            lvl.openFromFile("audio/08_BOSS3.ogg");
        }
    }
};
class MainMenu_Start_Settings: public Menu {
public:

    std::string optionList[5];

    MainMenu_Start_Settings(sf::RenderWindow& window, MenuBG& menuBG):
    optionList{"=== Choose Difficulty ===", "Easy", "Medium", "Hard", "Return"},
    Menu(5)
    {
        Menu::MenuTexts(optionList);
        int position = 0;
        selection = 0;
        highlight = 1;

        while(selection == 0)
        {
            if(menuBG.time.getElapsedTime() > sf::seconds(0.033333)) // 30 fps means 33 ms per frame.
            {
                menuBG.bgX++;
                if(menuBG.bgX >= 30){menuBG.bgY++;menuBG.bgX = 0;}
                if(menuBG.bgY >= 8){menuBG.bgY = 0;}
                if(menuBG.bgX >= 29 && menuBG.bgY >= 7){menuBG.bgX = 0;menuBG.bgY = 0;}
                menuBG.time.restart();
            }
            menuBG.bg.setTextureRect(sf::IntRect((256*menuBG.bgX), (256*menuBG.bgY), 256, 256));
            if(hilTimer.getElapsedTime() > sf::seconds(0.001666)) // 60 fps means 16.66 ms per frame
            {
                if(hil.getColor().g >= 253 || hil.getColor().b >= 253)
                    hilColorDirection = false;
                else if(hil.getColor().g <= 2 || hil.getColor().b <= 2)
                    hilColorDirection = true;
                if(hilColorDirection == true) // So forwards
                    hil.setColor(sf::Color(255, hil.getColor().g + 1, hil.getColor().b + 1));
                else
                    hil.setColor(sf::Color(255, hil.getColor().g - 1, hil.getColor().b - 1));
                hilTimer.restart();
            }
            sf::Event e;
            while (window.pollEvent(e))
            {                  
                if (e.type == sf::Event::Closed)
                {
                    selection = 10;
                    return;
                }
                if (e.type == sf::Event::KeyReleased)
                {
                    if(e.key.code == sf::Keyboard::Up)
                        highlight -= 1;
                    if(e.key.code == sf::Keyboard::Down)
                        highlight += 1;

                    if(highlight == 0){highlight = 4;}
                    if(highlight == 5){highlight = 1;}
                    if(e.key.code == sf::Keyboard::Enter)
                        selection = highlight;
                }
            }
            options[0].text.setPosition(640, 1280.0/6);
            sel.setPosition(options[0].text.getPosition().x - 200, options[0].text.getPosition().y + 10);
            window.draw(sel);
            sel.setPosition(options[0].text.getPosition().x + 200, options[0].text.getPosition().y + 10);
            window.draw(sel);
            window.draw(options[0].text);
            for(int i = 1; i < numOp; i++)
            {
                options[i].text.setPosition(640, (i+1)*(1280.0/6));
                sel.setPosition(options[i].text.getPosition().x, options[i].text.getPosition().y + 10);
                hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
                if(i == highlight)
                    window.draw(hil);
                window.draw(sel);
                window.draw(options[i].text);
            }
            window.display();
            window.clear();
            window.draw(menuBG.bg);
        }
    }
};
class Menu_Options: public Menu {
public:

    std::string optionList[3];

    Menu_Options(sf::RenderWindow& window, sf::Music& song, int& steering, MenuBG& menuBG):
    optionList{"Volume Control", "Steering Sensitivity", "Return"},
    Menu(3)
    {
        Menu::MenuTexts(optionList, 1); // The 1 means, don't change Origins.
        char tempChar;
        int position = 0;
        sf::Clock t;

        std::string temp = optionList[0];
        while(temp.length() != 25){temp += " ";}
        temp += std::to_string((int)song.getVolume());
        options[0].text.setString(temp);
        t.restart();

        temp = optionList[1];
        while(temp.length() != 25){temp += " ";}
        temp += std::to_string(steering + 1);
        options[1].text.setString(temp);

        while(selection == -1)
        {
            if(menuBG.time.getElapsedTime() > sf::seconds(0.033333)) // 30 fps means 33 ms per frame.
            {
                menuBG.bgX++;
                if(menuBG.bgX >= 30){menuBG.bgY++;menuBG.bgX = 0;}
                if(menuBG.bgY >= 8){menuBG.bgY = 0;}
                if(menuBG.bgX >= 29 && menuBG.bgY >= 7){menuBG.bgX = 0;menuBG.bgY = 0;}
                menuBG.time.restart();
            }
            menuBG.bg.setTextureRect(sf::IntRect((256*menuBG.bgX), (256*menuBG.bgY), 256, 256));
            if(hilTimer.getElapsedTime() > sf::seconds(0.001666)) // 60 fps means 16.66 ms per frame
            {
                if(hil.getColor().g >= 253 || hil.getColor().b >= 253)
                    hilColorDirection = false;
                else if(hil.getColor().g <= 2 || hil.getColor().b <= 2)
                    hilColorDirection = true;
                if(hilColorDirection == true) // So forwards
                    hil.setColor(sf::Color(255, hil.getColor().g + 1, hil.getColor().b + 1));
                else
                    hil.setColor(sf::Color(255, hil.getColor().g - 1, hil.getColor().b - 1));
                hilTimer.restart();
            }
            sf::Event e;
            while (window.pollEvent(e))
            {                  
                if (e.type == sf::Event::Closed)
                {
                    selection = 10;
                    return;
                }
                if (e.type == sf::Event::KeyReleased)
                {
                    if(e.key.code == sf::Keyboard::Up)
                        highlight -= 1;
                    if(e.key.code == sf::Keyboard::Down)
                        highlight += 1;
                    if(e.key.code == sf::Keyboard::Escape)
                        selection = 3;

                    if(highlight == 0) // Song Volume
                    {
                        if(song.getVolume() > 1)
                            if(e.key.code == sf::Keyboard::Key::Left)
                                song.setVolume(song.getVolume() - 1);
                        if(song.getVolume() < 100)
                            if(e.key.code == sf::Keyboard::Key::Right)
                                song.setVolume(song.getVolume() + 1);

                        std::string temp = optionList[0];
                        while(temp.length() != 25){temp += " ";}
                        temp += std::to_string((int)song.getVolume());
                        options[0].text.setString(temp);
                    }
                    if(highlight == 1) // Steering Sensitivity
                    {
                        if(steering > 0)
                            if(e.key.code == sf::Keyboard::Key::Left)
                                steering--;
                        if(steering < 9)
                            if(e.key.code == sf::Keyboard::Key::Right)
                                steering++;

                        std::string temp = optionList[1];
                        while(temp.length() != 25){temp += " ";}
                        temp += std::to_string(steering + 1);
                        options[1].text.setString(temp);
                    }

                    // No IF for highlight option 3. You just press enter.

                    if(highlight == -1){highlight = 2;}
                    if(highlight == 3){highlight = 0;}
                    if(e.key.code == sf::Keyboard::Enter)
                        selection = highlight;
                }
            }
            if(highlight == 0 && t.getElapsedTime().asSeconds() > 0.06) // Volume Control
            {
                if(song.getVolume() > 1)
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                        song.setVolume(song.getVolume() - 1);
                if(song.getVolume() < 100)
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                        song.setVolume(song.getVolume() + 1);

                std::string temp = optionList[0];
                while(temp.length() != 25){temp += " ";}
                temp += std::to_string((int)song.getVolume());
                options[0].text.setString(temp);
                t.restart();
            }
            if(highlight == 1 && t.getElapsedTime().asSeconds() > 0.2) // Steering Sensitivity
            {
                if(steering > 0)
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                        steering--;
                if(steering < 9)
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                        steering++;

                std::string temp = optionList[1];
                while(temp.length() != 25){temp += " ";}
                temp += std::to_string(steering + 1);
                options[1].text.setString(temp);
                t.restart();
            }
            for(int i = 0; i < numOp; i++)
            {
                options[i].text.setPosition(160, (i+1)*(1280.0/4));
                sel.setPosition(options[i].text.getPosition().x, options[i].text.getPosition().y + 30);
                hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
                if(i == highlight)
                    window.draw(hil);
                window.draw(sel);
                sel.setPosition(sel.getPosition().x + 400, sel.getPosition().y);
                hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
                if(i == highlight)
                    window.draw(hil);
                window.draw(sel);
                sel.setPosition(sel.getPosition().x + 400, sel.getPosition().y);
                hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
                if(i == highlight)
                    window.draw(hil);
                window.draw(sel);
                window.draw(options[i].text);
            }
            window.display();
            window.clear();
            window.draw(menuBG.bg);
        }
    }
};
class Menu_Help: public Menu {
public:

    std::string optionList[1];

    Menu_Help(sf::RenderWindow& window, MenuBG& menuBG):
    optionList{"Return"},
    Menu(1)
    {
        Menu::MenuTexts(optionList);
        char tempChar;
        int position = 0;

        sf::Sprite rect;
        sf::Texture tex;
        tex.loadFromFile("img/help.png");
        rect.setTexture(tex);
        rect.setOrigin(500, 500);
        rect.setPosition(640, 520);

        while(selection == -1)
        {
            if(menuBG.time.getElapsedTime() > sf::seconds(0.033333)) // 30 fps means 33 ms per frame.
            {
                menuBG.bgX++;
                if(menuBG.bgX >= 30){menuBG.bgY++;menuBG.bgX = 0;}
                if(menuBG.bgY >= 8){menuBG.bgY = 0;}
                if(menuBG.bgX >= 29 && menuBG.bgY >= 7){menuBG.bgX = 0;menuBG.bgY = 0;}
                menuBG.time.restart();
            }
            menuBG.bg.setTextureRect(sf::IntRect((256*menuBG.bgX), (256*menuBG.bgY), 256, 256));
            if(hilTimer.getElapsedTime() > sf::seconds(0.001666)) // 60 fps means 16.66 ms per frame
            {
                if(hil.getColor().g >= 253 || hil.getColor().b >= 253)
                    hilColorDirection = false;
                else if(hil.getColor().g <= 2 || hil.getColor().b <= 2)
                    hilColorDirection = true;
                if(hilColorDirection == true) // So forwards
                    hil.setColor(sf::Color(255, hil.getColor().g + 1, hil.getColor().b + 1));
                else
                    hil.setColor(sf::Color(255, hil.getColor().g - 1, hil.getColor().b - 1));
                hilTimer.restart();
            }
            sf::Event e;
            while (window.pollEvent(e))
            {                  
                if (e.type == sf::Event::Closed)
                {
                    selection = 10;
                    return;
                }
                if (e.type == sf::Event::KeyReleased)
                {
                    if(e.key.code == sf::Keyboard::Enter)
                        selection = highlight;
                    if(e.key.code == sf::Keyboard::Escape)
                        selection = 0;
                }
            }
            window.draw(rect);
            options[0].text.setPosition(640, 1180);
            sel.setPosition(options[0].text.getPosition().x, options[0].text.getPosition().y + 10);
            hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);

            window.draw(hil);
            window.draw(sel);
            window.draw(options[0].text);
            window.display();
            window.clear();
            window.draw(menuBG.bg);
        }
    }
};
class Menu_HighScores: public Menu {
public:

    std::string optionList[1];

    Menu_HighScores(sf::RenderWindow& window, MenuBG& menuBG, sf::Music& lvlMusic, sf::Music& menuMusic):
    optionList{"Return"},
    Menu(1)
    {
        if(lvlMusic.getStatus() == sf::Music::Playing)
            lvlMusic.stop();
        if(menuMusic.getStatus() == sf::Music::Playing)
            menuMusic.stop();
        Menu::MenuTexts(optionList); // 
        char tempChar;
        int position = 0;
        sf::Music m;
        m.openFromFile("audio/11_SCORESCREEN.ogg");
        m.setVolume(100);
        m.setLoop(false);
        m.play();

        std::fstream theScores;
        int rowsInTxtFile = 0;
        std::string name[10];
        int highScore[10];
        theScores.open("misc/hiscores.txt");
        for(int i = 0; i < 10; i++)
        {
            theScores >> name[i];
            theScores >> highScore[i];
        }
        theScores.close();
        sfmlText scoreList;
        scoreList.text.setCharacterSize(48);
        std::string dataInFile;

        for(int i = 0; i < 10; i++)
        {
            dataInFile += name[i];
            dataInFile += " ";
            dataInFile += std::to_string((highScore[i]));
            dataInFile += "\n";
        }
        scoreList.text.setString(dataInFile);

        while(selection == -1)
        {
            if(menuBG.time.getElapsedTime() > sf::seconds(0.033333)) // 30 fps means 33 ms per frame.
            {
                menuBG.bgX++;
                if(menuBG.bgX >= 30){menuBG.bgY++;menuBG.bgX = 0;}
                if(menuBG.bgY >= 8){menuBG.bgY = 0;}
                if(menuBG.bgX >= 29 && menuBG.bgY >= 7){menuBG.bgX = 0;menuBG.bgY = 0;}
                menuBG.time.restart();
            }
            menuBG.bg.setTextureRect(sf::IntRect((256*menuBG.bgX), (256*menuBG.bgY), 256, 256));
            if(hilTimer.getElapsedTime() > sf::seconds(0.001666)) // 60 fps means 16.66 ms per frame
            {
                if(hil.getColor().g >= 253 || hil.getColor().b >= 253)
                    hilColorDirection = false;
                else if(hil.getColor().g <= 2 || hil.getColor().b <= 2)
                    hilColorDirection = true;
                if(hilColorDirection == true) // So forwards
                    hil.setColor(sf::Color(255, hil.getColor().g + 1, hil.getColor().b + 1));
                else
                    hil.setColor(sf::Color(255, hil.getColor().g - 1, hil.getColor().b - 1));
                hilTimer.restart();
            }
            sf::Event e;
            while (window.pollEvent(e))
            {                  
                if (e.type == sf::Event::Closed)
                {
                    selection = 10;
                    return;
                }
                if (e.type == sf::Event::KeyReleased)
                {
                    if(e.key.code == sf::Keyboard::Enter)
                        selection = 0;
                    if(e.key.code == sf::Keyboard::Escape)
                        selection = 0;
                }
            }
            sf::Sprite rect;
            sf::Texture tex;
            tex.loadFromFile("img/scoreBG.png");
            rect.setTexture(tex);
            rect.setOrigin(480, 480);
            rect.setPosition(640, 520);
            window.draw(rect);
            options[0].text.setPosition(640, 1160);
            sel.setPosition(options[0].text.getPosition().x, options[0].text.getPosition().y + 10);
            hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
            window.draw(hil);
            window.draw(sel);
            window.draw(options[0].text);
            scoreList.text.setPosition(640, 500);
            scoreList.text.setOrigin(scoreList.text.getGlobalBounds().width / 2, scoreList.text.getGlobalBounds().height / 2);
            window.draw(scoreList.text);
            window.display();
            window.clear();
            window.draw(menuBG.bg);
        }
    }
};
class Menu_Debug: public Menu {
public:

    std::string optionList[7];

    Menu_Debug(sf::RenderWindow& window, Spaceship& player, bool& asteroidsUnlocked, MenuBG& menuBG):
    optionList{"Speed", "Damage", "God Mode", "Lives", "PowerUp", "Fire", "Return"},
    Menu(7)
    {
        Menu::MenuTexts(optionList, 1);
        char tempChar;
        int position = 0;

        {
            std::string temp = optionList[0];
            while(temp.length() != 25){temp += " ";}
            temp += std::to_string(player.getSpeed());
            options[0].text.setString(temp);
            temp = optionList[1];
            while(temp.length() != 25){temp += " ";}
            temp += std::to_string(player.getDamage());
            options[1].text.setString(temp);
            temp = optionList[2];
            while(temp.length() != 25){temp += " ";}
            if(player.isInvincible())
                temp += "ON";
            else
                temp += "OFF";
            options[2].text.setString(temp);
            temp = optionList[3];
            while(temp.length() != 25){temp += " ";}
            temp += std::to_string(player.getLives());
            options[3].text.setString(temp);
            temp = optionList[4];
            while(temp.length() != 25){temp += " ";}
            if(player.isEmpowered())
                temp += "ON";
            else
                temp += "OFF";
            options[4].text.setString(temp);
            temp = optionList[5];
            while(temp.length() != 25){temp += " ";}
            if(player.isOnFire() == true)
                temp += "ON";
            else
                temp += "OFF";
            options[5].text.setString(temp);
        }

        while(selection == -1)
        {
            if(menuBG.time.getElapsedTime() > sf::seconds(0.033333)) // 30 fps means 33 ms per frame.
            {
                menuBG.bgX++;
                if(menuBG.bgX >= 30){menuBG.bgY++;menuBG.bgX = 0;}
                if(menuBG.bgY >= 8){menuBG.bgY = 0;}
                if(menuBG.bgX >= 29 && menuBG.bgY >= 7){menuBG.bgX = 0;menuBG.bgY = 0;}
                menuBG.time.restart();
            }
            menuBG.bg.setTextureRect(sf::IntRect((256*menuBG.bgX), (256*menuBG.bgY), 256, 256));
            if(hilTimer.getElapsedTime() > sf::seconds(0.001666)) // 60 fps means 16.66 ms per frame
            {
                if(hil.getColor().g >= 253 || hil.getColor().b >= 253)
                    hilColorDirection = false;
                else if(hil.getColor().g <= 2 || hil.getColor().b <= 2)
                    hilColorDirection = true;
                if(hilColorDirection == true) // So forwards
                    hil.setColor(sf::Color(255, hil.getColor().g + 1, hil.getColor().b + 1));
                else
                    hil.setColor(sf::Color(255, hil.getColor().g - 1, hil.getColor().b - 1));
                hilTimer.restart();
            }
            sf::Event e;
            while (window.pollEvent(e))
            {                  
                if (e.type == sf::Event::Closed)
                {
                    selection = 10;
                    return;
                }
                if (e.type == sf::Event::KeyReleased)
                {
                    if(e.key.code == sf::Keyboard::Up)
                        highlight -= 1;
                    if(e.key.code == sf::Keyboard::Down)
                        highlight += 1;
                    if(e.key.code == sf::Keyboard::Escape)
                        selection = 6;

                    if(highlight == -1){highlight = 6;}
                    if(highlight == 7){highlight = 0;}
                    if(e.key.code == sf::Keyboard::Enter)
                        selection = highlight;

                    if(highlight == 0) // Speed
                    {
                        if(player.getSpeed() > 1)
                            if(e.key.code == sf::Keyboard::Left)
                                player.setSpeed(player.getSpeed() - 1);
                        if(player.getSpeed() < 4)
                            if(e.key.code == sf::Keyboard::Right)
                                player.setSpeed(player.getSpeed() + 1);

                        if(player.getSpeed() != 4)
                            asteroidsUnlocked = false;
                        std::string temp = optionList[0];
                        while(temp.length() != 25){temp += " ";}
                        temp += std::to_string(player.getSpeed());
                        options[0].text.setString(temp);
                    }
                    if(highlight == 1) // Damage
                    {
                        if(player.getDamage() > 1)
                            if(e.key.code == sf::Keyboard::Left)
                                player.setDamage(player.getDamage() - 1);
                        if(player.getDamage() < 4)
                            if(e.key.code == sf::Keyboard::Right)
                                player.setDamage(player.getDamage() + 1);

                        std::string temp = optionList[1];
                        while(temp.length() != 25){temp += " ";}
                        temp += std::to_string(player.getDamage());
                        options[1].text.setString(temp);
                    }
                    if(highlight == 2) // GodMode
                    {
                        if(player.isInvincible() == false)
                        {
                            if(e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right)
                                player.setInvincible(true);
                        }
                        else if(player.isInvincible() == true)
                        {
                            if(e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right)
                                player.setInvincible(false);
                        }
                        std::string temp = optionList[2];
                        while(temp.length() != 25){temp += " ";}
                        if(player.isInvincible())
                            temp += "ON";
                        else
                            temp += "OFF";
                        options[2].text.setString(temp);
                    }
                    if(highlight == 3) // Lives
                    {
                        if(player.getLives() > 1)
                            if(e.key.code == sf::Keyboard::Left)
                                player.setLives(player.getLives() - 1);
                        if(player.getLives() < 5)
                            if(e.key.code == sf::Keyboard::Right)
                                player.setLives(player.getLives() + 1);

                        std::string temp = optionList[3];
                        while(temp.length() != 25){temp += " ";}
                        temp += std::to_string(player.getLives());
                        options[3].text.setString(temp);
                    }
                    if(highlight == 4) // Power-Up
                    {
                        if(player.isEmpowered() == false)
                        {
                            if(e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right)
                                player.setEmpowered(true);
                        }
                        else if(player.isEmpowered() == true)
                        {
                            if(e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right)
                                player.setEmpowered(false);
                        }
                        std::string temp = optionList[4];
                        while(temp.length() != 25){temp += " ";}
                        if(player.isEmpowered())
                            temp += "ON";
                        else
                            temp += "OFF";
                        options[4].text.setString(temp);
                    }
                    if(highlight == 5) // Fire
                    {
                        if(player.isOnFire() == false)
                        {
                            if(e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right)
                                player.becomeOnFire(true);
                        }
                        else if(player.isOnFire() == true)
                        {
                            if(e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Right)
                                player.becomeOnFire(false);
                        }
                        std::string temp = optionList[5];
                        while(temp.length() != 25){temp += " ";}
                        if(player.isOnFire() == true)
                            temp += "ON";
                        else
                            temp += "OFF";
                        options[5].text.setString(temp);
                    }
                }
            }
            for(int i = 0; i < numOp; i++)
            {
                options[i].text.setPosition(160, (i+1)*(1280.0/8));
                sel.setPosition(options[i].text.getPosition().x, options[i].text.getPosition().y + 30);
                hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
                if(i == highlight)
                    window.draw(hil);
                window.draw(sel);
                sel.setPosition(sel.getPosition().x + 400, sel.getPosition().y);
                hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
                if(i == highlight)
                    window.draw(hil);
                window.draw(sel);
                sel.setPosition(sel.getPosition().x + 400, sel.getPosition().y);
                hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
                if(i == highlight)
                    window.draw(hil);
                window.draw(sel);
                window.draw(options[i].text);
            }
            window.display();
            window.clear();
            window.draw(menuBG.bg);
        }
    }
};
class LevelClear: public Menu {
public:

    std::string optionList[1];
    LevelClear(sf::RenderWindow& window, MenuBG& menuBG, std::string& name, int& totalScore):
    optionList{"Enter Name"},
    Menu(1)
    {
        Menu::MenuTexts(optionList);

        bool isNameEntered = false;
        char tempChar;
        sfmlText t;
        t.text.setString(name);

        while(isNameEntered == false)
        {
            if(menuBG.time.getElapsedTime() > sf::seconds(0.033333)) // 30 fps means 33 ms per frame.
            {
                menuBG.bgX++;
                if(menuBG.bgX >= 30){menuBG.bgY++;menuBG.bgX = 0;}
                if(menuBG.bgY >= 8){menuBG.bgY = 0;}
                if(menuBG.bgX >= 29 && menuBG.bgY >= 7){menuBG.bgX = 0;menuBG.bgY = 0;}
                menuBG.time.restart();
            }
            menuBG.bg.setTextureRect(sf::IntRect((256*menuBG.bgX), (256*menuBG.bgY), 256, 256));
            if(hilTimer.getElapsedTime() > sf::seconds(0.001666)) // 60 fps means 16.66 ms per frame
            {
                if(hil.getColor().g >= 253 || hil.getColor().b >= 253)
                    hilColorDirection = false;
                else if(hil.getColor().g <= 2 || hil.getColor().b <= 2)
                    hilColorDirection = true;
                if(hilColorDirection == true) // So forwards
                    hil.setColor(sf::Color(255, hil.getColor().g + 1, hil.getColor().b + 1));
                else
                    hil.setColor(sf::Color(255, hil.getColor().g - 1, hil.getColor().b - 1));
                hilTimer.restart();
            }
            sf::Event e;
            while (window.pollEvent(e))
            {
                if (e.type == sf::Event::Closed)
                {
                    selection = 10;
                    return;
                }
                if (e.type == sf::Event::TextEntered)
                {
                    tempChar = e.text.unicode;
                    if(int(tempChar) >= 97 && int(tempChar) <= 122)
                        tempChar -= 32;
                    if(tempChar != '\n' && (int(tempChar) >= 65 && int(tempChar <= 90)) && name.length() < 9)
                    // If the number of characters is more than 9, then don't take more inputs.
                        name += tempChar;
                    if(name.length() == 9)
                        isNameEntered = true;
                }
                if (e.type == sf::Event::KeyReleased)
                {
                    if(e.key.code == sf::Keyboard::Enter)
                        isNameEntered = true;
                }
            }
            options[0].text.setPosition(640, 256);
            sel.setPosition(options[0].text.getPosition().x, options[0].text.getPosition().y + 10);
            hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
            window.draw(hil);
            window.draw(sel);
            window.draw(options[0].text);
            t.text.setString(name);
            t.text.setOrigin(t.text.getGlobalBounds().width/2, t.text.getGlobalBounds().height/2);
            t.text.setPosition(640, 640);
            window.draw(t.text);
            window.display();
            window.clear();
            window.draw(menuBG.bg);
        }
        while(name.length() < 9)
            name += "_";
        std::fstream theScores;
        int rowsInTxtFile = 0;
        std::string nameList[10];
        int highScore[10];
        theScores.open("misc/hiscores.txt");
        for(int i = 0; i < 10; i++)
        {
            theScores >> nameList[i];
            theScores >> highScore[i];
        }
        theScores.close();
        sfmlText scoreList;
        scoreList.text.setCharacterSize(32);
        std::string dataInFile;

        int temp = highScore[0];
        std::string temp2 = nameList[0];
        int temp3 = totalScore;
        for(int i = 0; i < 10; i++)
        {
            if (temp3 > highScore[i])
            {
                temp = temp3;
                temp3 = highScore[i];
                highScore[i] = temp;
                temp2 = name;
                name = nameList[i];
                nameList[i] = temp2;
            }
        }

        for(int i = 0; i < 10; i++)
        {
            dataInFile += nameList[i];
            dataInFile += " ";
            dataInFile += std::to_string((highScore[i]));
            dataInFile += "\n";
        }
        theScores.open("misc/hiscores.txt", std::ios::out);
        theScores << dataInFile;
        totalScore = 0;
    }
};
class GameOver: public Menu {
public:
    std::string optionList[1];

    GameOver(sf::RenderWindow& window, int& totalScore):
    optionList{"Return"},
    Menu(1)
    {
        Menu::MenuTexts(optionList); // 
        char tempChar;
        int position = 0;
        sf::Sprite back;
        sf::Texture backTex;
        backTex.loadFromFile("img/gameOver.png");
        back.setScale(128, 128); // Screen is 1280x1280. Image is 10x10.
        sf::Music m;
        m.openFromFile("audio/10_LEVELFAILED.ogg");
        m.setVolume(100);
        m.setLoop(false);
        m.play();
        sfmlText t;
        t.text.setString("Game Over");
        t.text.setCharacterSize(100);
        t.text.setOrigin(t.text.getGlobalBounds().width / 2, t.text.getGlobalBounds().height / 2);
        t.text.setPosition(640, 440);
        while(selection == -1)
        {            
            if(hilTimer.getElapsedTime() > sf::seconds(0.001666)) // 60 fps means 16.66 ms per frame
            {
                if(hil.getColor().g >= 253 || hil.getColor().b >= 253)
                    hilColorDirection = false;
                else if(hil.getColor().g <= 2 || hil.getColor().b <= 2)
                    hilColorDirection = true;
                if(hilColorDirection == true) // So forwards
                    hil.setColor(sf::Color(255, hil.getColor().g + 1, hil.getColor().b + 1));
                else
                    hil.setColor(sf::Color(255, hil.getColor().g - 1, hil.getColor().b - 1));
                hilTimer.restart();
            }
            sf::Event e;
            while (window.pollEvent(e))
            {                  
                if (e.type == sf::Event::Closed)
                {
                    selection = 10;
                    return;
                }
                if (e.type == sf::Event::KeyReleased)
                {
                    if(e.key.code == sf::Keyboard::Enter)
                        selection = 0;
                    if(e.key.code == sf::Keyboard::Escape)
                        selection = 0;
                }
            }
            options[0].text.setPosition(640, 1160);
            sel.setPosition(options[0].text.getPosition().x, options[0].text.getPosition().y + 10);
            hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
            window.draw(hil);
            window.draw(sel);
            window.draw(options[0].text);
            window.draw(t.text);
            window.display();
            window.clear();
        }
        totalScore = 0;
    }
};
class PauseMenu: public Menu {
public:

    std::string optionList[7];

    PauseMenu(sf::RenderWindow& window, int rememberSelection, bool& debugActive, MenuBG& menuBG):
    optionList{"Resume", "Options", "Instructions", "High Scores", "Main Menu", "Exit", "Debug"},
    Menu(7)
    {
        Menu::MenuTexts(optionList);
        char tempChar;
        int position = 0;
        std::string code = "VULCAN";
        highlight = rememberSelection;

        while(selection == -1)
        {
            if(menuBG.time.getElapsedTime() > sf::seconds(0.033333)) // 30 fps means 33 ms per frame.
            {
                menuBG.bgX++;
                if(menuBG.bgX >= 30){menuBG.bgY++;menuBG.bgX = 0;}
                if(menuBG.bgY >= 8){menuBG.bgY = 0;}
                if(menuBG.bgX >= 29 && menuBG.bgY >= 7){menuBG.bgX = 0;menuBG.bgY = 0;}
                menuBG.time.restart();
            }
            menuBG.bg.setTextureRect(sf::IntRect((256*menuBG.bgX), (256*menuBG.bgY), 256, 256));
            if(hilTimer.getElapsedTime() > sf::seconds(0.001666)) // 60 fps means 16.66 ms per frame
            {
                if(hil.getColor().g >= 253 || hil.getColor().b >= 253)
                    hilColorDirection = false;
                else if(hil.getColor().g <= 2 || hil.getColor().b <= 2)
                    hilColorDirection = true;
                if(hilColorDirection == true) // So forwards
                    hil.setColor(sf::Color(255, hil.getColor().g + 1, hil.getColor().b + 1));
                else
                    hil.setColor(sf::Color(255, hil.getColor().g - 1, hil.getColor().b - 1));
                hilTimer.restart();
            }
            if(isJumping == true)
            {
                if(options[highlight].text.getPosition().y < originalY - 25)
                {    
                    jumpDirection = false;
                    options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y + 2);
                }
                if(jumpTimer.getElapsedTime() > sf::seconds(0.16666))
                {
                    if(jumpDirection == true)
                    {
                        // std::cout << "Up, New: " << options[highlight].text.getPosition().y << ", Original: " << originalY << std::endl;
                        options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y - 2);
                    }
                    else 
                    {
                        // std::cout << "Down, New: " << options[highlight].text.getPosition().y << ", Original: " << originalY << std::endl;
                        options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y + 2);
                    }
                    jumpTimer.restart();
                }
            }
            sf::Event e;
            while (window.pollEvent(e))
            {                  
                if (e.type == sf::Event::Closed)
                {
                    selection = 10;
                    return;
                }
                if (e.type == sf::Event::TextEntered) 
                {
                    tempChar = e.text.unicode;
                    if(int(tempChar) >= 97 && int(tempChar) <= 122)
                        tempChar -= 32;

                    if(tempChar != code[position])
                        position = 0;
                    else
                        position++;
                }
                if (e.type == sf::Event::KeyReleased)
                {
                    if(e.key.code == sf::Keyboard::Up)
                        highlight -= 1;
                    if(e.key.code == sf::Keyboard::Down)
                        highlight += 1;
                    if(e.key.code == sf::Keyboard::Escape)
                        selection = 3;
                    if(e.key.code == sf::Keyboard::P)
                        selection = 0;

                    if(highlight == -1)
                    {
                        if(debugActive == true)
                            highlight = 6;
                        else
                            highlight = 5;
                    }
                    if(highlight == 6)
                    {
                        if(debugActive == false)
                            highlight = 0;
                    }
                    if(highlight == 7)
                        highlight = 0;
                    if(e.key.code == sf::Keyboard::Enter)
                        selection = highlight;
                    if(e.key.code == sf::Keyboard::Up || e.key.code == sf::Keyboard::Down)
                    {
                        isJumping = true;
                        jumpDirection = true;
                        originalY = 0;
                    }
                }
            }
            if(position == code.length())
            {
                sf::Clock c;
                while(c.getElapsedTime().asSeconds() < 1)
                {
                    window.clear();
                    window.draw(menuBG.bg);
                    sfmlText temp;
                    temp.text.setString("Cheat code confirmed!");
                    temp.text.setOrigin(temp.text.getGlobalBounds().width / 2, temp.text.getGlobalBounds().height / 2);
                    temp.text.setPosition(640, 630);
                    sel.setPosition(640, 640);
                    window.draw(sel);
                    sel.setPosition(340, 640);
                    window.draw(sel);
                    sel.setPosition(940, 640);
                    window.draw(sel);
                    window.draw(temp.text);
                    position = 0;
                    debugActive = true;
                    window.display();
                }
            }

            for(int i = 0; i < numOp; i++)
            {
                if(!(i == highlight && isJumping == true))
                    options[i].text.setPosition(640, (i+1)*(1280.0/8));
                if(debugActive == false && i == 6)
                    continue;
                sel.setPosition(options[i].text.getPosition().x, options[i].text.getPosition().y + 10);
                hil.setPosition(sel.getPosition().x + 15, sel.getPosition().y + 15);
                if(i == highlight)
                {
                    window.draw(hil);
                    if(isJumping == true)
                    {
                        if(originalY == 0)
                            originalY = options[highlight].text.getPosition().y;
                        options[highlight].text.setPosition(options[highlight].text.getPosition().x, options[highlight].text.getPosition().y - 2);
                    }
                }
                window.draw(sel);
                window.draw(options[i].text);
            }
            window.display();
            window.clear();
            window.draw(menuBG.bg);
        }
    }
};
// Main Menu:
    // Start (Goes to Level Select Screen)
        // Level 1
        // Level 2
        // Level 3
        // Boss 1
        // Boss 2
        // Boss 3
        // Return to Main Menu
            // Level Settings (All Levels):
                // Difficulty [EASY/MEDIUM/HARD]
                // Bosses [ON/OFF]
                // Return to Level Select Screen
    // Options
        // Volume Control
        // Change Background
        // Steering Sensitivity
        // Return to Main Menu
    // Help
        // Return to Main Menu
    // High Scores
        // Return to Main Menu
    // Exit
    // Debug Menu
        // Change Speed
        // Change Damage
        // UnlockAllThings
        // Edit Lives
        // Trigger God-mode
        // Trigger Power-Up
        // Trigger Fire
        // Return to Main Menu

// Pause Menu:
    // Resume
    // Return to Main Menu
    // Options
        // Volume Control
        // Change Background
        // Steering Sensitivity
    // Help
    // High Scores
    // Exit
    // Debug Menu
        // Change Speed
        // Change Damage
        // UnlockAllThings
        // Edit Lives
        // Trigger God-mode
        // Trigger Power-Up
        // Trigger Fire

// Game Over:
    // (Enter Name)
    // (Shows Hi-Scores and Medals)
    // (Shows Time Played)
    // Return to Main Menu
    // Exit



#endif