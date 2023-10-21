// 640, 520 is a suitable center point for the shapes.
// The spawn area is 768x768 with that as the center.

// level 1: rectangle, triangle, crossing. enemy layouts, at boundary.
// level 2: circle, diamond, heart, at boundary.
// level 3: combined. total of 6 shapes. and they should be filled.

// 768 / 80 is 9.6

// Enemy's position is entered into the Level's constructor.
// Meaning enemy pointers have to be null before the level is made, and when it's made,
// all the enemies are initialized and spawned.

// Best approach is to first generate the coordinates, then add displacement for
// center afterwards. Most flexibility, and easiest to do.

#ifndef LEVELS_H_
#define LEVELS_H_

#include <SFML/Graphics.hpp>
#include "Alpha.h"
#include "Beta.h"
#include "Gamma.h"
#include "Monster.h"
#include "Dragon.h"
#include "Mothership.h"
#include <iostream>
#include "Spaceship.h"
#include <math.h>

class Levels {
public:
    static int phase;
    static int levelNumber;
    virtual void generateLevel(Enemy*&, int&, Enemy*&, int&, Enemy*&, int&, Spaceship&, int difficulty, Enemy*& boss) = 0;
    virtual void nextPhase(Enemy*&, int&, Enemy*&, int&, Enemy*&, int&, Spaceship&) = 0;
    virtual void levelEnd(bool& levelClear, bool& gameOver, Spaceship& player) = 0;
    virtual ~Levels() {}
};

