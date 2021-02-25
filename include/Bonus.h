#pragma once
#include "BGraphic.h"
#include<thread>
#include"Bullet.h"
#include<sstream>
#include<iostream>
 
enum BonusType { lockHP, accelerate, damageUp, recovery,urf ,newclear,coin };
using namespace sf;

class Bonus:public BSprite{
protected:
	BonusType thisType;
	int effectiveTime=-1;
	int ID = -1;
	Clock effectiveClock;
	bool effecting=false;
	bool expired = false;
	bool pausedEffecting;
public:
	Bonus() : BSprite(Texture(), BSize(50, 50), top, BPoint(0, 0)) {};
	int getEffectTime() { return effectiveTime; };
	void setImage(Texture&aText) {
		setTextureRect(IntRect(0, 0, 50, 50));
		BSetTexture(aText);
	}
	int getId(){ return ID; };
	void effictive() {
		effectiveClock.restart();
		effecting = true;
	}
	void pauseEffictive() {
		effectiveTime -= int(effectiveClock.getElapsedTime().asSeconds());
		effecting = false;
	}
	
	bool checkEffecting() {
		int nT = int(effectiveClock.getElapsedTime().asSeconds());
		if (nT < effectiveTime)
			effecting = true;
		else {
			expired = true;
			effecting = false;
		}
		return effecting;
	}

	bool getExpired() {
		return expired;
	}
};

class LockHP :public Bonus {
public:
	LockHP():Bonus() { 
		effectiveTime = 10;
		thisType = lockHP;
		ID = 0;
	}
};

class Accelerate :public Bonus {
public:
	Accelerate() :Bonus() {
		effectiveTime = 10;
		thisType = accelerate;
		ID = 1;
	}
};

class DamageUp :public Bonus {
public:
	DamageUp() :Bonus() {
		effectiveTime = 10;
		thisType = damageUp;
		ID = 2;
	}
};

class Recovery :public Bonus {
public:
	Recovery() :Bonus() {
		effectiveTime = 0;
		thisType = recovery;
		ID = 3;
	}
};

class Newclear :public Bonus {
public:
	Newclear():Bonus() {
		effectiveTime = 0;
		thisType = newclear;
		ID = 5;
	}
};

class URF :public Bonus {
public:
	URF() :Bonus() {
		effectiveTime = 5;
		thisType = urf;
		ID = 4;
	}
};

class Coin : public Bonus {
public:
	Coin() : Bonus() {
		effectiveTime = 0;
		thisType = coin;
		ID = 6;
	}

};