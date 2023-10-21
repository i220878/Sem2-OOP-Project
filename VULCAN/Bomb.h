
#ifndef BOMB_H_
#define BOMB_H_

#include <SFML/Graphics.hpp>

class Bomb
{
private:

	int damage;
	float velocity;

    // No angle this time as it all flows downward for regular goons.
    // Gonna need angles and extra stuff for Boss and Random encounters though.

	float posX;
	float posY;
	float angle;

	sf::Sprite b;
	sf::Texture t;

	sf::Clock textureTimer;
    
public:
	float getAngle() const {
		return this->angle;
	}
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
    
	Bomb() {} // Dummy constructor. Used so an Array can be made.
	Bomb(float angle, int damage, float posX, float posY);
	void operator=(const Bomb& copy);
	void move(float dT);
	void move(float x, float y);
	void moveStraightDown(bool direction, sf::Time dT);
	void setCoords(float x, float y);
	bool isOutside();
};

#endif