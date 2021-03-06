#pragma once
#include"Game.h"







class ServerGame :public Game {
	sf::IpAddress server;
	std::thread*threadRecive;
	std::thread*threadSend;
	sf::TcpSocket socket;
	std::thread*watingThread;
	bool isWaiting = true;


public:
	ServerGame(int port) :Game() {
		isOnLineGame = true;
		sf::TcpListener listener;
		listener.listen(port);
		cout << "Waiting Clint Connect!" << endl;
		//watingThread = new std::thread(&ServerGame::waiting, this);
		listener.accept(socket);
		
		threadRecive = new std::thread(&ServerGame::reciveData, this);
		threadSend = new std::thread(&ServerGame::sendData, this);
		gameWindow->setTitle("Server Game");

	}

	void waiting() {
		listener.accept(socket);
		cout << "Found a clint Game begin!" << endl;
		return;
		while(isWaiting){
			while (gameWindow->pollEvent(*event)) {
				if (event->type == Event::EventType::Closed) {
					gameWindow->close();
					throw("No Clint Found");
				}
			}
		}
		
	}

	void reciveData() {
		while (!gameOver) {
			std::size_t received;
			char*strData = new char[256];
			memset(strData, 0, sizeof(TankData));
			if (socket.receive(strData, sizeof(TankData), received) != Socket::Done)
				return;
			TankData data;
			memcpy((char*)&data, strData, sizeof(TankData));
			if (data.fire != 0)
				player2Tank->fire(bulletList);

			player2Tank->getSpTank()->setCenterAt(BPoint(data.x, data.y));
			player2Tank->setScore(data.score);
			player2Tank->setBullets(data.bullet);
			player2Tank->setHP(data.HP);
			player2Tank->switchDirection(direction(data.direction));
			sleep(milliseconds(1));
		}
	}

	void sendData() {
		while (!gameOver) {
			TankData data;
			char * tmp = playerTank->serilazationOut();
			memcpy(&data, tmp, sizeof(TankData));
			socket.send(tmp, sizeof(TankData));
			sleep(milliseconds(1));
		}
	}

	void KeyboardDown(Event event, bool keyboard = true, bool textEnterd = false) {//come to here when keyboard button down or joystick moved;

		auto keyID = event.text.unicode;
		Keyboard::Key a = event.key.code;
		if (a == Keyboard::Key::Escape)
			gamePause = !gamePause;
		if (gamePause || gameOver) {
			playerTank->stop();
			return;
		}

		auto joystick = event.joystickMove.axis;
		auto pt = event.joystickMove.position;
		auto player = event.joystickMove.joystickId;

		auto axispt1 = Joystick::getAxisPosition(0, Joystick::Z);
		if (axispt1 > -2 && player == 0 && keyboard == false)
			playerTank->stop();
		if (joystick == Joystick::Z && player == 0 && axispt1 <= -5 && keyboard == false)
			playerTank->move();
		if (keyID == 0x16 || (player == 0 && joystick == Joystick::Y && pt == -100)) {
			playerTank->switchDirection(direction::top);
			if (player != 1)
				playerTank->move();
		}
		else if (keyID == 0x12 || (player == 0 && joystick == Joystick::Y && pt == 100)) {
			playerTank->switchDirection(direction::bot);
			if (player != 1)
				playerTank->move();
		}
		else if ((keyID == 0 && keyboard) || (player == 0 && joystick == Joystick::X && pt == -100)) {
			playerTank->switchDirection(direction::left);
			if (player != 1)
				playerTank->move();
		}
		else if (keyID == 0x3 || (player == 0 && joystick == Joystick::X && pt == 100)) {
			playerTank->switchDirection(direction::right);
			if (player != 1)
				playerTank->move();
		}


	}

