#pragma once
#include"Game.h";
#include <SFML/Network.hpp>
#include"onlineGame.h"
#include <string> 
#include"SingleGame.h"
#include"LeaderBoard.h"
using namespace std::placeholders;
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
	Font* test = new Font();
	BText singlePlayer = BText("Single Player", Color::White, BPoint(50, 50), test);
	FloatRect leftConfig = FloatRect(107.46 / 2, 84.19 / 2, 420 / 2, 39 / 2);
	BLineEdit searchBox = BLineEdit(BPoint(50, 100), windowInstance, "Player Name");
	SButton play = SButton(BSize(93 / 2, 31 / 2), BPoint(176 / 2, 361 / 2), Color::Transparent, "Play", windowInstance);
	SButton scores = SButton(BSize(93 / 2, 28 / 2), BPoint(176 / 2, 594 / 2), Color::Transparent, "Scores", windowInstance);
	SButton globalScores = SButton(BSize(124 / 2, 33 / 2), BPoint(176 / 2, 692 / 2), Color::Transparent, "Global Scores", windowInstance);
	BText multiPlayer = BText("Multiplayer", Color::White, BPoint(200, 50), test);
	FloatRect centerConfig = FloatRect(912 / 2, 100 / 2, 392 / 2, 36 / 2);
	SButton multiPlay = SButton(BSize(113 / 2, 34 / 2), BPoint(1046 / 2, 567 / 2), Color::Transparent, "Play", windowInstance);
	SButton aboutDeveloper = SButton(BSize(434 / 2, 36 / 2), BPoint(891 / 2, 752 / 2), Color::Transparent, "About Developer", windowInstance);
	BText tankOnline = BText("Tank Online", Color::White, BPoint(350, 50));
	FloatRect rightConfig = FloatRect(1706 / 2, 102 / 2, 354 / 2, 41 / 2);
	BLineEdit ipAddress = BLineEdit(BPoint(1691 / 2, 125), windowInstance, "127.0.0.1", BSize(130, 30));
	BLineEdit portNumber = BLineEdit(BPoint(1691 / 2, 175), windowInstance, "35000", BSize(50, 30));
	BLineEdit portNumber2 = BLineEdit(BPoint(1800 / 2, 553 / 2), windowInstance, "35000", BSize(50, 30));
	SButton join = SButton(BSize(81 / 2, 29 / 2), BPoint(1800 / 2, 441 / 2), Color::Transparent, "Join", windowInstance);
	SButton create = SButton(BSize(149 / 2, 28 / 2), BPoint(1774 / 2, 661 / 2), Color::Transparent, "Create", windowInstance);
public:
	MainPage() {
		if (!texture.loadFromFile("images/t1.png")) {
			cout << "Trial" << endl;
		}

		if (!texture2.loadFromFile("images/t2.png")) {
			cout << "Trial" << endl;
		}

		if (!texture3.loadFromFile("images/t3.png")) {
			cout << "Trial" << endl;
		}
		leftSprite = new BSprite(texture, BSize(319, 495), top, BPoint(111, 200));
		middleSprite = new BSprite(texture2, BSize(471, 495), top, BPoint(650, 200));
		rightSprite = new BSprite(texture3, BSize(273, 495), top, BPoint(0, 0));
		leftSprite->setPosition(BPoint(0, 0));
		middleSprite->setPosition(BPoint(320, 0));
		rightSprite->setPosition(BPoint(792, 0));
		test->loadFromFile("fonts//font2.ttf");
		singlePlayer.setAlignment(leftConfig, topLeft);
		play.setTextColor(Color(255, 255, 0, 255));
		scores.setTextColor(Color(255, 255, 0, 255));
		globalScores.setTextColor(Color(255, 255, 0, 255));
		// Middle of menu buttons
		multiPlayer.GoCenter(centerConfig);
		multiPlay.setTextColor(Color(255, 255, 0, 255));
		aboutDeveloper.setTextColor(Color(255, 255, 0, 255));
		// Right side of main menu buttons
		tankOnline.setAlignment(rightConfig, centerRight);
		join.setTextColor(Color(255, 255, 0, 255));
		create.setTextColor(Color(255, 255, 0, 255));
		play.show();
		scores.show();
		globalScores.show();
		multiPlay.show();
		aboutDeveloper.show();
		join.show();
		create.show();
	}

	void mouseDown() {
		BPoint pt = BPoint(event->mouseButton.x, event->mouseButton.y);
		searchBox.setFocus(pt, windowInstance, event);
		ipAddress.setFocus(pt, windowInstance, event);
		portNumber.setFocus(pt, windowInstance, event);
		portNumber2.setFocus(pt, windowInstance, event);

		
		auto callbackTank1 = std::bind(&MainPage::pbMultiPlayDown, this);//placeholders c++ 11
		multiPlay.buttonDown(pt, callbackTank1);
		auto callbackTank2 = std::bind(&MainPage::pbSinglePlayDown, this);//placeholders c++ 11
		play.buttonDown(pt, callbackTank2);
		auto callbackTank3 = std::bind(&MainPage::pbScoreDown, this);//placeholders c++ 11
		scores.buttonDown(pt, callbackTank3);
		auto callbackTank4 = std::bind(&MainPage::pbJoinDown, this);//placeholders c++ 11
		join.buttonDown(pt, callbackTank4);
		auto callbackTank5 = std::bind(&MainPage::pbCreateDown, this);//placeholders c++ 11
		create.buttonDown(pt, callbackTank5);
		auto callbackTank6 = std::bind(&MainPage::pbAboutUsDown, this);//placeholders c++ 11
		aboutDeveloper.buttonDown(pt, callbackTank6);
	}
	
	void pbSinglePlayDown() {
		SingleGame aGame(searchBox.getTitle());
		aGame.play();
		int a = 0;
	}
	void pbJoinDown() {
		std::string aString = portNumber2.getTitle();
		std::string::size_type sz;
		int port = stoi(aString, &sz);
		sf::TcpSocket tmpSocket;
		IpAddress server = IpAddress(ipAddress.getTitle());
		if (tmpSocket.connect(server, port) != sf::Socket::Done) {
			cerr << "Can't find server" << endl;
			return;
		}
		ClintGame aGame(ipAddress.getTitle(), port);
		aGame.play();
		
	
	}

	void pbCreateDown() {
		std::string aString = portNumber.getTitle();
		std::string::size_type sz;
		int port = stoi(aString,&sz);
		ServerGame aGame(port);
		aGame.play();
	}

	void pbScoreDown() {
		LeaderBoard aBoard;
		aBoard.show();
	}

	void pbAboutUsDown() {
		
	}

	void pbMultiPlayDown() {
		Game aGame;
		aGame.play();
		int a = 0;
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
		//searchBox.getKey();
		searchBox.show();
		searchBox.draw(windowInstance);
		play.draw(windowInstance);
		scores.draw(windowInstance);
		globalScores.draw(windowInstance);
		//
		windowInstance->draw(multiPlayer.getText());
		multiPlay.draw(windowInstance);
		aboutDeveloper.draw(windowInstance);

		//
		windowInstance->draw(tankOnline.getText());
		join.draw(windowInstance);
		ipAddress.show();
		ipAddress.draw(windowInstance);
		portNumber.show();
		portNumber.draw(windowInstance);
		portNumber2.show();
		portNumber2.draw(windowInstance);
		create.draw(windowInstance);


		windowInstance->display();
	}

};