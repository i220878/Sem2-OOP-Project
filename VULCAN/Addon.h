#ifndef ADDON_H_
#define ADDON_H_

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Spaceship.h"
#include "HUD.h"

class Addon {
// This class is meant to be abstract.
// An object of this isn't supposed to be made.

// There's multiple addons. Some compulsory, some which I'm doing for bonus. Here's the list:
// Power-Up (7 Directions and Godmode)
// Fire Bullets
// Danger Sign (For SOME REASON this is here. This thing has more functionality of a bomb)
// Lives (This one makes sense, not in the sense of lives in general, but as a pickup.)

// Optional ones I want to implement:

// Speed
// Damage

// Heroic Blink is a modification that is only usable when the ship has Power-Up Enabled.
// Heroic Blink lasts two seconds and can only be triggered if more than 1 second of the Power-Up is left.
// It quips the ship with a Battering Ram. Choosing to use this will let the player align to a Row or Column, and
// move continuously throughout the duration, causing damage to any entity present with the area.

// Speed is a pickup that increases the movement speed and bullet fire rate of the player.
// Reaching a Speed of 4 lets "Asteroids" movement be unlocked, where the player can shoot at any angle and
// utilize momentum for elaborate movements.

// Damage increases the amount of damage enemies take when hit by a bullet.

// All of these sub-classes share some features.
// 1) They're pickups in stage, dropped by enemies. So although the spaceship USES them, their existence is determined
// by a random chance when an enemy is destroyed.
// 2) When dropping, they will move down the screen and can be collided with (All except for) or dodged (Danger Sign)
// 3) When collided with, they will perform an action to the Player and modify them.
// Specific IF checks and bool's will be required to apply these modifications.
// Some will be permanent (Speed, Damage, Lives, Missile. They stay until loss of life), and some will
// be time limited (Power-Up will last only 5 seconds, as will Fire Bullets)

// Addons.png:
// speed
// fire
// sword
// 1-up
// star

protected:

    float posX;
    float posY;
    sf::Sprite a;
    sf::Texture t;
    bool isDanger;

    sf::Clock textureTimer;

public:

    bool getDanger() const {
        return isDanger;
    }
    sf::Sprite getA() {
        return a;
    }
    bool collision(Spaceship& player) // This is what checks if the magic can happen
    {
        float absX = 0;
        float absY = 0;

        absX = posX - player.getPosX();
        if(absX < 0)
            absX = -absX;

        absY = posY - player.getPosY();
        if(absY < 0)
            absY = -absY;

        if(absX < 48 && absY < 48)
        {
            return true;
        }
        return false;
    }
    virtual void interaction(Spaceship& player) = 0; // This is where the magic happens.
    void move(sf::Time dT)
    {
        if(textureTimer.getElapsedTime().asSeconds() > 0.1)
        {
            if(a.getTextureRect().left >= 96)
                a.setTextureRect(sf::IntRect(0, a.getTextureRect().top, 32, 32));
            else
                a.setTextureRect(sf::IntRect(a.getTextureRect().left + 32, a.getTextureRect().top, 32, 32));
            textureTimer.restart();
        }
        a.setPosition(posX, posY += (dT.asSeconds() * 100));
    }
    float getY() const {
        return posY;
    }
    virtual ~Addon() {}
};

// public, inline, functionality in main


class Speed: public Addon {
public:

    Speed() {}
    Speed(float x, float y)
    {
        isDanger = false;
        posX = x;
        posY = y;
        t.loadFromFile("spritesheet/Addons.png");
        a.setTexture(t);
        a.setTextureRect(sf::IntRect(0, 0, 32, 32));
        a.setOrigin(16,16);
        a.setPosition(posX, posY);
    }
    virtual void interaction(Spaceship& player)
    {
        std::cout << "interacting." << std::endl;
        player.setSpeed(player.getSpeed() + 1);
    }
};
class Fire: public Addon {
public:

    Fire() {}
    Fire(float x, float y)
    {
        isDanger = false;
        posX = x;
        posY = y;
        t.loadFromFile("spritesheet/Addons.png");
        a.setTexture(t);
        a.setTextureRect(sf::IntRect(0, 32, 32, 32));
        a.setOrigin(16,16);
        a.setPosition(posX, posY);
    }
    virtual void interaction(Spaceship& player)
    {
        std::cout << "interacting." << std::endl;
        player.becomeOnFire(true);
    }
};
class Damage: public Addon {
public:

    Damage() {}
    Damage(float x, float y)
    {
        isDanger = false;
        posX = x;
        posY = y;
        t.loadFromFile("spritesheet/Addons.png");
        a.setTexture(t);
        a.setTextureRect(sf::IntRect(0, 64, 32, 32));
        a.setOrigin(16,16);
        a.setPosition(posX, posY);
    }
    virtual void interaction(Spaceship& player)
    {
        std::cout << "interacting." << std::endl;
        player.setDamage(player.getDamage() + 1);
    }
};
class Life: public Addon {
public:

    Life() {}
    Life(float x, float y)
    {
        isDanger = false;
        posX = x;
        posY = y;
        t.loadFromFile("spritesheet/Addons.png");
        a.setTexture(t);
        a.setTextureRect(sf::IntRect(0, 96, 32, 32));
        a.setOrigin(16,16);
        a.setPosition(posX, posY);
    }
    virtual void interaction(Spaceship& player)
    {
        std::cout << "interacting." << std::endl;
        player.setLives(player.getLives() + 1);
    }
};
class Power_Up: public Addon {
public:

    Power_Up() {}
    Power_Up(float x, float y)
    {
        isDanger = false;
        posX = x;
        posY = y;
        t.loadFromFile("spritesheet/Addons.png");
        a.setTexture(t);
        a.setTextureRect(sf::IntRect(0, 128, 32, 32));
        a.setOrigin(16,16);
        a.setPosition(posX, posY);
    }
    virtual void interaction(Spaceship& player)
    {
        std::cout << "interacting." << std::endl;
        player.setEmpowered(true);
    }
};
class Danger: public Addon {
public:

    Danger() {}
    Danger(float x, float y)
    {
        isDanger = true;
        posX = x;
        posY = y;
        t.loadFromFile("spritesheet/Addons.png");
        a.setTexture(t);
        a.setTextureRect(sf::IntRect(0, 80, 32, 32));
        a.setOrigin(16,16);
        a.setPosition(posX, posY);
    }
    virtual void interaction(Spaceship& player)
    {
        std::cout << "interacting." << std::endl;
        player.setLives(player.getLives() - 1);
    }
};

#endif