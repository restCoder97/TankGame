#pragma once

#include"Tank.h"
#include<list>
#include"GameMap.h"
//#include"GameMap.h"
//#include "Bonus.h"

using namespace sf;


class Game
{
	RenderWindow*gameWindow;
	Event event;
	Tank*playerTank;
	AITank*player2Tank;
	BLineEdit aEdit = BLineEdit(BPoint(100, 100), "Hello World!");
	//GameMap gameMap;
	std::thread*flyThread = nullptr;// execute Bullets' move
	std::thread*animateThread = nullptr;
	std::thread*GIFThread = nullptr;
	bool gameOver = false;
	BText gameOverText;
	BText tank1Hp;
	BText tank2Hp;
	BText lblP1Score, lblP2Score;
	BText tank2Bullet, tank1Bullet;

	BText pauseText;
	RectangleShape pauseView;
	Font*MyFont;
	bool flyThreadEnd = false;
	bool boomPlayed;
	std::vector<std::string>strExplosion = { "Explosion_A.png","Explosion_B.png" ,
		"Explosion_C.png" ,"Explosion_D.png" ,"Explosion_E.png" ,"Explosion_F.png" ,"Explosion_G.png" ,"Explosion_H.png" };
	std::vector<string>strBnsTxt = { "hplock.png", "acc.png","DamageUp.png","recovery.png","URF.png","Newclear.png","coin.png" };
	std::vector<Bonus*>BonusList = {};
	bool bonusMechanic=true;
	std::vector<Texture>bonusTexturs = {};
	std::string PtsFileName="binFile/Pts.bin";
	std::vector<BPoint>pts;
	std::vector<Texture*>boomTextures ;
	RectangleShape dashBoard = RectangleShape(Vector2f(100, 1000));
	Tank*deadTank= nullptr;
	GameMap*gameMap;
	int stickConnected = 0;
	Clock gameClock;// will be using for Counting game time and generate random event
	vector<BSprite>explosionList = {};
	vector<GIF*>boomGifs = {};
	Mutex mMutexExplosion;
	void CheckKeyboard();
	void KeyboardDown(Event event, bool keyboard = true);
	void KeyboardReleased(Event event);
	void checkBullets();
	void FLY();
	void update();
	void checkTanks();
	void playBoom();
	void playExplosion();
	bool gamePause=false;
	void GenerateBonus();


public:
	std::vector<Bullet*>bulletList;
        std::vector<Coin*> coinVec;
	Game();
	~Game();
	void play();
};
