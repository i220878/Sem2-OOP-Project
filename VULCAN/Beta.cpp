#include "Beta.h"
#include <SFML/System/Time.hpp>
#include <iostream>

void Beta::initialize(float offset)
{
    Enemy::setVariant(1);
	Enemy::textureLoad("spritesheet/Invaders.png");
	Enemy::textureSet();
    Enemy::rectSet(72, 0, 72, 72);
    Enemy::positionSet(Enemy::getPosX(), Enemy::getPosY());
    Enemy::originSet(36, 36);
    // std::cout << "BETA: " << offset << std::endl;
    Enemy::setFireTime(sf::Time(sf::milliseconds((int)offset)));
    Enemy::setFireRate(3);
    Enemy::initalizeFade(1);
}