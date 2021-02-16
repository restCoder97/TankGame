#pragma once
#include<SFML/System.hpp>
#include<SFML/Graphics.hpp>
#include<string>
#include "BGraphic.h"
#include"Bullet.h"
#include"Coin.h"
#include<thread>
#include <SFML/Audio.hpp>
#include<iostream>
#include<sstream>
using namespace sf;

class GIF {
public:
	int indexOfPic;
	BSprite*sp;
	std::vector<Texture*>txs;

	GIF(std::vector<std::string>fileName, BPoint pt ) {
		
		for (int i = 0; i < fileName.size(); i++) {
			Texture*tmp = new Texture();
			if (!tmp->loadFromFile(fileName[i])) {
				std::cerr << "Can't Open file " << fileName[i] << std::endl;
				std::exit(1);
			}
			txs.push_back(tmp);
		}
		sp = new BSprite(*txs[0], BSize(50, 50), top, pt);
	}

	bool playNext() {
		indexOfPic++;
		if (indexOfPic == txs.size())
			return false;
		sp->BSetTexture(*txs[indexOfPic]);
		return true;
	}

};

const Color NoColor = Color(0, 0, 0, 0);
class Tank {
	Texture aTexture;
	BSprite*spTank;
	BSize tankSize;
	direction tankFace = top;
	int nHp = 100;
	int nDamage;
	int nSpeed;
        unsigned int score = 0;
        sf::RectangleShape tankBox;
	BLine trajectory;
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
	bool justSwitchDic = false;
	//std::thread animatedThread;
	std::thread * moveThread;

public:
	bool getisMoving() { 
		return isMoving; };// start moving
	int getHp() { return nHp; }
        unsigned int getScore() { return score; }
	BPoint getPtMouth() { return spTank->getPeakPoint(); }// return tank's muzzle point
	BSprite* getSpTank() { return spTank; }
	void setGameOver();
	Tank(BPoint pt, direction dc, BSize size, Color aColor);
	bool isContainItems(BPoint pt = BPoint(0, 0), std::vector<BPoint>*vPoint = nullptr);
	//return tank's muzzle, peak left and peak right
	std::vector<BPoint>getFrontPoints();
	//return tank's top left point
	BPoint getTopLeft();
	void MOVE();
	void fire(std::vector<Bullet*>&bList);
	// get damage
	void damaged(int damage);
        //score functions
        void addScore(unsigned int);
        bool isCollidingWithCoin(Coin*);
	void eat() {};// eating bonus
	//start moving
	void move();
	void switchDirection(direction newDic);
	void stop(bool hittedWall = false);
	bool outOfScreen();
	~Tank();
};
