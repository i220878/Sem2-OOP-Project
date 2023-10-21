#include "Monster.h"
#include "Bomb.h"
#include "Enemy.h"
#include <cmath>
#include <math.h>
#include <iostream>
#include <string>

void Monster::initialize(float offset)
{
	beamTex.loadFromFile("spritesheet/beamSpriteSheet.png");
	beam.setTexture(beamTex);
	beam.setTextureRect(sf::IntRect(0, 0, 150, 1280));
	beam.setOrigin(75, 45);
	beamClock.restart();
	Enemy::setLives(3);
	Enemy::setDamage(1);
	Enemy::setBombs(new Bomb[400]);
	Enemy::getBombs()[0] = Bomb(0, Enemy::getDamage(), 10000, 10000 - 32);
	Enemy::setSize(1);
    enemyCount++;

    Enemy::setVariant(3);
	Enemy::textureLoad("img/boss/Monster.png");
	Enemy::textureSet();
    Enemy::originSet(256, 168);
    Enemy::positionSet(Enemy::getPosX(), Enemy::getPosY());
    // Image size is 512x336
    // std::cout << "ALPHA: " << offset << std::endl;
    Enemy::setFireTime(sf::Time(sf::milliseconds((int)offset)));
    Enemy::setFireRate(1);
	Enemy::setDirection(rand() % 2);
}
void Monster::fire(float playerX, float playerY)
{
	// float angle = 90 + atan((playerY - Enemy::getPosY()) / (playerX - Enemy::getPosX())) * (180/3.1416);
	// if(playerX - Enemy::getPosX() < 0)
	// 	angle += 180;
}
void Monster::move(sf::Time dT)
{
	// std::cout << "BEFOREdirection: " << getDirection() << ", position: " << getPosX() << std::endl;
	if(getDirection() == true && getPosX() > 1080)
		setDirection(false);
	else if(getDirection() == false && getPosX() < 200)
		setDirection(true);
	// std::cout << "direction: " << getDirection() << ", position: " << getPosX() << "\n\n";
	if(getDirection() == true)
		Enemy::positionSet((getPosX() + dT.asSeconds() * 100), getPosY());
	else
		Enemy::positionSet((getPosX() - dT.asSeconds() * 100), getPosY());
}
bool Monster::isOutside()
{
    return true;
}
Monster::~Monster()
{

}