	void KeyboardReleased(Event event) {
		Keyboard::Key aKey = event.key.code;

		if (event.joystickButton.joystickId == 0 && event.joystickButton.button == 2) {
			playerTank->addBullet((unsigned int)(playerTank->getScore() / 5));
			playerTank->setScore((unsigned int)(playerTank->getScore() % 5));
		}

		if (aKey == Keyboard::LShift) {
			playerTank->addBullet((unsigned int)(playerTank->getScore() / 5));
			playerTank->setScore((unsigned int)(playerTank->getScore() % 5));
		}

		if (event.joystickButton.joystickId == 0 && event.joystickButton.button == 1) {
			playerTank->fire(bulletList);
			char * tmp = playerTank->serilazationOut(true);
			socket.send(tmp, sizeof(TankData));
		}


		else if (aKey == sf::Keyboard::W || aKey == sf::Keyboard::S || aKey == sf::Keyboard::A || aKey == sf::Keyboard::D) {
			playerTank->stop();
			CheckKeyboard();
		}

		if (aKey == sf::Keyboard::Space) {
			playerTank->fire(bulletList);
			char * tmp = playerTank->serilazationOut(true);
			socket.send(tmp, sizeof(TankData));
		}

	}

	void CheckKeyboard() {// come to here if keyboard pressed
		if (gamePause || gameOver)
			return;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			playerTank->switchDirection(direction::top);
			playerTank->move();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			playerTank->switchDirection(direction::bot);
			playerTank->move();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			playerTank->switchDirection(direction::left);
			playerTank->move();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			playerTank->switchDirection(direction::right);
			playerTank->move();
		}

	}


	void play() { // call this function to start playing
		gameWindow->requestFocus();
		gameClock.restart();
		while (gameWindow->isOpen())
		{
			if (!gameOver) {
				checkBullets();

				//checkTanks();
				while (gameWindow->pollEvent(*event))// listening events
				{
					if (event->type == Event::EventType::JoystickConnected)
						event->joystickConnect.joystickId = stickConnected++;
					if (event->type == Event::EventType::Closed)
						gameWindow->close();
					if (event->type == sf::Event::EventType::KeyPressed || event->type == sf::Event::EventType::TextEntered) {
						if (event->type == sf::Event::EventType::TextEntered)
							KeyboardDown(*event, false, true);
						else
							KeyboardDown(*event, true);
					}

					if (event->type == sf::Event::EventType::JoystickMoved)
						KeyboardDown(*event, false);
					if (event->type == sf::Event::EventType::KeyReleased || event->type == sf::Event::EventType::JoystickButtonReleased)//|| Event::EventType::JoystickButtonPressed)
						ServerGame::KeyboardReleased(*event);
					if (event->type == sf::Event::EventType::MouseButtonPressed) {
						mouseButtonDown(*event);
					}
				}

			}
			else {
				if (boomPlayed) {
					update();
				}
			}
			update();// re-painting
		}
	}


};



class ClintGame :public Game {
	sf::IpAddress server;
	std::thread*threadRecive;
	std::thread*threadSend;
	sf::TcpSocket socket;


public:
	ClintGame() = default;
	ClintGame(std::string serverIP, int port) :Game() {
		server = IpAddress(serverIP);

		// Connect to the server
		if (socket.connect(server, port) != sf::Socket::Done) {
			cerr << "Can't find server" << endl;
		}
		threadRecive = new std::thread(&ClintGame::reciveData, this);
		threadSend = new std::thread(&ClintGame::sendData, this);
	}
	void sendData() {
		while (!gameOver) {
			TankData data;
			char * tmp = player2Tank->serilazationOut();
			memcpy(&data, tmp, sizeof(TankData));
			socket.send(tmp, sizeof(TankData));
			sleep(milliseconds(1));
		}
	}

	void reciveData() {
		while (!gameOver) {
			std::size_t received;
			char*strData = new char[256];
			memset(strData, 0, sizeof(TankData));
			if (socket.receive(strData, sizeof(TankData), received) != Socket::Done)
				return;
			TankData data;
			memcpy((char*)&data, strData, sizeof(TankData));
			if (data.fire != 0)
				playerTank->fire(bulletList);
			playerTank->setScore(data.score);
			playerTank->setBullets(data.bullet);
			playerTank->setHP(data.HP);
			playerTank->getSpTank()->setCenterAt(BPoint(data.x, data.y));
			playerTank->switchDirection(direction(data.direction));
			sleep(milliseconds(1));
		}
	}


