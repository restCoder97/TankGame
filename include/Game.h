#pragma once
#include"Tank.h"
#include<list>
#include<map>
#include"GameMap.h"
#include"SWeidget.h"
#include <SFML/Network.hpp>


//#include"GameMap.h"
//#include "Bonus.h"

using namespace sf;
using namespace sb;

class Game
{
protected:

	RenderWindow*gameWindow;
	Event*event = nullptr;
	Tank*playerTank;
	Tank*player2Tank;
	std::thread*flyThread = nullptr;// execute Bullets' move
	std::thread*animateThread = nullptr;
	std::thread*GIFThread = nullptr;
	std::thread*checkTankThread = nullptr;
	bool gameOver = false;
	BText gameOverText;
	BText tank1Hp;
	BText tank2Hp;
	BText lblP1Score, lblP2Score;
	BText tank2Bullet, tank1Bullet;
	bool isOnLineGame = false;
	bool forcedEnd;
	BText pauseText;
	RectangleShape pauseView;
	Font*myFont;
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
	std::vector<Texture>boomTextures ;
	RectangleShape dashBoard = RectangleShape(Vector2f(100, 1000));
	SButton BtContinue;
	BLineEdit cheatCode1;
	BLineEdit cheatCode2;

	Tank*deadTank= nullptr;
	GameMap*gameMap;
	int stickConnected = 0;
	Clock gameClock;// will be using for Counting game time and generate random event
	vector<BSprite>explosionList = {};
	vector<GIF*>boomGifs = {};
	Mutex mMutexExplosion;
	std::vector<Bullet*>bulletList;
	std::vector<Coin*> coinVec;

	void checkKeyboard();
	void keyboardDown(Event event, bool keyboard = true,bool textEntered = false);
	void keyboardReleased(Event event);
	void checkBullets();
	void update();
	void checkTanks();
	bool gamePause=false;
	void generateBonus();
	void mouseButtonDown(Event e);

public:

	void playBoom();
	void playExplosion();
	Game();
	void fly();
	~Game();
	void play();
};
