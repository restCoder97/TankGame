#pragma once
#include"Game.h"
#include"Bullet.h"
#include<thread>
#include"Bonus.h"
#include<stack>
#include"GameMap.h"
#include<chrono>
class AITank : public Tank {
public:
	GameMap* surroundings = nullptr;
	Tank* enemy = nullptr;
	std::vector<Bullet*>* bList = nullptr;
	std::chrono::time_point<std::chrono::system_clock> lastThought;

	//pick a random direction to face.shoot
	void think(bool force) { //the 'force' means to force change dir
		std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
		std::chrono::duration<double> dt = now - lastThought; //dt is in seconds

		//we change dir if we haven't changed in a second, or if we're forced to
		if (dt.count() > 2 || force) {
			//std::cout << dt.count() << std::endl;
			lastThought = now;
			int g;
			direction next = tankFace;
			while (next == tankFace) {
				g = rand() % 4;
				switch (g) {
				case 0:
					next = direction::top;
					break;
				case 1:
					next = direction::right;
					break;
				case 2:
					next = direction::left;
					break;
				case 3:
					next = direction::bot;
					break;
				default:
					break;
				}

			}
			switchDirection(next);
			move();
			g = rand() % 11;
			if (g >= 3)
				fire(*bList); //yes fire at 10% chance each think(). stupid, but testing
		}

	}

	AITank(BPoint pt, direction dc, BSize size, Color aColor, Tank* target, GameMap* map, std::vector<Bullet*>* bvec)
		: Tank(pt, dc, size, aColor) {
		enemy = target;
		surroundings = map;
		bList = bvec;
		lastThought = std::chrono::system_clock::now();
		nBullets = 0xfffffff;
	};

	void fire(std::vector<Bullet*>&bList) {// shooting
		BPoint aPoint = getPtMouth();
		Bullet*aBullet;
		aBullet = new Bullet(getPtMouth(), tankFace, 40);
		aBullet->AIBullet = true;
		bList.push_back(aBullet);
		sdFire->play();
	}
};
