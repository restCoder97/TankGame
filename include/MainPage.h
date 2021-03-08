#pragma once

#include"Game.h"
#include <SFML/Network.hpp>
#include"onlineGame.h"
#include <string>
#include"SingleGame.h"
#include"leaderBoard.h"
#include "AboutUs.h"
using namespace std::placeholders;
using namespace sb;
//using namespace bg;
class MainPage {
	RenderWindow* windowInstance = new RenderWindow(VideoMode(319 + 471 + 273, 495), "Tank 2021");
	Event*event=new Event();

	// Main menu left side buttons
	Texture texture;
	Texture texture2;
	Texture texture3;

	BSprite* leftSprite = new BSprite(texture, BSize(319, 495), top, BPoint(111, 200));
	BSprite* middleSprite = new BSprite(texture2, BSize(471, 495), top, BPoint(650, 200));
	BSprite* rightSprite = new BSprite(texture3, BSize(273, 495), top, BPoint(0, 0));

	BText singlePlayer = BText("Single Player", Color::White, BPoint(50, 50));
	FloatRect leftConfig = FloatRect(float(107.46 / 2), float(84.19 / 2), float(420 / 2), float(39 / 2));
	BLineEdit searchBox = BLineEdit(BPoint(50, 100), windowInstance, "Player Name");
	SButton play = SButton(BSize(93 / 2, 31 / 2), BPoint(176 / 2, 361 / 2),
		Color::Transparent, "Play", windowInstance);
	SButton scores = SButton(BSize(93 / 2, 28 / 2), BPoint(176 / 2, 594 / 2),
		Color::Transparent, "Scores", windowInstance);
	BText multiPlayer = BText("Multiplayer", Color::White, BPoint(200, 50));
	FloatRect centerConfig = FloatRect(912 / 2, 100 / 2, 392 / 2, 36 / 2);
	SButton multiPlay = SButton(BSize(113 / 2, 34 / 2), BPoint(1046 / 2, 567 / 2),
		Color::Transparent, "Play", windowInstance);
	SButton aboutDeveloper = SButton(BSize(434 / 2, 36 / 2), BPoint(891 / 2, 752 / 2+50)
		, Color::Transparent, "About Developer", windowInstance);
	BText tankOnline = BText("Tank Online", Color::White, BPoint(300, 50));
	FloatRect rightConfig = FloatRect(1706 / 2, 102 / 2, 354 / 2, 41 / 2);
	BLineEdit ipAddress = BLineEdit(BPoint(1691 / 2, 125), windowInstance, "127.0.0.1", BSize(130, 30));
	BLineEdit portNumber = BLineEdit(BPoint(1691 / 2, 175), windowInstance, "35000", BSize(50, 30));//defualt port number 35000
	BLineEdit portNumber2 = BLineEdit(BPoint(1800 / 2, 553 / 2), windowInstance, "35000", BSize(50, 30));
	SButton join = SButton(BSize(81 / 2, 29 / 2), BPoint(1800 / 2, 441 / 2),
		Color::Transparent, "Join", windowInstance);
	SButton create = SButton(BSize(149 / 2, 28 / 2), BPoint(1774 / 2, 661 / 2)
		, Color::Transparent, "Create", windowInstance);
	SButton music = SButton(BSize(149 / 2, 28 / 2), BPoint(319 + 471 +173,465)
		, Color::Transparent, "MUSIC", windowInstance);
	BSound * aSound=nullptr;
public:
	MainPage() {
		aSound = new BSound("sounds/music1.wav",true);
		aSound->setVolume(50);
		aSound->play();
		try {
			texture.loadFromFile("images/t1.png");
		}
		catch (...) {
			cerr << "picture 1 not found " << endl;
			exit(1);
		}
		try {
			texture2.loadFromFile("images/t2.png");
		}
		catch (...) {
			cerr << "picture 2 not found " << endl;
			exit(1);
		}
		try {
			texture3.loadFromFile("images/t3.png");
		}
		catch (...) {
			cerr << "picture 3 not found " << endl;
			exit(1);
		}

		leftSprite = new BSprite(texture, BSize(319, 495), top, BPoint(111, 200));
		middleSprite = new BSprite(texture2, BSize(471, 495), top, BPoint(650, 200));
		rightSprite = new BSprite(texture3, BSize(273, 495), top, BPoint(0, 0));
		leftSprite->setPosition(BPoint(0, 0));
		middleSprite->setPosition(BPoint(320, 0));
		rightSprite->setPosition(BPoint(792, 0));
		singlePlayer.setAlignment(leftConfig, topLeft);
		play.setTextColor(Color(255, 255, 0, 255));
		scores.setTextColor(Color(255, 255, 0, 255));
		// Middle of menu buttons
		multiPlayer.GoCenter(centerConfig);
		multiPlay.setTextColor(Color(255, 255, 0, 255));
		aboutDeveloper.setTextColor(Color(255, 255, 0, 255));
		// Right side of main menu buttons
		tankOnline.setAlignment(rightConfig, centerRight);
		join.setTextColor(Color(255, 255, 0, 255));
		create.setTextColor(Color(255, 255, 0, 255));
		music.setTextColor(Color(255, 255, 0, 255));
		play.show();
		scores.show();
		multiPlay.show();
		aboutDeveloper.show();
		join.show();
		create.show();
		music.show();
	}

