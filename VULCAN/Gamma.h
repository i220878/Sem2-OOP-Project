#ifndef GAMMA_H_
#define GAMMA_H_

#include "Invaders.h"
#include <iostream>

class Gamma: public Invaders 
{
    virtual void initialize(float offset);
    public:
    virtual ~Gamma() 
    {
        Enemy::deleteBombs();
    }
};

#endif