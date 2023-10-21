#include "Bomb.h"
#include <math.h>
#include <iostream>

Bomb::Bomb(float angle, int damage, float posX, float posY)
{
	this->velocity = 1;
	this->damage = damage;
	t.loadFromFile("img/Bomb.png");

	b.setTexture(t);

	this->posX = posX;
	this->posY = posY + 32;
	b.setOrigin(16, 16);
	b.setTextureRect(sf::IntRect(0, 0, 32, 32));
	b.setPosition(this->posX, this->posY);

	this->angle = angle;
}
void Bomb::move(float dT)
{
	float dispV = cos(angle * (3.1416/180)) / 10;
	dispV *= -1;
	float dispH = sin(angle * (3.1416/180)) / 10;
	dispH *= (velocity * 5);
	dispV *= (velocity * 5);
	b.setPosition(posX += (dispH * dT * 250), posY += (dispV * dT * 250));
	if(textureTimer.getElapsedTime().asSeconds() > 0.1)
	{
		if(b.getTextureRect().left >= 96)
			b.setTextureRect(sf::IntRect(0, 0, 32, 32));
		else
			b.setTextureRect(sf::IntRect(b.getTextureRect().left + 32, 0, 32, 32));
		textureTimer.restart();
	}
}
void Bomb::operator=(const Bomb& copy)
{
	damage = copy.getDamage();
	velocity = copy.getVelocity();
	posX = copy.getPosX();
	posY = copy.getPosY();
	angle = copy.getAngle();
	t.loadFromFile("img/Bomb.png");

	b.setTexture(t);
	b.setTextureRect(sf::IntRect(0, 0, 32, 32));

	b.setOrigin(16, 16);
	b.setPosition(posX, posY);
}
bool Bomb::isOutside()
{
	if(posX > 1400 || posX < -100 || posY < -100 || posY > 1400)
		return true;
	return false;
}
void Bomb::moveStraightDown(bool direction, sf::Time dT)
{
	if(direction == true)
		posX -= dT.asSeconds() * 100;
	else
		posX += dT.asSeconds() * 100;
	posY += dT.asSeconds() * 1000;
	b.setPosition(posX, posY);
}
void Bomb::move(float x, float y)
{
	b.move(x, y);
	posX += x;
	posY += y;
}
void Bomb::setCoords(float x, float y)
{
	b.setPosition(x, y);
	posX = x;
	posY = y;
}