class Level1: public Levels {
public:
    virtual void generateLevel(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, Spaceship& player, int difficulty, Enemy*& boss)
    {
        player.setState(3, 640, 1024, 0, 1, 1, 0, 0);
        levelNumber = 1;
        A_Count = 5;
        B_Count = 5;
        G_Count = 2;
        enemyA = new Alpha[A_Count];
        enemyB = new Beta[B_Count];
        enemyG = new Gamma[G_Count];
        for(int i = 0; i < A_Count; i++)
            enemyA[i].initialize(float(rand() % 5000));
        for(int i = 0; i < B_Count; i++)
            enemyB[i].initialize(float(rand() % 3000));
        for(int i = 0; i < G_Count; i++)
            enemyG[i].initialize(float(rand() % 2000));

        for(int i = 0; i < A_Count; i++)
        {
            enemyA[i].Enemy::setPosX(640 - 320 + 64 + 128 * i);
            enemyA[i].Enemy::setPosY(520 - 192 + 0);
        }
        for(int i = 0; i < B_Count; i++)
        {
            enemyB[i].Enemy::setPosX(640 - 320 + 64 + 128 * i);
            enemyB[i].Enemy::setPosY(520 - 192 + 256);
        }
        enemyG[0].Enemy::setPosX(640 - 320 + 64 + 0);
        enemyG[0].Enemy::setPosY(520 - 192 + 128);
        enemyG[1].Enemy::setPosX(640 - 320 + 64 + 512);
        enemyG[1].Enemy::setPosY(520 - 192 + 128);
    }
    virtual void nextPhase(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, Spaceship& player)
    {
        A_Count = 0;
        B_Count = 0;
        G_Count = 0;
        delete[] enemyA;
        delete[] enemyB;
        delete[] enemyG;
        enemyA = nullptr;
        enemyB = nullptr;
        enemyG = nullptr;
        std::cout << "Next phase....";
        phase++;
        std::cout << "Phase " << phase + 1 << std::endl;
        if(phase == 1) // Second phase. Hollow Triangle.
        {
            int thing = 5; // Enemy spawn is triple this amount.
            A_Count = thing;
            B_Count = thing;
            G_Count = thing;
            enemyA = new Alpha[A_Count];
            enemyB = new Beta[B_Count];
            enemyG = new Gamma[G_Count];
            for(int i = 0; i < A_Count; i++)
                enemyA[i].initialize(float(rand() % 5000));
            for(int i = 0; i < B_Count; i++)
                enemyB[i].initialize(float(rand() % 3000));
            for(int i = 0; i < G_Count; i++)
                enemyG[i].initialize(float(rand() % 2000));

            int a = 0, b = 0, g = 0;
            for(int i = 0; i < thing; i++)
            {
                int pX = 0;
                int pY = 80 * i;
                for(int j = i; j < thing; j++)
                {
                    pX += 80;
                }
                enemyA[a].Enemy::setPosX(240 + pX);
                enemyA[a++].Enemy::setPosY(160 + pY);
                pX += 80;
                for(int j = 0; j < i; j++)
                {
                    pX += 80;
                    if(j == i - 1)
                    {
                        enemyB[b].Enemy::setPosX(240 + pX);
                        enemyB[b++].Enemy::setPosY(160 + pY);
                    }
                    else
                        pX += 80;
                }
            }
            for(int i = 0; i <= thing; i++)
            {
                int pX = 160 * i;
                int pY = 80 * thing;
                if(i == thing)
                {
                    enemyB[b].Enemy::setPosX(240 + pX);
                    enemyB[b++].Enemy::setPosY(160 + pY);
                    continue;
                }
                enemyG[g].Enemy::setPosX(240 + pX);
                enemyG[g++].Enemy::setPosY(160 + pY);
            }
        }
        else if(phase == 2) // Third phase. Cross.
        {
            int thing = 6;
            A_Count = 6;
            B_Count = 3;
            G_Count = 3;
            enemyA = new Alpha[A_Count];
            enemyB = new Beta[B_Count];
            enemyG = new Gamma[G_Count];
            for(int i = 0; i < A_Count; i++)
                enemyA[i].initialize(float(rand() % 5000));
            for(int i = 0; i < B_Count; i++)
                enemyB[i].initialize(float(rand() % 3000));
            for(int i = 0; i < G_Count; i++)
                enemyG[i].initialize(float(rand() % 2000));

            int a = 0, b = 0, g = 0;
            int pX = 0, pY = 0;
            for(int i = 0; i < thing; i++)
            {
                pY = 80 * i;
                pX = 80 * i;
                enemyA[a].Enemy::setPosX(640 - 200 + pX);
                enemyA[a++].Enemy::setPosY(160 + pY);
            }
            pX = 80 * thing;
            for(int i = 0; i < 3; i++)
            {
                pX -= 80;
                pY = 80 * i;
                enemyB[b].Enemy::setPosX(440 + pX);
                enemyB[b++].Enemy::setPosY(160 + pY);
	        }
            for(int i = 3; i < 6; i++)
            {
                pX -= 80;
                pY = 80 * i;
                enemyG[g].Enemy::setPosX(440 + pX);
                enemyG[g++].Enemy::setPosY(160 + pY);
            }
        }
    }
    virtual void levelEnd(bool& levelClear, bool& gameOver, Spaceship& player)
    {
        if(phase >= 3 && player.getLives() > 0)
        {
            gameOver = false;
            levelClear = true;
            phase = 0;
        }
        else if(phase <= 2 && player.getLives() <= 0)
        {
            levelClear = false;
            gameOver = true;
            phase = 0;
        }
    }
};
class Level2: public Levels {
public:
    virtual void generateLevel(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, Spaceship& player, int difficulty, Enemy*& boss)
    {
        player.setState(5 - difficulty, 640, 1024, 0, 4 - difficulty, 4 - difficulty, 0, 0);
        boss = new Monster;
        boss->Enemy::setPosX(640);
        boss->Enemy::setPosY(-200); // 200 is to be reached via animation
        boss->initialize(10);
        levelNumber = 2;
        int thing = 24;
        A_Count = thing / 3;
        B_Count = thing / 3;
        G_Count = thing / 3;
        enemyA = new Alpha[A_Count];
        enemyB = new Beta[B_Count];
        enemyG = new Gamma[G_Count];
        for(int i = 0; i < A_Count; i++)
            enemyA[i].initialize(float(rand() % 5000));
        for(int i = 0; i < B_Count; i++)
            enemyB[i].initialize(float(rand() % 3000));
        for(int i = 0; i < G_Count; i++)
            enemyG[i].initialize(float(rand() % 2000));
        float angle = 0;
        float dispH = 0;
        float dispV = 0;
        float radius = 300;
        int a = 0, b = 0, g = 0;
        for(int i = 0; i < thing; i++)
        {
            angle += 15;
            dispV = cos(angle * (3.1416/180)) * radius;
            dispV *= -1;
            dispH = sin(angle * (3.1416/180)) * radius;
            if(i < 8)
            {
                enemyA[a].Enemy::setPosX(640 + dispH);
                enemyA[a++].Enemy::setPosY(440 + dispV);
            }
            else if(i >= 8 && i < 16)
            {
                enemyB[b].Enemy::setPosX(640 + dispH);
                enemyB[b++].Enemy::setPosY(440 + dispV);
            }
            else if(i >= 16 && i < 24)
            {
                enemyG[g].Enemy::setPosX(640 + dispH);
                enemyG[g++].Enemy::setPosY(440 + dispV);
            }
        }
    }
    virtual void nextPhase(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, Spaceship& player) 
    {
        A_Count = 0;
        B_Count = 0;
        G_Count = 0;
        delete[] enemyA;
        delete[] enemyB;
        delete[] enemyG;
        enemyA = nullptr;
        enemyB = nullptr;
        enemyG = nullptr;
        std::cout << "Next phase....";
        phase++;
        std::cout << "Phase " << phase + 1 << std::endl;
        if(phase == 1) // Phase 2
        {
            A_Count = 8;
            B_Count = 8;
            G_Count = 4;
            enemyA = new Alpha[A_Count];
            enemyB = new Beta[B_Count];
            enemyG = new Gamma[G_Count];
            for(int i = 0; i < A_Count; i++)
                enemyA[i].initialize(float(rand() % 5000));
            for(int i = 0; i < B_Count; i++)
                enemyB[i].initialize(float(rand() % 3000));
            for(int i = 0; i < G_Count; i++)
                enemyG[i].initialize(float(rand() % 2000));

            int thing = 6;
            thing--;
            float posX = 0;
            float posY = 0;
            int a = 0, b = 0, g = 0;        
            for(int i = 0; i < thing; i++)
            {
                for(int j = i; j < thing; j++)
                {
                    posX += 40;
                }
                if(i == 0)
                {
                    enemyG[g].Enemy::setPosX(posX + 440);
                    enemyG[g++].Enemy::setPosY(posY + 40);
                }
                else 
                {
                    enemyA[a].Enemy::setPosX(posX + 440);
                    enemyA[a++].Enemy::setPosY(posY + 40);
                }
                posX += 40;
                for(int j = 0; j < i; j++)
                {
                    if(j == i - 1)
                    {
                        posX += 40;
                        enemyB[b].Enemy::setPosX(posX + 440);
                        enemyB[b++].Enemy::setPosY(posY + 40);
                    }
                    else
                        posX += 80;
                }
                posY += 80;
                posX = 0;
            }
            for(int i = 0; i < thing; i++)
            {
                for(int j = 0; j < i; j++)
                    posX += 40;
                if(i == 0)
                {
                    enemyG[g].Enemy::setPosX(posX + 440);
                    enemyG[g++].Enemy::setPosY(posY + 40);
                }
                else 
                {
                    enemyB[b].Enemy::setPosX(posX + 440);
                    enemyB[b++].Enemy::setPosY(posY + 40);
                }
                posX += 40;
                for(int j = i; j < thing; j++)
                {
                    if(j == thing - 1)
                    {
                        posX += 40;
                        if(i == 0)
                        {
                            enemyG[g].Enemy::setPosX(posX + 440);
                            enemyG[g++].Enemy::setPosY(posY + 40);
                        }
                        else 
                        {
                            enemyA[a].Enemy::setPosX(posX + 440);
                            enemyA[a++].Enemy::setPosY(posY + 40);
                        }
                    }
                    else
                        posX += 80;
                }
                posY += 80;
                posX = 0;
            }
            for(int i = 0; i < thing; i++)
                posX += 40;
            enemyG[g].Enemy::setPosX(posX + 440);
            enemyG[g++].Enemy::setPosY(posY + 40);
        }
        else if(phase == 2) // Phase 3
        {
            A_Count = 9;
            B_Count = 9;
            G_Count = 15;
            enemyA = new Alpha[A_Count];
            enemyB = new Beta[B_Count];
            enemyG = new Gamma[G_Count];
            for(int i = 0; i < A_Count; i++)
                enemyA[i].initialize(float(rand() % 5000));
            for(int i = 0; i < B_Count; i++)
                enemyB[i].initialize(float(rand() % 3000));
            for(int i = 0; i < G_Count; i++)
                enemyG[i].initialize(float(rand() % 2000));
            float angle = 0;
            float dispH = 0;
            float dispV = 0;
            float radius = 200;
            int a = 0, b = 0, g = 0;
            int centerDisplaceX = 440, centerDisplaceY = 240;
            angle = 270;
            for(int i = 0; i < A_Count; i++)
            {
                dispV = cos(angle * (3.1416/180)) * radius;
                dispV *= -1;
                dispH = sin(angle * (3.1416/180)) * radius;
                enemyA[i].Enemy::setPosX(centerDisplaceX + dispH);
                enemyA[i].Enemy::setPosY(centerDisplaceY + dispV);
                angle += 20;
            }
            angle = 290;
            centerDisplaceX = 840;
            for(int i = 0; i < B_Count; i++)
            {
                dispV = cos(angle * (3.1416/180)) * radius;
                dispV *= -1;
                dispH = sin(angle * (3.1416/180)) * radius;
                enemyB[i].Enemy::setPosX(centerDisplaceX + dispH);
                enemyB[i].Enemy::setPosY(centerDisplaceY + dispV);
                angle += 20;
            }
            for(int i = 0; i < G_Count; i++)
            {
                float temp1X = enemyA[0].getPosX();
                float temp1Y = enemyA[0].getPosY();
                float temp2X = enemyB[8].getPosX();
                float temp2Y = enemyB[8].getPosY();
                enemyG[i].Enemy::setPosX((i+1)*((temp2X - temp1X)/14) + 182);
                if(i <= 7)
                    enemyG[i].Enemy::setPosY((i+1)*(80) + centerDisplaceY);
                if(i > 7)
                    enemyG[i].Enemy::setPosY((G_Count - i)*(80) + centerDisplaceY);
            }
        }
    }
    virtual void levelEnd(bool& levelClear, bool& gameOver, Spaceship& player)
    {
        if(phase >= 3 && player.getLives() > 0)
        {
            gameOver = false;
            levelClear = true;
            phase = 0;
       }
        else if(phase <= 2 && player.getLives() <= 0)
        {
            levelClear = false;
            gameOver = true;
            phase = 0;
        }
    }
};
class Level3: public Levels {
public:
    // level 1: rectangle, triangle, crossing. enemy layouts, at boundary.
    // level 2: circle, diamond, heart, at boundary.
    // level 3: combined. total of 6 shapes. and they should be filled.
    virtual void generateLevel(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, Spaceship& player, int difficulty, Enemy*& boss)
    {
        player.setState(5 - difficulty, 640, 1024, 0, 4 - difficulty, 4 - difficulty, 0, 0);
        levelNumber = 3;
        boss = new Dragon;
        boss->Enemy::setPosX(640);
        boss->Enemy::setPosY(-240); // 240 is to be reached via animation
        boss->initialize(10);
        int thing = 24;
        A_Count = thing / 3;
        B_Count = thing / 3;
        G_Count = thing / 3;
        enemyA = new Alpha[A_Count];
        enemyB = new Beta[B_Count];
        enemyG = new Gamma[G_Count];
        for(int i = 0; i < A_Count; i++)
            enemyA[i].initialize(float(rand() % 5000));
        for(int i = 0; i < B_Count; i++)
            enemyB[i].initialize(float(rand() % 3000));
        for(int i = 0; i < G_Count; i++)
            enemyG[i].initialize(float(rand() % 2000));
        // So filled rectangle first.
        float cenDisX = 200, cenDisY = 200;
        for(int i = 0; i < A_Count; i++)
        {
            enemyA[i].Enemy::setPosX(128 * i + cenDisX);
            enemyB[i].Enemy::setPosX(128 * i + cenDisX);
            enemyG[i].Enemy::setPosX(128 * i + cenDisX);
            enemyA[i].Enemy::setPosY(0 + cenDisY);
            enemyB[i].Enemy::setPosY(128 + cenDisY);
            enemyG[i].Enemy::setPosY(256 + cenDisY);
        }
    }
    virtual void nextPhase(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, Spaceship& player)
    {
        A_Count = 0;
        B_Count = 0;
        G_Count = 0;
        delete[] enemyA;
        delete[] enemyB;
        delete[] enemyG;
        enemyA = nullptr;
        enemyB = nullptr;
        enemyG = nullptr;
        std::cout << "Next phase....";
        phase++;
        std::cout << "Phase " << phase + 1 << std::endl;
        if(phase == 1) // Filled Triangle
        {
            int thing = 6;
            A_Count = 21;
            B_Count = 15;
            G_Count = 13;
            enemyA = new Alpha[A_Count];
            enemyB = new Beta[B_Count];
            enemyG = new Gamma[G_Count];
            for(int i = 0; i < A_Count; i++)
                enemyA[i].initialize(float(rand() % 5000));
            for(int i = 0; i < B_Count; i++)
                enemyB[i].initialize(float(rand() % 3000));
            for(int i = 0; i < G_Count; i++)
                enemyG[i].initialize(float(rand() % 2000));

            int a = 0, b = 0, g = 0;
            float cenDisX = 160, cenDisY = 160;
            for(int i = 0; i < thing; i++)
            {
                int pX = 0;
                int pY = 80 * i;
                for(int j = i; j < thing; j++)
                {
                    pX += 80;
                }
                enemyA[a].Enemy::setPosX(cenDisX + pX);
                enemyA[a++].Enemy::setPosY(cenDisY + pY);
                pX += 80;
                for(int j = 0; j < i; j++)
                {
                    enemyB[b].Enemy::setPosX(cenDisX + pX);
                    enemyB[b++].Enemy::setPosY(cenDisY + pY);
                    pX += 80;
                    enemyA[a].Enemy::setPosX(cenDisX + pX);
                    enemyA[a++].Enemy::setPosY(cenDisY + pY);
                    pX += 80;
                }
            }
            for(int i = 0; i < G_Count; i++)
            {
                int pX = 80 * i;
                int pY = 80 * thing;
                enemyG[g].Enemy::setPosX(cenDisX + pX);
                enemyG[g++].Enemy::setPosY(cenDisY + pY);
            }
        }
        else if(phase == 2) // Filled X
        {
            // AAA   AAA       
            //  AAA AAA          
            //   BBBBB            
            //    GGG           
            //   BBBBB        
            //  AAA AAA          
            // AAA   AAA
            int thing = 6;
            A_Count = 24;
            B_Count = 10;
            G_Count = 3;
            enemyA = new Alpha[A_Count];
            enemyB = new Beta[B_Count];
            enemyG = new Gamma[G_Count];
            for(int i = 0; i < A_Count; i++)
                enemyA[i].initialize(float(rand() % 5000));
            for(int i = 0; i < B_Count; i++)
                enemyB[i].initialize(float(rand() % 3000));
            for(int i = 0; i < G_Count; i++)
                enemyG[i].initialize(float(rand() % 2000));

            int a = 0, b = 0, g = 0;
            float posX = 0, posY = 0;
            float cenDisX = 320, cenDisY = 200;
            for(int i = 0; i < 9; i++)
            {
                if(!(i == 3 || i == 4 || i == 5))
                {
                    enemyA[a].Enemy::setPosX(cenDisX + posX);
                    enemyA[a++].Enemy::setPosY(cenDisY + posY);
                }
                posX += 80;
            }
            posX = 80; posY = 80;
            for(int i = 0; i < 7; i++)
            {
                if(!(i == 3))
                {
                    enemyA[a].Enemy::setPosX(cenDisX + posX);
                    enemyA[a++].Enemy::setPosY(cenDisY + posY);
                }
                posX += 80;
            }
            posX = 160; posY = 160;
            for(int i = 0; i < 5; i++)
            {
                enemyB[b].Enemy::setPosX(cenDisX + posX);
                enemyB[b++].Enemy::setPosY(cenDisY + posY);
                posX += 80;
            }
            posX = 240; posY = 240;
            for(int i = 0; i < 3; i++)
            {
                enemyG[g].Enemy::setPosX(cenDisX + posX);
                enemyG[g++].Enemy::setPosY(cenDisY + posY);
                posX += 80;
            }
            posX = 160; posY = 320;
            for(int i = 0; i < 5; i++)
            {
                enemyB[b].Enemy::setPosX(cenDisX + posX);
                enemyB[b++].Enemy::setPosY(cenDisY + posY);
                posX += 80;
            }
            posX = 80; posY = 400;
            for(int i = 0; i < 7; i++)
            {
                if(!(i == 3))
                {
                    enemyA[a].Enemy::setPosX(cenDisX + posX);
                    enemyA[a++].Enemy::setPosY(cenDisY + posY);
                }
                posX += 80;
            }
            posX = 0; posY = 480;
            for(int i = 0; i < 9; i++)
            {
                if(!(i == 3 || i == 4 || i == 5))
                {
                    enemyA[a].Enemy::setPosX(cenDisX + posX);
                    enemyA[a++].Enemy::setPosY(cenDisY + posY);
                }
                posX += 80;
            }
        }
        else if(phase == 3) // Filled Circle
        {
            A_Count = 25;
            B_Count = 16;
            G_Count = 8;
            enemyA = new Alpha[A_Count];
            enemyB = new Beta[B_Count];
            enemyG = new Gamma[G_Count];
            for(int i = 0; i < A_Count; i++)
                enemyA[i].initialize(float(rand() % 5000));
            for(int i = 0; i < B_Count; i++)
                enemyB[i].initialize(float(rand() % 3000));
            for(int i = 0; i < G_Count; i++)
                enemyG[i].initialize(float(rand() % 2000));
            float angle = 0;
            float dispH = 0;
            float dispV = 0;
            int a = 0, b = 0, g = 0;
            enemyA[a].Enemy::setPosX(640);
            enemyA[a++].Enemy::setPosY(440);
            for(int j = 1; j <= 3; j++)
            {
                float radius = 70 * j;
                for(int i = 0; i < 8 * j; i++)
                {
                    angle += 15 * (4 - j);
                    dispV = cos(angle * (3.1416/180)) * radius;
                    dispV *= -1;
                    dispH = sin(angle * (3.1416/180)) * radius;
                    if(i < 8)
                    {
                        enemyA[a].Enemy::setPosX(640 + dispH);
                        enemyA[a++].Enemy::setPosY(440 + dispV);
                    }
                    else if(i >= 8 && i < 16)
                    {
                        enemyB[b].Enemy::setPosX(640 + dispH);
                        enemyB[b++].Enemy::setPosY(440 + dispV);
                    }
                    else if(i >= 16 && i < 24)
                    {
                        enemyG[g].Enemy::setPosX(640 + dispH);
                        enemyG[g++].Enemy::setPosY(440 + dispV);
                    }
                    else if(i >= 24 && i < 32)
                    {
                        enemyA[a].Enemy::setPosX(640 + dispH);
                        enemyA[a++].Enemy::setPosY(440 + dispV);
                    }
                }
            }
            std::cout << a << " " << b << " " << g << std::endl;
        }
        else if(phase == 4) // Filled Diamond
        {
            A_Count = 14;
            B_Count = 14;
            G_Count = 8;
            enemyA = new Alpha[A_Count];
            enemyB = new Beta[B_Count];
            enemyG = new Gamma[G_Count];
            for(int i = 0; i < A_Count; i++)
                enemyA[i].initialize(float(rand() % 5000));
            for(int i = 0; i < B_Count; i++)
                enemyB[i].initialize(float(rand() % 3000));
            for(int i = 0; i < G_Count; i++)
                enemyG[i].initialize(float(rand() % 2000));

            int thing = 6;
            thing--;
            float posX = 0;
            float posY = 0;
            int a = 0, b = 0, g = 0;        
            for(int i = 0; i < thing; i++)
            {
                for(int j = i; j < thing; j++)
                {
                    posX += 40;
                }
                if(i == 0)
                {
                    enemyG[g].Enemy::setPosX(posX + 440);
                    enemyG[g++].Enemy::setPosY(posY + 40);
                }
                else 
                {
                    enemyA[a].Enemy::setPosX(posX + 440);
                    enemyA[a++].Enemy::setPosY(posY + 40);
                }
                posX += 40;
                for(int j = 0; j < i; j++)
                {
                    posX += 40;
                    enemyB[b].Enemy::setPosX(posX + 440);
                    enemyB[b++].Enemy::setPosY(posY + 40);
                    posX += 40;
                }
                posY += 80;
                posX = 0;
            }
            for(int i = 0; i < thing; i++)
            {
                for(int j = 0; j < i; j++)
                    posX += 40;
                if(i == 0)
                {
                    enemyG[g].Enemy::setPosX(posX + 440);
                    enemyG[g++].Enemy::setPosY(posY + 40);
                }
                else 
                {
                    enemyB[b].Enemy::setPosX(posX + 440);
                    enemyB[b++].Enemy::setPosY(posY + 40);
                }
                posX += 40;
                for(int j = i; j < thing; j++)
                {
                    posX += 40;
                    if(i == 0)
                    {
                        enemyG[g].Enemy::setPosX(posX + 440);
                        enemyG[g++].Enemy::setPosY(posY + 40);
                    }
                    else 
                    {
                        enemyA[a].Enemy::setPosX(posX + 440);
                        enemyA[a++].Enemy::setPosY(posY + 40);
                    }
                    posX += 40;
                }
                posY += 80;
                posX = 0;
            }
            for(int i = 0; i < thing; i++)
                posX += 40;
            enemyG[g].Enemy::setPosX(posX + 440);
            enemyG[g++].Enemy::setPosY(posY + 40);
        }
        else if(phase == 5) // Filled Heart
        {
            A_Count = 12;
            B_Count = 12;
            G_Count = 13;
            enemyA = new Alpha[A_Count];
            enemyB = new Beta[B_Count];
            enemyG = new Gamma[G_Count];
            for(int i = 0; i < A_Count; i++)
                enemyA[i].initialize(float(rand() % 5000));
            for(int i = 0; i < B_Count; i++)
                enemyB[i].initialize(float(rand() % 3000));
            for(int i = 0; i < G_Count; i++)
                enemyG[i].initialize(float(rand() % 2000));
            //    BB   BB            
            //   GGGG GGGG           
            //   AAAAAAAAA       
            //    BBBBBBB          
            //     GGGGG           
            //      AAA            
            //       B               
            // It's gonna look way more square when actually drawn.
            float posX = 0, posY = 0;
            float cenDispX = 280, cenDispY = 200;
            int a = 0, b = 0, g = 0;
            posX = 80; posY = 0;
            for(int j = 0; j < 2; j++)
            {
                enemyB[b].Enemy::setPosX(posX + cenDispX);
                enemyB[b++].Enemy::setPosY(posY + cenDispY);
                posX += 80;
            }
            posX = 480; posY = 0;
            for(int j = 0; j < 2; j++)
            {
                enemyB[b].Enemy::setPosX(posX + cenDispX);
                enemyB[b++].Enemy::setPosY(posY + cenDispY);
                posX += 80;
            }
            posX = 0; posY = 80;
            for(int j = 0; j < 4; j++)
            {
                enemyG[g].Enemy::setPosX(posX + cenDispX);
                enemyG[g++].Enemy::setPosY(posY + cenDispY);
                posX += 80;
            }
            posX = 400; posY = 80;
            for(int j = 0; j < 4; j++)
            {
                enemyG[g].Enemy::setPosX(posX + cenDispX);
                enemyG[g++].Enemy::setPosY(posY + cenDispY);
                posX += 80;
            }
            posX = 0; posY = 160;
            for(int j = 0; j < 9; j++)
            {
                enemyA[a].Enemy::setPosX(posX + cenDispX);
                enemyA[a++].Enemy::setPosY(posY + cenDispY);
                posX += 80;
            }
            posX = 80; posY += 80;
            for(int j = 0; j < 7; j++)
            {
                enemyB[b].Enemy::setPosX(posX + cenDispX);
                enemyB[b++].Enemy::setPosY(posY + cenDispY);
                posX += 80;
            }
            posX = 160; posY += 80;
            for(int j = 0; j < 5; j++)
            {
                enemyG[g].Enemy::setPosX(posX + cenDispX);
                enemyG[g++].Enemy::setPosY(posY + cenDispY);
                posX += 80;
            }
            posX = 240; posY += 80;
            for(int j = 0; j < 3; j++)
            {
                enemyA[a].Enemy::setPosX(posX + cenDispX);
                enemyA[a++].Enemy::setPosY(posY + cenDispY);
                posX += 80;
            }
            posY += 80;
            enemyB[b].Enemy::setPosX(320 + cenDispX);
            enemyB[b++].Enemy::setPosY(posY + cenDispY);
        }
    }
    virtual void levelEnd(bool& levelClear, bool& gameOver, Spaceship& player)
    {
        if(phase >= 6 && player.getLives() > 0)
        {
            gameOver = false;
            levelClear = true;
            phase = 0;
       }
        else if(phase <= 5 && player.getLives() <= 0)
        {
            levelClear = false;
            gameOver = true;
            phase = 0;
        }
    }
};

