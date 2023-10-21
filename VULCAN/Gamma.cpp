#include "Gamma.h"
#include <SFML/System/Time.hpp>
#include <iostream>

void Gamma::initialize(float offset)
{
    Enemy::setVariant(2);
	Enemy::textureLoad("spritesheet/Invaders.png");
	Enemy::textureSet();
    Enemy::rectSet(144, 0, 72, 72);
    Enemy::positionSet(Enemy::getPosX(), Enemy::getPosY());
    Enemy::originSet(36, 36);
    // std::cout << "GAMMA: " << offset << std::endl;
    Enemy::setFireTime(sf::Time(sf::milliseconds((int)offset)));
    Enemy::setFireRate(2);
    Enemy::initalizeFade(2);
}