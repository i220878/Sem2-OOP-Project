#include "Spaceship.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <math.h>
#include <iostream>
#include <string>

Spaceship::Spaceship()
{
	lives = 3;
	posX = 1280 / 2.0;
	posY = (1280 / 5.0) * 4;
	angle = 0;

	empowered = false;
	onFire = false;
	speed = 1;
	godMode = false;
	// After a lot of experimentation:
	// speed == 4 will be reserved for controlScheme == 2, which is Asteroids-like movement with Momentum,
	// while speed == 1, speed == 2, and speed == 3 will be reserved for controlScheme == 1 and controlScheme == 3.
	damage = 1;

	fireRate = (800 - (200 * (speed - 1))); // 800ms for regular speed.
	// Gets faster by 200ms for every pickup, upto a bullet every 200ms.

	t.loadFromFile("img/Ship360.png");
	p.setTexture(t);
	p.setTextureRect(sf::IntRect(0, 0, 72, 72));

	p.setPosition(posX, posY);

	p.setOrigin(36, 36);

	bullets = new Bullet[200];
	bullets[0] = Bullet(angle, speed, damage, 10000, 10000);
	size = 1;

	momentumDX = 0;
	momentumDY = 0;
	
	trail = new sf::Sprite[4];
	tex = new sf::Texture[4];
	for(int i = 0; i < 4; i++)
	{
		std::string path = "img/trails/trail";
		path += std::to_string(i + 1);
		path += "_360.png";
		tex[i].loadFromFile(path);
		trail[i].setTexture(tex[i]);
		trail[i].setOrigin(32, 32);
		trail[i].setTextureRect(sf::IntRect(64*(int(angle) % 19), 64*(int(angle) / 19), 64, 64));
	}
	t1.loadFromFile("img/trails/trail1G_360.png");
	t2.loadFromFile("img/trails/trail1R_360.png");
	t3.loadFromFile("img/trails/trail1LB_360.png");
	s1.setTexture(t1);
	s2.setTexture(t2);
	s3.setTexture(t3);
	s1.setTextureRect(sf::IntRect(0, 0, 64, 64));
	s2.setTextureRect(sf::IntRect(0, 0, 64, 64));
	s3.setTextureRect(sf::IntRect(0, 0, 64, 64));
	s1.setOrigin(32, 32);
	s2.setOrigin(32, 32);
	s3.setOrigin(32, 32);
}
void Spaceship::move(sf::RenderWindow& window, std::string d, int controlScheme, sf::Time& dT, int levelNumber)
{
	// std::cout << "dispV: " << dispV << ", dispH: " << dispH << ", angle: " << angle << std::endl;
	
	p.setTextureRect(sf::IntRect(72*(int(angle) % 19), 72*(int(angle) / 19), 72, 72));

	if(d == "0") // This is just for updating the texture and not actually performing movement
		return;

	if(d == "b" || d == "hb") // For blink and Heroic Blink
	{
		float dispV = cos(angle * (3.1416/180)) * 1;
		dispV *= -1;
		float dispH = sin(angle * (3.1416/180)) * 1;
		momentumDX = dispH;
		momentumDY = dispV;
		if(d == "hb")
		{
			momentumDX *= 3;
			momentumDY *= 3;
			s1.setTextureRect(sf::IntRect(64*(int(angle) % 19), 64*(int(angle) / 19), 64, 64));
			s2.setTextureRect(sf::IntRect(64*(int(angle) % 19), 64*(int(angle) / 19), 64, 64));
			s3.setTextureRect(sf::IntRect(64*(int(angle) % 19), 64*(int(angle) / 19), 64, 64));
			s1.setPosition(posX - 3.4*(momentumDX * dT.asSeconds() * 20000), posY - 3.4*(momentumDY * dT.asSeconds() * 20000));
			s2.setPosition(posX - 3.8*(momentumDX * dT.asSeconds() * 20000), posY - 3.8*(momentumDY * dT.asSeconds() * 20000));
			s3.setPosition(posX - 4.2*(momentumDX * dT.asSeconds() * 20000), posY - 4.2*(momentumDY * dT.asSeconds() * 20000));
		}
		for(int i = 0; i < 4; i++)
		{
			trail[i].setTextureRect(sf::IntRect(64*(int(angle) % 19), 64*(int(angle) / 19), 64, 64));
			trail[i].setPosition(posX - (i+1)*(momentumDX * dT.asSeconds() * 20000), posY - (i+1)*(momentumDY * dT.asSeconds() * 20000));
		}
		// std::cout << "mdx: " << momentumDX << ", mdy: " << momentumDY << std::endl;
		p.setPosition(posX += (momentumDX * dT.asSeconds() * 2000), posY += (momentumDY * dT.asSeconds() * 2000));
	}
	else
	{
		if(controlScheme == 2)
		{
			float dispV = cos(angle * (3.1416/180)) * 0.0025;
			dispV *= -1;
			float dispH = sin(angle * (3.1416/180)) * 0.0025;

			if (d == "u")
			{
				if(pow((pow(momentumDX, 2) + pow(momentumDY, 2)), 0.5) < this->getSpeed())
				{
					momentumDX += dispH;
					momentumDY += dispV;
				}
			}
			else if (d == "d")
			{
				momentumDX *= 0.99;
				momentumDY *= 0.99;
			}
			else
			{
				momentumDX *= 0.999;
				momentumDY *= 0.999;
			}

			float drag = 0.000625; // This exact fraction gives the proper desired Velocity
			momentumDX -= (drag * momentumDX);
			momentumDY -= (drag * momentumDY);

			// std::cout << "Speed: " << pow((pow(momentumDX, 2) + pow(momentumDY, 2)), 0.5) << std::endl;

			posX += (momentumDX * dT.asSeconds() * 500);
			posY += (momentumDY * dT.asSeconds() * 500);

			// std::cout << dT.asSeconds() << std::endl;

			p.setPosition(posX, posY);
		}

		if(controlScheme == 3)
		{
			momentumDX = 0;
			momentumDY = 0;
			if (d == "u")
				p.setPosition(posX, posY -= (1 * dT.asSeconds() * 1000));
			else if (d == "d")
				p.setPosition(posX, posY += (1 * dT.asSeconds() * 1000));
			if (d == "l")
				p.setPosition(posX -= (1 * dT.asSeconds() * 1000), posY);
			else if (d == "r")
				p.setPosition(posX += (1 * dT.asSeconds() * 1000), posY);
			else if (d == "ur")
				p.setPosition(posX += (1 * dT.asSeconds() * 1000), posY -= (1 * dT.asSeconds() * 1000));
			else if (d == "dr")
				p.setPosition(posX += (1 * dT.asSeconds() * 1000), posY += (1 * dT.asSeconds() * 1000));
			else if (d == "dl")
				p.setPosition(posX -= (1 * dT.asSeconds() * 1000), posY += (1 * dT.asSeconds() * 1000));
			else if (d == "ul")
				p.setPosition(posX -= (1 * dT.asSeconds() * 1000), posY -= (1 * dT.asSeconds() * 1000));

			angle = 0;
			this->move(window, "0", 1, dT);
		}

		if(controlScheme == 1)
		{
			momentumDX = 0;
			momentumDY = 0;
			if (d == "u")
			{
				p.setPosition(posX, posY -= (1 * dT.asSeconds() * 1000));
				angle = 0;
			}
			else if (d == "d")
			{
				p.setPosition(posX, posY += (1 * dT.asSeconds() * 1000));
				angle = 180;
			}
			if (d == "l")
			{
				p.setPosition(posX -= (1 * dT.asSeconds() * 1000), posY);
				angle = 270;
			}
			else if (d == "r")
			{
				p.setPosition(posX += (1 * dT.asSeconds() * 1000), posY);
				angle = 90;
			}
			else if (d == "ur")
			{
				p.setPosition(posX += (1 * dT.asSeconds() * 1000), posY -= (1 * dT.asSeconds() * 1000));
				angle = 45;
			}
			else if (d == "dr")
			{
				p.setPosition(posX += (1 * dT.asSeconds() * 1000), posY += (1 * dT.asSeconds() * 1000));
				angle = 135;
			}
			else if (d == "dl")
			{
				p.setPosition(posX -= (1 * dT.asSeconds() * 1000), posY += (1 * dT.asSeconds() * 1000));
				angle = 225;
			}
			else if (d == "ul")
			{
				p.setPosition(posX -= (1 * dT.asSeconds() * 1000), posY -= (1 * dT.asSeconds() * 1000));
				angle = 315;
			}
			this->move(window, "0", 1, dT);
		}
	}
	if(levelNumber >= 4)
	{
		if(posX > 1240)
		{
			posX = 1240;
			p.setPosition(posX, posY);
		}
		else if(posX < 40)
		{
			posX = 40;
			p.setPosition(posX, posY);
		}
		if(posY < 40)
		{
			posY = 40;
			p.setPosition(posX, posY);
		}
		else if(posY > 1240)
		{
			posY = 1240;
			p.setPosition(posX, posY);
		}
		return;
	}

	if(d != "hb")
	{
		if(posX > 1280)
		{
			posX = 0;
			p.setPosition(posX, posY);
		}
		else if(posX < 0)
		{
			posX = 1280;
			p.setPosition(posX, posY);
		}
		if(posY > 1280)
		{
			posY = 0;
			p.setPosition(posX, posY);
		}
		else if(posY < 0)
		{
			posY = 1280;
			p.setPosition(posX, posY);
		}
	}
	else 
	{
		if(posX > 1280 || posX < 0)
		{
			if(angle != 0)
				angle = 360 - angle;
		}
		if(posY > 1280 || posY < 0)
		{
			if(angle <= 180)
				angle = 180 - angle;
			else
				angle = 360 - (angle - 180);
		}
	}
}
void Spaceship::fire()
{
	fireRate = (800 - (200 * (speed - 1)));

	int bulletsMax = 200;
	// This integer represents the value to check before clearing bullets not on screen.
	// It also has to be updated manually in the constructor.

	if(size < bulletsMax)
	{
		bullets[size] = Bullet(angle, speed, damage, posX, posY);
		size++;
		return;
	}
	Bullet* temp = new Bullet[bulletsMax];
	int c = 0;
	for(int i = 0; i < bulletsMax; i++)
	{
		if(bullets[i].isOutside())
			continue;
		temp[c++] = bullets[i];
	}
	temp[c] = Bullet(angle, speed, damage, posX, posY);
	delete[] bullets;
	bullets = temp;
	size = c + 1;
}
void Spaceship::empoweredFire()
{
	fireRate = (800 - (200 * (speed - 1)));

	int bulletsMax = 200;
	// This integer represents the value to check before clearing bullets not on screen.
	// It also has to be updated manually in the constructor.

	if(size + 6 < bulletsMax)
	{
		int angle2 = angle - 21; if(angle2 < 0) angle2 = angle + 360 - 21;
		bullets[size + 0] = Bullet(angle2, speed, damage, posX, posY);

		angle2 = angle - 14; if(angle2 < 0) angle2 = angle + 360 - 14;
		bullets[size + 1] = Bullet(angle2, speed, damage, posX, posY);
		
		angle2 = angle - 7; if(angle2 < 0) angle2 = angle + 360 - 7;
		bullets[size + 2] = Bullet(angle2, speed, damage, posX, posY);

		angle2 = angle; if(angle2 < 0) angle2 += 1;
		bullets[size + 3] = Bullet(angle2, speed, damage, posX, posY);
		
		angle2 = angle + 7; if(angle2 >= 360) angle2 = angle - 360 + 7;
		bullets[size + 4] = Bullet(angle2, speed, damage, posX, posY);

		angle2 = angle + 14; if(angle2 >= 360) angle2 = angle - 360 + 14;
		bullets[size + 5] = Bullet(angle2, speed, damage, posX, posY);
		
		angle2 = angle + 21; if(angle2 >= 360) angle2 = angle - 360 + 21;
		bullets[size + 6] = Bullet(angle2, speed, damage, posX, posY);

		size++;

		size += 6;
		return;
	}
	Bullet* temp = new Bullet[bulletsMax];
	int c = 0;
	int d = 0;
	if(size > bulletsMax)
		d = bulletsMax;
	else
	 	d = size;
	for(int i = 0; i < d; i++)
	{
		if(bullets[i].isOutside())
			continue;
		temp[c++] = bullets[i];
	}

	int angle2 = angle - 21; if(angle2 < 0) angle2 = angle + 360 - 21;
	bullets[c + 0] = Bullet(angle2, speed, damage, posX, posY);

	angle2 = angle - 14; if(angle2 < 0) angle2 = angle + 360 - 14;
	bullets[c + 1] = Bullet(angle2, speed, damage, posX, posY);
	
	angle2 = angle - 7; if(angle2 < 0) angle2 = angle + 360 - 7;
	bullets[c + 2] = Bullet(angle2, speed, damage, posX, posY);

	angle2 = angle; if(angle2 < 0) angle2 += 1;
	bullets[c + 3] = Bullet(angle2, speed, damage, posX, posY);
	
	angle2 = angle + 7; if(angle2 >= 360) angle2 = angle - 360 + 7;
	bullets[c + 4] = Bullet(angle2, speed, damage, posX, posY);

	angle2 = angle + 14; if(angle2 >= 360) angle2 = angle - 360 + 14;
	bullets[c + 5] = Bullet(angle2, speed, damage, posX, posY);
	
	angle2 = angle + 21; if(angle2 >= 360) angle2 = angle - 360 + 21;
	bullets[c + 6] = Bullet(angle2, speed, damage, posX, posY);

	delete[] bullets;
	bullets = temp;
	size = c + 1;
	size = size + 6;
}
void Spaceship::setState(Enemy*& enemyA, int A_Count, Enemy*& enemyB, int B_Count, Enemy*& enemyG, int G_Count,sf::RenderWindow& window, int lives, float posX, float posY, float angle, int speed, int damage, float momentumDX, float momentumDY)
{
	if(this->lives > lives) // Plays an animation and also clears all bombs on screen
	{
		sf::Clock deathLoop;
		sf::Texture d;
		sf::Sprite death;
		int frameNumber = 0;
		d.loadFromFile("spritesheet/death.png");
		death.setTexture(d);
		death.setPosition(this->posX, this->posY);
		deathLoop.restart();
		float totalTime = 0;
		for(int i = 0; i < A_Count; i++)
			for(int j = 0; j < enemyA[i].getSize(); j++)
				enemyA[i].yeetBomb(enemyA[i].getBombs()[j]);
		for(int i = 0; i < B_Count; i++)
			for(int j = 0; j < enemyB[i].getSize(); j++)
				enemyB[i].yeetBomb(enemyB[i].getBombs()[j]);
		for(int i = 0; i < G_Count; i++)
			for(int j = 0; j < enemyG[i].getSize(); j++)
				enemyG[i].yeetBomb(enemyG[i].getBombs()[j]);
		while(totalTime < 0.5) // 9 frames, 30 fps. 9/30 is 0.3
		{
			if(deathLoop.getElapsedTime().asSeconds() > 0.04)
			{
				if(frameNumber == 0)
				{
					death.setTextureRect(sf::IntRect(440, 656, 150, 150));
					death.setOrigin(death.getGlobalBounds().width / 2, death.getGlobalBounds().height / 2);
				}
				else if(frameNumber == 1)
				{
					death.setTextureRect(sf::IntRect(644, 464, 168, 168));
					death.setOrigin(death.getGlobalBounds().width / 2, death.getGlobalBounds().height / 2);
				}
				else if(frameNumber == 2)
				{
					death.setTextureRect(sf::IntRect(450, 460, 192, 192));
					death.setOrigin(death.getGlobalBounds().width / 2, death.getGlobalBounds().height / 2);
				}
				else if(frameNumber == 3)
				{
					death.setTextureRect(sf::IntRect(230, 464, 216, 216));
					death.setOrigin(death.getGlobalBounds().width / 2, death.getGlobalBounds().height / 2);
				}
				else if(frameNumber == 4)
				{
					death.setTextureRect(sf::IntRect(444, 234, 220, 220));
					death.setOrigin(death.getGlobalBounds().width / 2, death.getGlobalBounds().height / 2);
				}
				else if(frameNumber == 5)
				{
					death.setTextureRect(sf::IntRect(244, 0, 240, 240));
					death.setOrigin(death.getGlobalBounds().width / 2, death.getGlobalBounds().height / 2);
				}
				else if(frameNumber == 6)
				{
					death.setTextureRect(sf::IntRect(0, 0, 240, 240));
					death.setOrigin(death.getGlobalBounds().width / 2, death.getGlobalBounds().height / 2);
				}
				else if(frameNumber == 7)
				{
					death.setTextureRect(sf::IntRect(0, 480, 240, 240));
					death.setOrigin(death.getGlobalBounds().width / 2, death.getGlobalBounds().height / 2);
				}
				else if(frameNumber == 8)
				{
					death.setTextureRect(sf::IntRect(0, 240, 240, 240));
					death.setOrigin(death.getGlobalBounds().width / 2, death.getGlobalBounds().height / 2);
				}
				else if(frameNumber == 9)
				{
					death.setTextureRect(sf::IntRect(480, 0, 240, 240));
					death.setOrigin(death.getGlobalBounds().width / 2, death.getGlobalBounds().height / 2);
				}
				death.setPosition(this->posX, this->posY);
				death.setScale(1.25, 1.25);
				
				for(int i = 0; i < A_Count; i++)
					window.draw(enemyA[i].getP());
				for(int i = 0; i < B_Count; i++)
					window.draw(enemyB[i].getP());
				for(int i = 0; i < G_Count; i++)
					window.draw(enemyG[i].getP());
				window.draw(death);
				window.display();
				window.clear();
				death.setScale(1, 1);
				totalTime += deathLoop.getElapsedTime().asSeconds();
				deathLoop.restart();
				frameNumber++;
			}
		}
	}
	p.setPosition(this->posX = posX, this->posY = posY);
	this->lives = lives;
	this->angle = angle;
	this->speed = speed;
	this->damage = damage;
	this->momentumDX = momentumDX;
	this->momentumDY = momentumDY;
	onFire = false;
	empowered = false;

	delete[] bullets;
	bullets = new Bullet[200];
	bullets[0] = Bullet(angle, speed, damage, 10000, 10000);
	size = 1;
}
void Spaceship::setState(int lives, float posX, float posY, float angle, int speed, int damage, float momentumDX, float momentumDY)
{
	p.setPosition(this->posX = posX, this->posY = posY);
	this->lives = lives;
	this->angle = angle;
	this->speed = speed;
	this->damage = damage;
	this->momentumDX = momentumDX;
	this->momentumDY = momentumDY;
	onFire = false;
	empowered = false;

	delete[] bullets;
	bullets = new Bullet[200];
	bullets[0] = Bullet(angle, speed, damage, 10000, 10000);
	size = 1;
}
void Spaceship::yeetBullet(Bullet& b)
{
	b.setCoords(10000, 10000);
}
void Spaceship::blink(sf::RenderWindow& window, int& controlScheme, sf::Time& dT, sf::Clock& blinkTimer, float& empoweredTime, bool& blinkActive)
{
	for(int i = 0; i < 4; i++)
	{
		window.draw(trail[i]);
		if(empowered == true)
		{
			for(int j = 0; j < 4; j++)
			{
				window.draw(trail[j]);
			}
			window.draw(s1);
			window.draw(s2);
			window.draw(s3);
		}
	}
	if(empowered == true)
	{
		// Heroic Blink
		if(blinkTimer.getElapsedTime().asSeconds() < 4)
		{
			move(window, "hb", controlScheme, dT);
		}
		else 
		{
			blinkTimer.restart();
			blinkActive = false;
			empoweredTime = 0;
			empowered = false;
		}
	}

	if(!empowered)
	{
		// If not empowered, or not enough time remaining on empowerment,
		// just a regular blink.
		if(blinkTimer.getElapsedTime().asSeconds() < 0.2)
		{
			move(window, "b", controlScheme, dT);
		}
		else 
		{
			blinkTimer.restart();
			blinkActive = false;
		}
	}
}