class Boss1: public Levels {
public:
    virtual void generateLevel(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, Spaceship& player, int difficulty, Enemy*& boss)
    {
        levelNumber = 4;
        player.setState(3, 640, 1024, 0, 4, 1, 0, 0);
        boss = new Monster;
        boss->Enemy::setPosX(640);
        boss->Enemy::setPosY(200);
        boss->initialize(10);
        A_Count = 1;
        B_Count = 1;
        G_Count = 1;
        enemyA = new Alpha[A_Count];
        enemyB = new Beta[B_Count];
        enemyG = new Gamma[G_Count];
        for(int i = 0; i < A_Count; i++)
            enemyA[i].initialize(float(rand() % 5000));
        for(int i = 0; i < B_Count; i++)
            enemyB[i].initialize(float(rand() % 3000));
        for(int i = 0; i < G_Count; i++)
            enemyG[i].initialize(float(rand() % 2000));
        enemyA[0].Enemy::setPosX(5000);
        enemyB[0].Enemy::setPosX(5000);
        enemyG[0].Enemy::setPosX(5000);
    }
    virtual void nextPhase(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, Spaceship& player)
    {

    }
    virtual void levelEnd(bool& levelClear, bool& gameOver, Spaceship& player)
    {

    }
};
class Boss2: public Levels {
public:
    virtual void generateLevel(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, Spaceship& player, int difficulty, Enemy*& boss)
    {
        levelNumber = 5;
        player.setState(3, 640, 1024, 0, 4, 1, 0, 0);
        boss = new Dragon;
        boss->Enemy::setPosX(640);
        boss->Enemy::setPosY(240);
        boss->initialize(10);
        A_Count = 1;
        B_Count = 1;
        G_Count = 1;
        enemyA = new Alpha[A_Count];
        enemyB = new Beta[B_Count];
        enemyG = new Gamma[G_Count];
        for(int i = 0; i < A_Count; i++)
            enemyA[i].initialize(float(rand() % 5000));
        for(int i = 0; i < B_Count; i++)
            enemyB[i].initialize(float(rand() % 3000));
        for(int i = 0; i < G_Count; i++)
            enemyG[i].initialize(float(rand() % 2000));
        enemyA[0].Enemy::setPosX(5000);
        enemyB[0].Enemy::setPosX(5000);
        enemyG[0].Enemy::setPosX(5000);
    }
    virtual void nextPhase(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, Spaceship& player)
    {

    }
    virtual void levelEnd(bool& levelClear, bool& gameOver, Spaceship& player)
    {

    }
};
class Boss3: public Levels {
public:
    virtual void generateLevel(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, Spaceship& player, int difficulty, Enemy*& boss)
    {
        levelNumber = 6;
        player.setState(3, 640, 1024, 0, 4, 1, 0, 0);
        boss = new Mothership;
        boss->Enemy::setPosX(640);
        boss->Enemy::setPosY(240);
        boss->initialize(10);
        A_Count = 1;
        B_Count = 1;
        G_Count = 1;
        enemyA = new Alpha[A_Count];
        enemyB = new Beta[B_Count];
        enemyG = new Gamma[G_Count];
        for(int i = 0; i < A_Count; i++)
            enemyA[i].initialize(float(rand() % 5000));
        for(int i = 0; i < B_Count; i++)
            enemyB[i].initialize(float(rand() % 3000));
        for(int i = 0; i < G_Count; i++)
            enemyG[i].initialize(float(rand() % 2000));
        enemyA[0].Enemy::setPosX(5000);
        enemyB[0].Enemy::setPosX(5000);
        enemyG[0].Enemy::setPosX(5000);
    }
    virtual void nextPhase(Enemy*& enemyA, int& A_Count, Enemy*& enemyB, int& B_Count, Enemy*& enemyG, int& G_Count, Spaceship& player)
    {

    }
    virtual void levelEnd(bool& levelClear, bool& gameOver, Spaceship& player)
    {

    }
};
#endif
