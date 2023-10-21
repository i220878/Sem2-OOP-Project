#include "Mothership.h"
#include "Bomb.h"
#include "Enemy.h"
#include <cmath>
#include <math.h>
#include <iostream>
#include <string>

void Mothership::initialize(float offset)
{
	Enemy::setLives(3);
	Enemy::setDamage(1);
	Enemy::setBombs(new Bomb[20]);
	Enemy::getBombs()[0] = Bomb(0, Enemy::getDamage(), 10000, 10000 - 32);
	Enemy::setSize(1);
    enemyCount++;

    eyeTex.loadFromFile("img/boss/Mothership_Eye.png");
    eye.setTexture(eyeTex);
    Enemy::setVariant(5);
	Enemy::textureLoad("img/boss/Mothership_Eyeless.png");
	Enemy::textureSet();
    Enemy::originSet(480, 256);
    Enemy::positionSet(Enemy::getPosX(), Enemy::getPosY());
	eye.setPosition(Enemy::getPosX(), Enemy::getPosY() - 20);
	eye.setOrigin(52, 52);
    Enemy::rectSet(0, 0, 960, 512);
    // Each mothership Rect is 960x512.
    // std::cout << "ALPHA: " << offset << std::endl;
    Enemy::setFireTime(sf::Time(sf::milliseconds((int)offset)));
    Enemy::setFireRate(15);
}
void Mothership::fire(float playerX, float playerY)
{
	float angle = 90 + atan((playerY - Enemy::getPosY()) / (playerX - Enemy::getPosX())) * (180/3.1416);
	if(playerX - Enemy::getPosX() < 0)
		angle += 180;

	int bombsMax = 20;

	if(Enemy::getSize() < bombsMax)
	{
		Enemy::getBombs()[Enemy::getSize()] = Bomb(angle, Enemy::getDamage(), Enemy::getPosX(), Enemy::getPosY());
        Enemy::getBombs()[Enemy::getSize()].setVelocity(5);
		Enemy::setSize(Enemy::getSize() + 1);
		return;
	}
	Bomb* temp = new Bomb[bombsMax];
	int c = 0;
	for(int i = 0; i < bombsMax; i++)
	{
		if(Enemy::getBombs()[i].isOutside())
			continue;
		temp[c++] = Enemy::getBombs()[i];
	}
	temp[c] = Bomb(angle, Enemy::getDamage(), Enemy::getPosX(), Enemy::getPosY());
    temp[c].setVelocity(5);
	Enemy::deleteBombs();
	Enemy::setBombs(temp);
	Enemy::setSize(c + 1);
}
void Mothership::move(sf::Time dT)
{

}
bool Mothership::isOutside()
{
    return true;
}
Mothership::~Mothership()
{

}