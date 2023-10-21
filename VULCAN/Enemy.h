#ifndef ENEMY_H_
#define ENEMY_H_

#include "Bomb.h"

class Enemy {
private:
	int lives;
	float posX;
	float posY;
	int variant;

	Bomb* bombs;
	int size;

	int damage;

	sf::Texture t;
	sf::Sprite p;

	int fireRate;
	sf::Time fireTime;

	bool direction;
	sf::Clock fade;
	sf::Sprite silhouette;
	sf::Texture silTex;


public:
	// Specifically for the monster
	sf::Sprite beam;
	sf::Texture beamTex;
	sf::Clock beamClock;
	// Specifically for the last boss.
    sf::Sprite eye;
    sf::Texture eyeTex;
	void increaseAlpha() {
		if(p.getColor().a < 255)
			p.setColor(sf::Color(255, 255, 255, p.getColor().a + 1));
	}
	void lowerAlpha() {
		if(p.getColor().a > 0)
			p.setColor(sf::Color(255, 255, 255, p.getColor().a - 1));
	}
	sf::Sprite getSil() const {
		return silhouette;
	}
	bool silhouetteInPlace() {
		float absX = 0;
		float absY = 0;
		absX = silhouette.getPosition().x - posX;
		if(absX < 0) absX = -absX;
		absY = silhouette.getPosition().y - posY;
		if(absY < 0) absY = -absY;
		if(absX < 3 && absY < 3)
			return true;
		return false;
	}
	sf::Clock getFade() const {
		return fade;
	}
	void reveal() {
		p.setPosition(posX, posY);
	}
	bool isSpawned() {
		return silhouette.getColor().a <= 2;
	}
	void lowerFade() {
		p.setPosition(posX, posY);
		fade.restart();
		if(silhouette.getColor().a <= 2)
			return;
		silhouette.setColor(sf::Color(255, 255, 255, silhouette.getColor().a - 1));
	}
	void moveFade(float x, float y) {
		silhouette.move(x, y);
	}
	void initalizeFade(int variant) {
		p.setPosition(5000, 5000);
		silTex.loadFromFile("spritesheet/Invaders_Silhouette.png");
		silhouette.setTexture(silTex);
		fade.restart();
		silhouette.setOrigin(36, 36);
		if(variant == 0)
			silhouette.setTextureRect(sf::IntRect(0, 0, 72, 72));
		else if(variant == 1)
			silhouette.setTextureRect(sf::IntRect(72, 0, 72, 72));
		else if(variant == 2)
			silhouette.setTextureRect(sf::IntRect(144, 0, 72, 72));
		silhouette.setColor(sf::Color(255, 255, 255, 255));
		silhouette.setPosition(640, 640);
	}
	void restartFade() {
		fade.restart();
	}
	sf::Time getFireTime() const {
		return fireTime;
	}
	void setFireTime(sf::Time f) {
		this->fireTime = f;
	}
	void rectSet(float a, float b, float c, float d)
	{
		p.setTextureRect(sf::IntRect(a, b, c, d));
	}
	void textureLoad(std::string s) 
	{
		t.loadFromFile(s);
	}
	void textureSet() 
	{
		p.setTexture(t);
	}
	void positionSet(float x, float y) 
	{
		posX = x;
		posY = y;
		p.setPosition(posX, posY);
	}
	void originSet(float x, float y) 
	{
		p.setOrigin(x, y);
	}
	static int enemyCount;
	void deleteBombs() {
		if(bombs != nullptr)
			delete[] bombs;
	}
	void setFireRate(int fireRate) {
		this->fireRate = fireRate;
	}
	Bomb* getBombs() const {
		return this->bombs;
	}
	void setBombs(Bomb* bombs) {
		this->bombs = bombs;
	}
    void setVariant(int v) {
        variant = v;
    }
    sf::Texture getT() const {
        return t;
    }
	bool getDirection() const {
		return direction;
	}
	void setDirection(bool d) {
		direction = d;
	}

	int getFireRate() const {
		return this->fireRate;
	}
	int getSize() const {
		return this->size;
	}
	void setSize(const int& size) {
		this->size = size;
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
	int getDamage() const {
		return this->damage;
	}
	void setDamage(const int& damage) {
		this->damage = damage;
	}
	int getVariant() const {
		return variant;
	}

	Enemy() {}
	virtual void initialize(float offset) = 0;
	virtual void fire(float playerX, float playerY) = 0;
	virtual bool isOutside() = 0;
	virtual void move(sf::Time dT) = 0;
	void yeetBomb(Bomb& b) {b.setCoords(10000, 10000);}
	virtual ~Enemy() {}
};

#endif