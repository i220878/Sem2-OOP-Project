#include "Invaders.h"
#include "Bomb.h"
#include "Enemy.h"
#include <cmath>
#include <math.h>
#include <iostream>
#include <string>

Invaders::Invaders()
{
	Enemy::setLives(3);
	// Enemy::setPosX(64 + 128 * (enemyCount % 10));
	// Enemy::setPosY(64 + 128 * (enemyCount / 10));
	Enemy::setDamage(1);
	Enemy::setBombs(new Bomb[20]);
	Enemy::getBombs()[0] = Bomb(0, Enemy::getDamage(), 10000, 10000 - 32);
	Enemy::setSize(1);
    enemyCount++;
	Enemy::setDirection(true);

	// std::cout << Enemy::getP().getPosition().x << ", " << Enemy::getP().getPosition().y << std::endl;

		// Enemy::getT().loadFromFile("spritesheet/Invaders.png");
		// Enemy::getP().setTexture(Enemy::getT());
		// Enemy::getP().setOrigin(36, 36);
		// Enemy::getP().setTextureRect(sf::IntRect(0, 0, 72, 72));
}
void Invaders::fire(float playerX, float playerY)
{
	float angle = 90 + atan((playerY - Enemy::getPosY()) / (playerX - Enemy::getPosX())) * (180/3.1416);
	if(playerX - Enemy::getPosX() < 0)
		angle += 180;

	int bombsMax = 20;

	if(Enemy::getSize() < bombsMax)
	{
		Enemy::getBombs()[Enemy::getSize()] = Bomb(angle, Enemy::getDamage(), Enemy::getPosX(), Enemy::getPosY());
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
	Enemy::deleteBombs();
	Enemy::setBombs(temp);
	Enemy::setSize(c + 1);
}
bool Invaders::isOutside()
{
	if(Enemy::getPosX() > 1400 || Enemy::getPosX() < -100 || Enemy::getPosY() < -100 || Enemy::getPosY() > 1400)
		return true;
	return false;
}
Invaders::~Invaders()
{
	enemyCount--;
}