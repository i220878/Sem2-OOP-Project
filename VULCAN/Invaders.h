#ifndef INVADERS_H_
#define INVADERS_H_

#include "Bomb.h"
#include "Enemy.h"

class Invaders: public Enemy
{
public:
	Invaders();
	virtual void initialize(float offset) = 0;
	virtual void fire(float x, float y);
	virtual bool isOutside();
	virtual void move(sf::Time dT) {}
	virtual ~Invaders();
};

#endif