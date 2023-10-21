#include "Dragon.h"
#include "Bomb.h"
#include "Enemy.h"
#include <cmath>
#include <math.h>
#include <iostream>
#include <string>

void Dragon::initialize(float offset)
{
	Enemy::setLives(3);
	Enemy::setDamage(1);
	Enemy::setBombs(new Bomb[400]);
	Enemy::getBombs()[0] = Bomb(0, Enemy::getDamage(), 10000, 10000 - 32);
	Enemy::setSize(1);
    enemyCount++;

    Enemy::setVariant(4);
	Enemy::textureLoad("img/boss/Dragon.png");
	Enemy::textureSet();
    Enemy::originSet(480, 256);
    Enemy::positionSet(Enemy::getPosX(), Enemy::getPosY());
    // Image size is 960x512
    Enemy::setFireTime(sf::Time(sf::milliseconds((int)offset)));
    Enemy::setFireRate(1);
}
void Dragon::fire(float playerX, float playerY)
{
	float angle = 0;

	int bombsMax = 400;

	if(Enemy::getSize() + 3 < bombsMax)
	{
		angle = 90 + atan((playerY - Enemy::getPosY() - 80) / (playerX - Enemy::getPosX() + 310)) * (180/3.1416);
		if(playerX - Enemy::getPosX() + 310 < 0)
			angle += 180;
		Enemy::getBombs()[Enemy::getSize()] = Bomb(angle, Enemy::getDamage(), Enemy::getPosX() - 310, Enemy::getPosY() + 80);
        Enemy::getBombs()[Enemy::getSize()].setVelocity(10);

		angle = 90 + atan((playerY - Enemy::getPosY() - 45) / (playerX - Enemy::getPosX() + 185)) * (180/3.1416);
		if(playerX - Enemy::getPosX() + 185 < 0)
			angle += 180;
		Enemy::getBombs()[Enemy::getSize() + 1] = Bomb(angle, Enemy::getDamage(), Enemy::getPosX() - 185, Enemy::getPosY() + 45);
        Enemy::getBombs()[Enemy::getSize() + 1].setVelocity(10);

		angle = 90 + atan((playerY - Enemy::getPosY() - 45) / (playerX - Enemy::getPosX() - 185)) * (180/3.1416);
		if(playerX - Enemy::getPosX() - 185 < 0)
			angle += 180;
		Enemy::getBombs()[Enemy::getSize() + 2] = Bomb(angle, Enemy::getDamage(), Enemy::getPosX() + 185, Enemy::getPosY() + 45);
        Enemy::getBombs()[Enemy::getSize() + 2].setVelocity(10);
		
		angle = 90 + atan((playerY - Enemy::getPosY() - 80) / (playerX - Enemy::getPosX() - 310)) * (180/3.1416);
		if(playerX - Enemy::getPosX() - 310 < 0)
			angle += 180;
		Enemy::getBombs()[Enemy::getSize() + 3] = Bomb(angle, Enemy::getDamage(), Enemy::getPosX() + 310, Enemy::getPosY() + 80);
        Enemy::getBombs()[Enemy::getSize() + 3].setVelocity(10);
		Enemy::setSize(Enemy::getSize() + 4);
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
	Enemy::setSize(c);


	angle = 90 + atan((playerY - Enemy::getPosY() - 80) / (playerX - Enemy::getPosX() + 310)) * (180/3.1416);
	if(playerX - Enemy::getPosX() + 310 < 0)
		angle += 180;
	Enemy::getBombs()[Enemy::getSize()] = Bomb(angle, Enemy::getDamage(), Enemy::getPosX() - 310, Enemy::getPosY() + 80);
	Enemy::getBombs()[Enemy::getSize()].setVelocity(10);

	angle = 90 + atan((playerY - Enemy::getPosY() - 45) / (playerX - Enemy::getPosX() + 185)) * (180/3.1416);
	if(playerX - Enemy::getPosX() + 185 < 0)
		angle += 180;
	Enemy::getBombs()[Enemy::getSize() + 1] = Bomb(angle, Enemy::getDamage(), Enemy::getPosX() - 185, Enemy::getPosY() + 45);
	Enemy::getBombs()[Enemy::getSize() + 1].setVelocity(10);

	angle = 90 + atan((playerY - Enemy::getPosY() - 45) / (playerX - Enemy::getPosX() - 185)) * (180/3.1416);
	if(playerX - Enemy::getPosX() - 185 < 0)
		angle += 180;
	Enemy::getBombs()[Enemy::getSize() + 2] = Bomb(angle, Enemy::getDamage(), Enemy::getPosX() + 185, Enemy::getPosY() + 45);
	Enemy::getBombs()[Enemy::getSize() + 2].setVelocity(10);
	
	angle = 90 + atan((playerY - Enemy::getPosY() - 80) / (playerX - Enemy::getPosX() - 310)) * (180/3.1416);
	if(playerX - Enemy::getPosX() - 310 < 0)
		angle += 180;
	Enemy::getBombs()[Enemy::getSize() + 3] = Bomb(angle, Enemy::getDamage(), Enemy::getPosX() + 310, Enemy::getPosY() + 80);
	Enemy::getBombs()[Enemy::getSize() + 3].setVelocity(10);


	Enemy::setSize(Enemy::getSize() + 4);
	Enemy::deleteBombs();
	Enemy::setBombs(temp);
}
void Dragon::move(sf::Time dT)
{

}
bool Dragon::isOutside()
{
    return true;
}
Dragon::~Dragon()
{

}