	void KeyboardDown(Event event, bool keyboard = true, bool textEnterd = false) {//come to here when keyboard button down or joystick moved;

		auto keyID = event.text.unicode;
		Keyboard::Key a = event.key.code;
		if (a == Keyboard::Key::Escape)
			gamePause = !gamePause;
		if (gamePause || gameOver) {
			playerTank->stop();
			player2Tank->stop();
			return;
		}

		auto joystick = event.joystickMove.axis;
		auto pt = event.joystickMove.position;
		auto player = event.joystickMove.joystickId;

		auto axispt1 = Joystick::getAxisPosition(0, Joystick::Z);
		if (axispt1 > -2 && player == 0 && keyboard == false)
			player2Tank->stop();
		if (joystick == Joystick::Z && player == 0 && axispt1 <= -5 && keyboard == false)
			player2Tank->move();

		if (keyID == 0x16 || (player == 1 && joystick == Joystick::Y && pt == -100)) {
			player2Tank->switchDirection(direction::top);
			if (player != 1)
				player2Tank->move();
		}
		else if (keyID == 0x12 || (player == 1 && joystick == Joystick::Y && pt == 100)) {
			player2Tank->switchDirection(direction::bot);
			if (player != 1)
				player2Tank->move();
		}
		else if ((keyID == 0 && keyboard) || (player == 1 && joystick == Joystick::X && pt == -100)) {
			player2Tank->switchDirection(direction::left);
			if (player != 1)
				player2Tank->move();
		}
		else if (keyID == 0x3 || (player == 1 && joystick == Joystick::X && pt == 100)) {
			player2Tank->switchDirection(direction::right);
			if (player != 1)
				player2Tank->move();
		}


	}


	void KeyboardReleased(Event event) {// come to here if keyboard realeased
		Keyboard::Key aKey = event.key.code;


		if (event.joystickButton.joystickId == 0 && event.joystickButton.button == 2) {
			player2Tank->addBullet((unsigned int)(playerTank->getScore() / 5));
			player2Tank->setScore((unsigned int)(playerTank->getScore() % 5));
		}


		if (aKey == Keyboard::LShift) {
			player2Tank->addBullet((unsigned int)(player2Tank->getScore() / 5));
			player2Tank->setScore((unsigned int)(player2Tank->getScore() % 5));
		}

		if (event.joystickButton.joystickId == 0 && event.joystickButton.button == 1) {
			player2Tank->fire(bulletList);
			char * tmp = player2Tank->serilazationOut(true);
			socket.send(tmp, sizeof(TankData));
		}

		else if (aKey == sf::Keyboard::W || aKey == sf::Keyboard::S || aKey == sf::Keyboard::A || aKey == sf::Keyboard::D) {
			player2Tank->stop();
			CheckKeyboard();
		}

		if (aKey == sf::Keyboard::Space) {
			player2Tank->fire(bulletList);
			char * tmp = player2Tank->serilazationOut(true);
			socket.send(tmp, sizeof(TankData));
		}
	}

	void play() { // call this function to start playing
		gameWindow->requestFocus();
		gameClock.restart();
		while (gameWindow->isOpen())
		{
			if (!gameOver) {
				checkBullets();

				//checkTanks();
				while (gameWindow->pollEvent(*event))// listening events
				{
					if (event->type == Event::EventType::JoystickConnected)
						event->joystickConnect.joystickId = stickConnected++;
					if (event->type == Event::EventType::Closed)
						gameWindow->close();
					if (event->type == sf::Event::EventType::KeyPressed || event->type == sf::Event::EventType::TextEntered) {
						if (event->type == sf::Event::EventType::TextEntered)
							KeyboardDown(*event, false, true);
						else
							KeyboardDown(*event, true);
					}

					if (event->type == sf::Event::EventType::JoystickMoved)
						KeyboardDown(*event, false);
					if (event->type == sf::Event::EventType::KeyReleased || event->type == sf::Event::EventType::JoystickButtonReleased)//|| Event::EventType::JoystickButtonPressed)
						ClintGame::KeyboardReleased(*event);
					if (event->type == sf::Event::EventType::MouseButtonPressed) {
						mouseButtonDown(*event);
					}
				}
			}
			else {
				if (boomPlayed) {
					update();
				}
			}
			update();// re-painting
		}
	}
};
