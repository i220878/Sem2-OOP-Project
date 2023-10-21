#ifndef MOTHERSHIP_H_
#define MOTHERSHIP_H_

#include "Bomb.h"
#include "Enemy.h"

class Mothership: public Enemy
{
public:
	virtual void initialize(float offset);
	virtual void fire(float x, float y);
	virtual bool isOutside();
    virtual void move(sf::Time dT);
    virtual sf::Sprite getEye()
    {
        return eye;
    }
	virtual ~Mothership();
};

#endif