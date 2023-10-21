#include "Alpha.h"
#include <SFML/System/Time.hpp>
#include <iostream>

void Alpha::initialize(float offset)
{
    Enemy::setVariant(0);
	Enemy::textureLoad("spritesheet/Invaders.png");
	Enemy::textureSet();
    Enemy::rectSet(0, 0, 72, 72);
    Enemy::positionSet(Enemy::getPosX(), Enemy::getPosY());
    Enemy::originSet(36, 36);
    // std::cout << "ALPHA: " << offset << std::endl;
    Enemy::setFireTime(sf::Time(sf::milliseconds((int)offset)));
    Enemy::setFireRate(5);
    Enemy::initalizeFade(0);
}