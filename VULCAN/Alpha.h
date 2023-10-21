#ifndef ALPHA_H_
#define ALPHA_H_

#include <iostream>
#include "Invaders.h"

class Alpha: public Invaders 
{
    virtual void initialize(float offset);
    public:
    virtual ~Alpha()
    {
        Enemy::deleteBombs();
    }
};

#endif