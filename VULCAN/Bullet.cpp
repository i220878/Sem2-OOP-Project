#include "Bullet.h"
#include <math.h>
#include <iostream>

Bullet::Bullet(int angle, int speed, int damage, int posX, int posY)
// I would definitely pass the Spaceship as an argument into the constructor, but then
// Bullet and Spaceship would recursively be including each other.
{
	this->angle = angle;
	this->velocity = speed;
	this->damage = damage;

	// std::cout << "Bullet angle: " << this->angle << ", Ship angle: " << angle << std::endl;

	std::string path = "img/degrees360Bullet/0";
	if(angle < 100)
		path += "0";
	if(angle < 10)
		path += "0";
	path += std::to_string(angle);
	path += ".png";
	// std::cout << path << std::endl;
	t.loadFromFile(path);

	b.setTexture(t);

	float radius = 16; // Of texture file.
	float dispV = 0;
	float dispH = 0;
	dispV = radius * cos(angle * (3.1416/180));
	dispV *= -1; // Because upward is negative. 0,0 is top left.
	dispH = radius * sin(angle * (3.1416/180));
	this->posX = posX + dispH;
	this->posY = posY + dispV;

	// b.setOrigin(32 - dispH, 32 - dispV); // Set to the tail of the bullet.
	// Set to 32 + dispH and 32 + dispV to set to head of the bullet.
	// std::cout << "Origin: (" << 32 - dispH << ", " << 32 - dispV << ")\n";

	b.setTextureRect(sf::IntRect(0, 0, 32, 32));
	b.setOrigin(16 - dispH, 16 - dispV);
	b.setPosition(this->posX, this->posY);
	// std::cout << "posX: " << posX << ", posY: " << posY << "\nthis->posX: " << this->posX << ", this->posY: " << this->posY << "\n";

	dispV = -32 * cos(angle * (3.1416/180)); 
	dispH = 32 * sin(angle * (3.1416/180));
	// 64 is the length from the center of the ship.
}
void Bullet::move(int angle, sf::Time dT)
{
	// 0 is up. 90 is Right. 180 is Down. 270 is Left.
	// That doesn't really match with the Quadrant we did in Calculus but whatever. This is my version.
	float dispV = cos(angle * (3.1416/180)) / 10;
	dispV *= -1;
	float dispH = sin(angle * (3.1416/180)) / 10;
	dispV *= (velocity * 10);
	dispH *= (velocity * 10);
	if(textureTimer.getElapsedTime().asSeconds() > 0.1)
	{
		if(b.getTextureRect().left >= 96)
			b.setTextureRect(sf::IntRect(0, 0, 32, 32));
		else
			b.setTextureRect(sf::IntRect(b.getTextureRect().left + 32, 0, 32, 32));
		textureTimer.restart();
	}
	// std::cout << "dispV: " << dispV << ", dispH: " << dispH << ", angle: " << angle << std::endl;
	b.setPosition(posX += (dispH * dT.asSeconds() * 250), posY += (dispV * dT.asSeconds() * 250));
	// std::cout << "posX: " << posX << ", posY: " << posY << ", HBO_X: " << hitBoxOrigin_X << ", HBO_Y: " << hitBoxOrigin_Y << "\n";
}
void Bullet::operator=(const Bullet& copy)
{
	damage = copy.getDamage();
	velocity = copy.getVelocity();
	angle = copy.getAngle();
	posX = copy.getPosX();
	posY = copy.getPosY();

	std::string path = "img/degrees360Bullet/0";
	if(angle < 100)
		path += "0";
	if(angle < 10)
		path += "0";
	path += std::to_string(angle);
	path += ".png";
	// std::cout << path << std::endl;
	t.loadFromFile(path);

	b.setTexture(t);

	float radius = 32; // Of texture file.
	float dispV = 0;
	float dispH = 0;
	dispV = radius * cos(angle * (3.1416/180));
	dispV *= -1; // Because upward is negative. 0,0 is top left.
	dispH = radius * sin(angle * (3.1416/180));
	b.setOrigin(16 - dispH, 16 - dispV);
	b.setPosition(posX, posY);
	b.setTextureRect(sf::IntRect(0, 0, 32, 32));
}
bool Bullet::isOutside()
{
	if(posX > 1400 || posX < -100 || posY < -100 || posY > 1400)
		return true;
	return false;
}
void Bullet::setCoords(float x, float y)
{
	b.setPosition(x, y);
	posX = x;
	posY = y;
}