	void mouseDown() {
		BPoint pt = BPoint(event->mouseButton.x, event->mouseButton.y);
		searchBox.setFocus(pt, windowInstance, event);
		ipAddress.setFocus(pt, windowInstance, event);
		portNumber.setFocus(pt, windowInstance, event);
		portNumber2.setFocus(pt, windowInstance, event);

		auto callbackTank1 = std::bind(&MainPage::pbMultiPlayDown, this);
		multiPlay.buttonDown(pt, callbackTank1);
		auto callbackTank2 = std::bind(&MainPage::pbSinglePlayDown, this);
		play.buttonDown(pt, callbackTank2);
		auto callbackTank3 = std::bind(&MainPage::pbScoreDown, this);
		scores.buttonDown(pt, callbackTank3);
		auto callbackTank4 = std::bind(&MainPage::pbJoinDown, this);
		join.buttonDown(pt, callbackTank4);
		auto callbackTank5 = std::bind(&MainPage::pbCreateDown, this);
		create.buttonDown(pt, callbackTank5);
		auto callbackTank6 = std::bind(&MainPage::pbAboutUsDown, this);
		aboutDeveloper.buttonDown(pt, callbackTank6);
		auto callbackTank7 = std::bind(&MainPage::pbMusic, this);
		music.buttonDown(pt, callbackTank7);
	}

	void pbSinglePlayDown() {
		aSound->stop();
		SingleGame aGame(searchBox.getTitle());
		aGame.play();
		aSound->play();
	}

	void pbJoinDown() {

		std::string aString = portNumber.getTitle();
		std::string::size_type sz;
		int port = stoi(aString, &sz);
		IpAddress*server = new IpAddress(ipAddress.getTitle());
		TcpSocket*socket = new TcpSocket();
		if (socket->connect(*server, port) != sf::Socket::Done) {
			cerr << "Can't find server" << endl;
			delete server;
			delete socket;
			return;
		}
		aSound->stop();
		ClientGame aGame(server,socket);
		aGame.play();
		aSound->play();
		delete server;
		delete socket;
	}

	void pbCreateDown() {
		aSound->stop();
		std::string aString = portNumber2.getTitle();
		std::string::size_type sz;
		int port = stoi(aString,&sz);
		ServerGame aGame(port);
		aGame.play();
		aSound->play();
	}

	void pbScoreDown() {
		LeaderBoard aBoard;
		aBoard.show();
	}

	void pbAboutUsDown() {
		AboutUs abtUS;
		abtUS.show();
	}

	void pbMultiPlayDown() {
		aSound->stop();
		Game aGame;
		aGame.play();
		aSound->play();
		int a = 0;
	}
	void pbMusic() {
		if (aSound->getStatus() == Sound::Paused || aSound->getStatus() == Sound::Stopped) {
			aSound->play();
		}
		else
			aSound->stop();
	}

	void keyboardDown(Event event, bool keyboard=true, bool textEnterd=false) {
		Keyboard::Key keyCode = event.key.code;
		if (textEnterd == false) {
			if (searchBox.isFocusing)
				searchBox.readInput(keyCode);
			else if (portNumber.isFocusing)
				portNumber.readInput(keyCode);
			else if (portNumber2.isFocusing)
				portNumber2.readInput(keyCode);
			else if (ipAddress.isFocusing)
				ipAddress.readInput(keyCode);
			else if (portNumber.isFocusing)
				portNumber.readInput(keyCode);
		}

	}

	void display() {
		while (windowInstance->isOpen()) {
			while (windowInstance->pollEvent(*event)) {
				update();
				if (event->type == Event::EventType::Closed)
					windowInstance->close();
				if (event->type == sf::Event::EventType::KeyPressed ) {
					keyboardDown(*event);
				}
				if (event->type == sf::Event::EventType::TextEntered) {
					keyboardDown(*event,false,true);
				}
				if (event->type == sf::Event::EventType::MouseButtonPressed) {
					mouseDown();
				}
			}
		}
	}


	void update() {
		windowInstance->clear();
		windowInstance->draw(leftSprite->getSprite());
		windowInstance->draw(middleSprite->getSprite());
		windowInstance->draw(rightSprite->getSprite());
		windowInstance->draw(singlePlayer.getText());

		searchBox.show();
		searchBox.draw(windowInstance);
		play.draw(windowInstance);
		scores.draw(windowInstance);
		//
		windowInstance->draw(multiPlayer.getText());
		multiPlay.draw(windowInstance);
		aboutDeveloper.draw(windowInstance);

		windowInstance->draw(tankOnline.getText());
		join.draw(windowInstance);
		ipAddress.show();
		ipAddress.draw(windowInstance);
		portNumber.show();
		portNumber.draw(windowInstance);
		portNumber2.show();
		portNumber2.draw(windowInstance);
		create.draw(windowInstance);
		music.draw(windowInstance);
		windowInstance->display();
	}
	~MainPage() {
		if(windowInstance)
			delete windowInstance;
		if (leftSprite)
			delete leftSprite;
		if (middleSprite)
			delete middleSprite;
		if (rightSprite)
			delete rightSprite;
		if (event)
			delete event;
		if (aSound)
			delete aSound;
	}
};
