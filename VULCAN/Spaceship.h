
#ifndef SPACESHIP_H_
#define SPACESHIP_H_

#include "Bullet.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class Spaceship
{
private:
	int lives;
	float posX;
	float posY;

	Bullet* bullets;
	int size;

	bool empowered;
	bool godMode;
	
	bool onFire;
	int speed;
	int damage;

	sf::Texture t;
	sf::Sprite p;
	float angle; // 0 to 359

	float fireRate;

	float momentumDX;
	float momentumDY;

	sf::Sprite* trail;
	sf::Texture* tex;

public:
	sf::Texture t1;
	sf::Sprite s1;
	sf::Texture t2;
	sf::Sprite s2;
	sf::Texture t3;
	sf::Sprite s3;
	void setInvincible(bool g) {
		godMode = g;
	}
	bool isInvincible() const {
		return godMode;
	}
	sf::Sprite* getTrail() const {
		return trail;
	}
	bool isOnFire() const {
		return onFire;
	}
	void becomeOnFire(bool onFire) {
		this->onFire = onFire;
	}
	float getMomentumX() const {
		return momentumDX;
	}
	float getMomentumY() const {
		return momentumDY;
	}
	void setMomentum(float x, float y) {
		momentumDX = x;
		momentumDY = y;
	}
	float getFireRate() const {
		return this->fireRate;
	}
	int getSize() const {
		return this->size;
	}
	void setSize(const int& size) {
		this->size = size;
	}
	float getAngle() const {
		return this->angle;
	}
	void setAngle(const float& angle) {
		this->angle = angle;
	}
	sf::Sprite getP() const {
		return this->p;
	}
	void setP(const sf::Sprite& p) {
		this->p = p;
	}
	int getLives() const {
		return this->lives;
	}
	void setLives(const int& lives) {
		this->lives = lives;
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
	Bullet* getBullets() const {
		return this->bullets;
	}
	void setBullets(Bullet*& bullets) {
		this->bullets = bullets;
	}
	bool isEmpowered() const {
		return this->empowered;
	}
	void setEmpowered(bool power) {
		this->empowered = power;
	}
	int getSpeed() const {
		return this->speed;
	}
	void setSpeed(const int& speed) {
		this->speed = speed;
	}
	int getDamage() const {
		return this->damage;
	}
	void setDamage(const int& damage) {
		this->damage = damage;
	}
	Spaceship();
	void move(sf::RenderWindow& window, std::string d, int controlScheme, sf::Time& dT, int levelNumber = 0);
	void setState(Enemy*& enemyA, int A_Count, Enemy*& enemyB, int B_Count, Enemy*& enemyG, int G_Count, sf::RenderWindow& window, int lives, float posX, float posY, float angle, int speed, int damage, float momentumDX, float momentumDY);
	void setState(int lives, float posX, float posY, float angle, int speed, int damage, float momentumDX, float momentumDY);
	void fire();
	void yeetBullet(Bullet& b);
	void empoweredFire();
	void blink(sf::RenderWindow& window, int& controlScheme, sf::Time& dT, sf::Clock& blinkTimer, float& empoweredTimer, bool& blinkActive);
};

#endif