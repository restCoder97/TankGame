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
	bool getisMoving() {
		return isMoving; };// start moving
	int getHp() { return nHp; }
  int getScore() {
		int i = 0;
		return score;
	}
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
	void eat(Bonus*aBn) {
		if (aBn->getId() == 3) {
			nHp = 100;
			return;
		}
		else if (aBn->getId() == 6) {
			score += 10;
			return;
		}
		if (storedBonus.size() != 0)
			storedBonus.top()->pauseEffictive();
		storedBonus.push(aBn);
		aBn->effictive();
	};// eating bonus

	void checkingBonus() {
		if (storedBonus.size() == 0)
			return;
		if (!storedBonus.top()->checkEffecting() && !storedBonus.top()->getExpired())
			storedBonus.top()->effictive();
		else if(storedBonus.top()->getExpired()){
			Bonus*tmp = storedBonus.top();
			tmp = new Bonus();
			delete tmp;
			storedBonus.pop();
			if (!storedBonus.size() == 0)
				storedBonus.top()->effictive();
		}
	}

	//start moving
	void move();
	int getBulletAmount() {
		return nBullets;
	}
	void addBullet(unsigned int amount) {
		nBullets += amount;
	}
	void setScore(int amount) {
		score = amount;
		if (score <= 0)
			score = 0;
	}

	void switchDirection(direction newDic);
	void stop(bool hittedWall = false);
	bool outOfScreen();

	int getEffectiveBonusID() {
		if (storedBonus.size() != 0)
			return storedBonus.top()->getId();
		return -1;
	}

	void setBullets(int n) {
		nBullets = n;
	}

	void setHP(int n) {
		nHp = n;
	}

	char* serilazationOut(bool firing=false) {
		char*result = new char[256];
		TankData data;
		data.x = spTank->getCenter().x;
		data.y = spTank->getCenter().y;
		data.bullet = nBullets;
		data.score = score;
		data.HP = nHp;
		data.direction = int(spTank->getDc());
		if (firing)
			data.fire = 77;
		memset(result, 0, sizeof(TankData));
		memcpy(result, &data, sizeof(data));
		return result;
	}
	void setDamage(unsigned int n) { nDmg = n; }

	void tankCheating(std::string strBackDoor) {
		Bonus*aBonus = new Bonus();
		if (strBackDoor == "lockhp") {
			aBonus = new LockHP();
			eat(aBonus);
		}

		if (strBackDoor == "accelerate") {
			aBonus = new Accelerate();
			eat(aBonus);
		}

		if (strBackDoor == "damageup") {
			aBonus = new DamageUp();;
			eat(aBonus);
		}

		if (strBackDoor == "recovery") {
			aBonus = new Recovery();
			eat(aBonus);
		}

		if (strBackDoor == "urf") {
			aBonus = new URF();
			eat(aBonus);
		}

		if (strBackDoor == "coin") {
			aBonus = new Coin();
			eat(aBonus);
		}
	}

	~Tank();
};
