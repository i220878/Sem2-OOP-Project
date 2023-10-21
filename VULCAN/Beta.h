#ifndef BETA_H_
#define BETA_H_

#include "Invaders.h"
#include <iostream>

class Beta: public Invaders 
{
    virtual void initialize(float offset);
    public:
    virtual ~Beta() 
    {
        Enemy::deleteBombs();
    }
};

#endif