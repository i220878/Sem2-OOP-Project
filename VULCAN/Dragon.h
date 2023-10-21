#ifndef DRAGON_H_
#define DRAGON_H_

#include "Bomb.h"
#include "Enemy.h"

class Dragon: public Enemy
{
public:
	virtual void initialize(float offset);
	virtual void fire(float x, float y);
	virtual bool isOutside();
    virtual void move(sf::Time dT);
	virtual ~Dragon();
};

#endif