#pragma once
#include"Bullet.h"
#include<thread>
#include"Bonus.h"
#include<stack>
using namespace sf;

const Color NoColor = Color(0, 0, 0, 0);
class Tank {
protected:
	Texture aTexture;
	BSprite*spTank = nullptr;
	BSize tankSize;
	direction tankFace = top;
	int nHp = 100;
	int nDamage;
	int nSpeed;
    int score = 0;
    sf::RectangleShape tankBox;
	bool isMoving;
	Clock fireFrequenceClock;
	Texture fireTexture;
	BPoint ptMouth = BPoint(0, 0);
	std::string tankPath = "images/Ntank1.png";
	std::string damageSoundPath = "sounds/attacked.wav";
	std::string fireSoundPath = "sounds/fire.wav";
	std::string moveSoundPath = "sounds/moving.wav";
	std::string explSoundPath = "sounds/Explosion.wav";
	bool gameOver = false;
	bool MvThreadEnd = false;
	BSound * sdExplosion = nullptr;
	BSound * sdFire = nullptr;
	BSound * sdDamaged = nullptr;
	BSound * sdMoving = nullptr;
	Clock stopClock;
	int limitFireFrequence = 500;
	int nBullets = 20;
	bool justSwitchDic = false;
	bool lockedHP = false;
	std::thread * moveThread;
	std::string Name = "";
	int nDmg = 10;


public:
	std::stack<Bonus*>storedBonus;
	bool getisMoving() {return isMoving; };// start moving
	int getHp() { return nHp; }
	int getScore() {return score;}
	BPoint getPtMouth() { return spTank->getPeakPoint(); }// return tank's muzzle point
	BSprite* getSpTank() { return spTank; }
	void setGameOver();
	Tank(BPoint pt, direction dc, BSize size, Color aColor);
	bool isContainItems(BPoint pt = BPoint(0, 0), std::vector<BPoint>*vPoint = nullptr);
	//return tank's muzzle, peak left and peak right
	std::vector<BPoint>getFrontPoints();
	//return tank's top left point
	BPoint getTopLeft();
	void MOVE(); //the only major non-styled part
	void fire(std::vector<Bullet*>&bList);
	std::string getName() { return Name; };
	void setName(std::string str) { Name = str; };

	// get damage
	void damaged(int damage);
    //score functions
	void addScore(unsigned int);
	bool isCollidingWithCoin(Coin*);
	void eat(Bonus*aBn);

	void checkingBonus();

	//start moving
	void move();
	int getBulletAmount() {return nBullets;}
	void addBullet(unsigned int amount) {nBullets += amount;}
	void setScore(int amount);

	void switchDirection(direction newDic);
	void stop(bool hittedWall = false);
	bool outOfScreen();

	int getEffectiveBonusID();

	void setBullets(int n) {nBullets = n;}

	void setHP(int n) {nHp = n;}

	char* serilazationOut(bool firing = false);
	void setDamage(unsigned int n) { nDmg = n; }

	void tankCheating(std::string strBackDoor);

	~Tank();
};
