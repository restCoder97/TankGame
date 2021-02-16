#pragma once

#include "BGraphic.h"
#include"Tank.h"
#include<thread>
#include"Bullet.h"
#include<sstream>
#include<iostream>
#include <SFML/Audio.hpp>
#include"GameMap.h"
//#include"GameMap.h"
//#include "Bonus.h"

using namespace sf;


class Game
{
	RenderWindow*gameWindow;
	Event event;
	Tank*playerTank;
	Tank*player2Tank;
	//GameMap gameMap;
	std::thread*flyThread = nullptr;// execute Bullets' move
	std::thread*animateThread = nullptr;
	std::thread*GIFThread = nullptr;
	bool gameOver = false;
	Text gameOverText;
	Text tank1Hp;
	Text tank2Hp;
	Font*MyFont;
	bool flyThreadEnd = false;
	bool boomPlayed;
	std::vector<std::string>strExplosion = { "Explosion_A.png","Explosion_B.png" ,
		"Explosion_C.png" ,"Explosion_D.png" ,"Explosion_E.png" ,"Explosion_F.png" ,"Explosion_G.png" ,"Explosion_H.png" };
	std::string PtsFileName="binFile/Pts.bin";
	std::vector<BPoint>pts;
	std::vector<Texture*>boomTextures ;
	RectangleShape dashBoard = RectangleShape(Vector2f(100, 1000));
	Tank*deadTank= nullptr;
	GameMap gameMap;
	int stickConnected = 0;
	Clock clock;// will be using for Counting game time and generate random event
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

public:
	std::vector<Bullet*>bulletList;
	Game();
	~Game();
	void play();
};
