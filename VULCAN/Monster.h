#ifndef MONSTER_H_
#define MONSTER_H_

#include "Bomb.h"
#include "Enemy.h"

class Monster: public Enemy
{
public:
	virtual void initialize(float offset);
	virtual void fire(float x, float y);
	virtual bool isOutside();
    virtual void move(sf::Time dT);
	virtual ~Monster();
};

#endif