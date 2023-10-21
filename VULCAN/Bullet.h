#ifndef BULLET_H_
#define BULLET_H_

#include <SFML/Graphics.hpp>
#include "Enemy.h"

class Bullet
{
private:

	int damage;
	float velocity;
	int angle; // From 0 to 359

	float posX;
	float posY;

	sf::Sprite b;
	sf::Texture t;
	sf::Clock textureTimer;
	
public:
	int getDamage() const {
		return this->damage;
	}
	void setDamage(const int& damage) {
		this->damage = damage;
	}
	float getVelocity() const {
		return this->velocity;
	}
	void setVelocity(const float& velocity) {
		this->velocity = velocity;
	}
	sf::Sprite getB() const {
		return this->b;
	}
	void setP(const sf::Sprite& b) {
		this->b = b;
	}
	int getAngle() const {
		return this->angle;
	}
	void setAngle(const int& angle) {
		this->angle = angle;
	}
	float getPosX() const {
		return this->posX;
	}
	void setPosX(const float& posX) {
		this->posX = posX;
	}
	float getPosY() const {
		return this->posY;
	}
	void setPosY(const float& posY) {
		this->posY = posY;
	}

	Bullet() {posX = 10000; angle = 0; posY = 10000;} // Dummy constructor. Used so an Array can be made.
	Bullet(int angle, int speed, int damage, int posX, int posY);
	void operator=(const Bullet& copy); // Shallow copy would've been fine but textures weren't working.
	// Turns out SFML takes a Pointer to a texture rather than loading it. So a manual deep copy is needed.
	
	void move(int angle, sf::Time dT);
	bool isOutside();
	void setCoords(float x, float y);

	~Bullet() {}
};

#endif