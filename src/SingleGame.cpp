#include "SingleGame.h"

using namespace sf;
using namespace std;
using namespace std::placeholders;
BonusType setaType(int time = 0) {
	int nType = rand() % 7;
	int counter = 0;
	if (time <= 600) {//first 10 minutes game newclear is rare
		if (nType == 5) {
			nType += 1;
		}
	}
	return BonusType(nType);
}



void SingleGame::GenerateBonus() {
	BonusType type = setaType(int(gameClock.getElapsedTime().asSeconds()));
	Bonus*aBonus = new Bonus();
	switch (type)
	{
	case lockHP:
		aBonus = new LockHP();
		break;
	case accelerate:
		aBonus = new Accelerate();
		aBonus->setImage(bonusTexturs[type]);
		break;
	case damageUp:
		aBonus = new DamageUp();
		break;
	case recovery:
		aBonus = new Recovery();
		break;
	case urf:
		aBonus = new URF();
		break;
	case newclear:
		aBonus = new Newclear();
		break;
	case coin:
		aBonus = new Coin();
	default:
		break;
	}
	aBonus->setImage(bonusTexturs[aBonus->getId()]);
	int y = rand() % 950;
	int x = rand() % 950;
	while (x < 25 || y < 25) {
		y = rand() % 950;
		x = rand() % 950;
	}
	aBonus->setCenterAt(BPoint(x, y));
	BonusList.push_back(aBonus);
}

void SingleGame::CheckKeyboard() {// come to here if keyboard pressed
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
	if (sf::Keyboard::isKeyPressed(Keyboard::Key::Up)) {
		playerTank->switchDirection(direction::top);
		playerTank->move();
	}
	if (sf::Keyboard::isKeyPressed(Keyboard::Key::Down)) {
		playerTank->switchDirection(direction::bot);
		playerTank->move();
	}
	if (sf::Keyboard::isKeyPressed(Keyboard::Key::Left)) {
		playerTank->switchDirection(direction::left);
		playerTank->move();
	}
	if (sf::Keyboard::isKeyPressed(Keyboard::Key::Right)) {
		playerTank->switchDirection(direction::right);
		playerTank->move();
	}
	checkTanks();
}

void  SingleGame::KeyboardReleased(Event event) {// come to here if keyboard realeased
	Keyboard::Key aKey = event.key.code;

	if (event.joystickButton.joystickId == 1 && event.joystickButton.button == 1)
		playerTank->fire(bulletList);
	if (event.joystickButton.joystickId == 0 && event.joystickButton.button == 1)
		playerTank->fire(bulletList);

	else if (aKey == sf::Keyboard::W || aKey == sf::Keyboard::S || aKey == sf::Keyboard::A || aKey == sf::Keyboard::D) {
		playerTank->stop();
		CheckKeyboard();
	}
	
	if (aKey == sf::Keyboard::Space) {
		playerTank->fire(bulletList);
	}
	
}

void SingleGame::KeyboardDown(Event event, bool keyboard) {//come to here when keyboard button down or joystick moved;

	auto keyID = event.text.unicode;
	Keyboard::Key a = event.key.code;
	if (a == Keyboard::Key::Escape)
		gamePause = !gamePause;
	if (gamePause || gameOver) {
		auto callbackTank2 = std::bind(&Tank::tankCheating, playerTank, _1);//placeholders c++ 11
		BPoint pt = BPoint(cheatCode2.getPosition().x, cheatCode2.getPosition().y);
		cheatCode2.setFocus(pt, gameWindow, &event);
		if (keyboard) {
			cheatCode2.readInput(a, callbackTank2);
			string str = cheatCode2.getTitle();
			if (cheatCode2.getTitle() == "win") {
				for (AITank*ai : AiTankList) {
					ai->setHP(0);
				}
			}
				
		}
			

		playerTank->stop();
		for (AITank* aiTank : AiTankList)
			aiTank->stop();
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


	checkTanks();
}





void SingleGame::checkBullets() {// traverse bullet list, check bullets' position, if out of screen then delete it.
	Bullet* tmp;
	BPoint bulletPeak;
	bool stop = false;
	for (unsigned int i = 0; i < bulletList.size() && !stop; i++) {
		tmp = bulletList[i];
		bulletPeak = tmp->getPeekPoint();
		if (bulletPeak.x > 1000 || bulletPeak.y >= 1000 || bulletPeak.x <= 0 || bulletPeak.y <= 0) {
			bulletList.erase(bulletList.begin() + i);
			delete tmp;
			stop = true;
		}
		else if (playerTank->isContainItems(bulletPeak)) {
			bulletList.erase(bulletList.begin() + i);
			playerTank->damaged(tmp->nDamage);
			stop = true;
			delete tmp;
		}
		else if (AiTankList.size()) {
			for (AITank* aiTank : AiTankList) {
				//Bullet * tmp = bulletList[i];
				if (aiTank->isContainItems(bulletPeak) && !tmp->AIBullet) {
					bulletList.erase(bulletList.begin() + i);
					aiTank->damaged(tmp->nDamage);
					if (aiTank->getHp() <= 0) {
						GIF*aGif = new GIF(strExplosion, aiTank->getSpTank()->getCenterPoint());
						boomGifs.push_back(aGif);
					}
					playerTank->addScore(15);
					delete tmp;
					stop = true;
					break;

				}
			}
		}

		if (!stop) {
			for (int j = 0; j < gameMap->vBricks.size() && !stop; j++) {
				if (gameMap->vBricks[j]->sprite.isContaining(bulletPeak)) {
					GIF*aGif = new GIF(strExplosion, gameMap->vBricks[j]->sprite.getCenterPoint());
					int newScore = playerTank->getScore()-5;
					playerTank->setScore(newScore);
					boomGifs.push_back(aGif);
					Brick*abrk = gameMap->vBricks[j];
					bulletList.erase(bulletList.begin() + i);
					delete tmp;
					stop = true;
					gameMap->vBricks.erase(gameMap->vBricks.begin() + j);
					delete abrk;
				}
			}

			for (int k = 0; k < gameMap->vMetals.size() && !stop; k++) {
				if (gameMap->vMetals[k]->sprite.isContaining(bulletPeak)) {
					bulletList.erase(bulletList.begin() + i);
					delete tmp;
					stop = true;

				}
			}
		}


	}
}

void SingleGame::FLY() {// thread function, move bullets
	while (!gameOver) {
		sleep(milliseconds(10));
		if (!gamePause) {
			for (unsigned int i = 0; i < bulletList.size(); i++) {
				Bullet*Tmp = bulletList[i];
				direction dirFly = Tmp->dirFly;// get shooting direction
				if (dirFly == 1)
					Tmp->spBullet->move(BulletSpeed, 0);
				else if (dirFly == 3)
					Tmp->spBullet->move(-BulletSpeed, 0);
				else if (dirFly == top)
					Tmp->spBullet->move(0, -BulletSpeed);
				else if (dirFly == bot)
					Tmp->spBullet->move(0, BulletSpeed);
			}
		}
	}
	flyThreadEnd = true;
}





void SingleGame::checkTanks() {// check tank for collision or running out of screen;
	AITank* aiTank;
	for (int i = AiTankList.size()-1; i >=0 ; i--) {
		aiTank = AiTankList[i];
		if (aiTank->getHp() <= 0) {
			deadTank = aiTank;
			AiTankList.erase(AiTankList.begin() +i);
			deadTank->setGameOver();
			delete deadTank;
		}
	}
	
	if (playerTank->getHp() <= 0) {
		deadTank = playerTank;
		std::string text = "AITank Win\n Score: ";
		text.append(to_string(playerTank->getScore()));
		gameOverText.setString(text);
		gameOverText.setFillColor(Color::Red);
		gameOver = true;
		while (!flyThreadEnd) {
			sleep(milliseconds(10));
		}
		deadTank->setGameOver();
	}
	
	else if (AiTankList.size() == 0) {
		std::string text = "YOU WIN \n Score: ";
		text.append(to_string(playerTank->getScore()));
		gameOverText.setString(text);
		gameOverText.setFillColor(Color::Green);
		gameOver = true;
		while (!flyThreadEnd) {
			sleep(milliseconds(10));
		}
	}

	if (gamePause&&!gameOver) {
		for (AITank* aiTank : AiTankList) {
			aiTank->stop();
		}
		return;
	}





	for (AITank* aiTank : AiTankList) {
		vector<BPoint>tmp2 = aiTank->getFrontPoints();
		if (playerTank->isContainItems(aiTank->getPtMouth(), &tmp2) || aiTank->outOfScreen())
		{
			aiTank->think(true);
		}
	}

	vector<BPoint>tmp1 = playerTank->getFrontPoints();
	for (AITank* aiTank : AiTankList) {
		if (aiTank->isContainItems(playerTank->getPtMouth(), &tmp1) || playerTank->outOfScreen()) {
			playerTank->stop(true);
		}
	}

	for (int i = 0; i < gameMap->vBricks.size(); i++) {
		if (gameMap->vBricks[i]->sprite.isContaining(playerTank->getPtMouth(), &tmp1))
			playerTank->stop(true);

		for (AITank* aiTank : AiTankList) {
			vector<BPoint>tmp2 = aiTank->getFrontPoints();
			if (gameMap->vBricks[i]->sprite.isContaining(aiTank->getPtMouth(), &tmp2))
				aiTank->think(true);
		}
	}



	for (auto i = 0; i < gameMap->vMetals.size(); i++) {
		if (gameMap->vMetals[i]->sprite.isContaining(playerTank->getPtMouth(), &tmp1))
			playerTank->stop(true);
		for (AITank* aiTank : AiTankList) {
			vector<BPoint>tmp2 = aiTank->getFrontPoints();
			if (gameMap->vMetals[i]->sprite.isContaining(aiTank->getPtMouth(), &tmp2))
				aiTank->think(true);
		}
	}
	for (int i = 0; i < BonusList.size(); i++) {
		if (BonusList[i]->isContaining(playerTank->getPtMouth(), &tmp1)) {
			playerTank->eat(BonusList[i]);
			BonusList.erase(BonusList.begin() + i);
		}

	}

	playerTank->checkingBonus();

	if (gameOver) {
		playerTank->stop();
		for (AITank* aiTank : AiTankList) {
			aiTank->setGameOver();
			aiTank->stop();
		}
		playerTank->setGameOver();
		if (!didWriteScore) {
			Date newDate;
			Score newScore(playerTank->getName(), playerTank->getScore(), newDate);
			char fileName[] = "gameRecord.txt";
			eraseScoresFile(fileName); //remove previous game record
			writeScoresToFile(fileName, &newScore, 1);
			doScores();
			didWriteScore = true;
		}
		//animateThread = new std::thread(&SingleGame::playBoom, this);
	}

	


	std::ostringstream ssScoreP1;
	ssScoreP1.str(""); //refresh score label
	ssScoreP1 << " Pts: " << playerTank->getScore();
	lblP1Score = BText(ssScoreP1.str(), Color::Green, BPoint(1000, 35), MyFont);


	std::ostringstream str1HP, str2Hp;
	std::string hp = " Hp: ";
	str1HP << hp << playerTank->getHp();
	tank1Hp.setString(str1HP.str());
	std::ostringstream str1Bullet, str2Bullet;
}


SingleGame::SingleGame(std::string Name) {
	gameWindow = new RenderWindow(VideoMode(1100, 1000), "SingleGame");// init window
	flyThread = new std::thread(&SingleGame::FLY, this);// init fly thread
	GIFThread = new std::thread(&SingleGame::playExplosion, this);
	gameMap = new GameMap();
	playerTank = new Tank(BPoint(500, 900), direction::top, BSize(50, 50), Color(100, 255, 100, 255));//init player Tank color:r,g,b,a
	playerTank->setName(Name);
	playerTank->setBullets(0xfffffff);
	AiTankList.push_back(new AITank(BPoint(500, 100), direction::bot, BSize(50, 50), Color(255, 100, 100, 255), playerTank, gameMap, &bulletList));
	AiTankList.push_back(new AITank(BPoint(100, 100), direction::bot, BSize(50, 50), Color(255, 100, 100, 255), playerTank, gameMap, &bulletList));
	AiTankList.push_back(new AITank(BPoint(500, 500), direction::bot, BSize(50, 50), Color(255, 100, 100, 255), playerTank, gameMap, &bulletList));
	AiTankList.push_back(new AITank(BPoint(800, 800), direction::bot, BSize(50, 50), Color(255, 100, 100, 255), playerTank, gameMap, &bulletList));


	MyFont = new Font();
	try{
		MyFont->loadFromFile("fonts/font2.ttf");
	}
	catch(...){
		std::cout << "font2 did not comply, attempting font1\n";
		try{
			MyFont->loadFromFile("fonts/font1.otf");
		}
		catch(...){
			std::cerr << "Didn't work either\n";
			std::exit(1);
		}
	}

	FloatRect gameRect = FloatRect(0, 0, 1000, 1000);
	pauseView = RectangleShape(Vector2f(1000, 1000));
	pauseView.setPosition(Vector2f(0, 0));
	pauseView.setFillColor(Color(0, 0, 0, 230));
	pauseText = BText("Paused", Color::Red, BPoint(450, 10), MyFont, 50);
	cheatCode2 = BLineEdit(BPoint(400, 960), gameWindow, "");
	pauseText.GoCenter(gameRect);
	srand(time(nullptr));
	for (int i = 0; i < strBnsTxt.size(); i++) {
		std::string path = strBnsTxt[i].insert(0, "images/");
		Texture aTx = Texture();
		try {
			if (!aTx.loadFromFile(path)) {
				throw "Can't find Bonus image!";
			}
		}
		catch (const char*a) {
			cerr << a << endl;
			bonusMechanic = false;
			bonusMechanic = {};
			break;
		}
		bonusTexturs.push_back(aTx);
	}


	for (int i = 0; i < 1000; i += 50) {
		for (int j = 0; j < 1000; j += 50) {
			pts.push_back(BPoint(i, j));
		}
	}

	for (int i = 0; i < 8; i++) {
		std::stringstream os;
		os << "images/" << strExplosion[i];
		strExplosion[i] = os.str();
	}
	for (int i = 0; i < 8; i++) {
		Texture*tmp = new Texture();
		if (!tmp->loadFromFile(strExplosion[i])) {
			std::cerr << "Can't Open file " << strExplosion[i] << std::endl;
			std::exit(1);
		}
		boomTextures.push_back(tmp);
	}

	dashBoard.setFillColor(Color(125, 125, 125));
	dashBoard.setPosition(1000, 0);

	tank1Hp = BText("", Color::Green, BPoint(1000, 0), MyFont, 35);
	gameOverText = BText("", Color::Yellow, BPoint(0, 0), MyFont, 50);
	gameOverText.GoCenter(gameRect);
}

void SingleGame::play() { // call this function to start playing
	gameWindow->requestFocus();
	gameClock.restart();


	while (gameWindow->isOpen())
	{
		if (!gameOver) {
			checkBullets();
			checkTanks();
			while (gameWindow->pollEvent(event))// listening events
			{
				if (event.type == Event::EventType::JoystickConnected)
					event.joystickConnect.joystickId = stickConnected++;
				if (event.type == Event::EventType::Closed) {
					playerTank->setGameOver();
					playerTank->setHP(0);
					gameOver = true;
					gameWindow->close();
				}

				if (event.type == sf::Event::EventType::KeyPressed )
					KeyboardDown(event, true);
				if(event.type == sf::Event::EventType::TextEntered)
					KeyboardDown(event, false);
				if (event.type == sf::Event::EventType::JoystickMoved)
					KeyboardDown(event, false);
				if (event.type == sf::Event::EventType::KeyReleased || event.type == sf::Event::EventType::JoystickButtonReleased)//|| Event::EventType::JoystickButtonPressed)
					KeyboardReleased(event);
			}
			int time = int(gameClock.getElapsedTime().asSeconds());
			int r = rand();
			if ((time % 10) == r % 10 && time != 0 && !(BonusList.size() >= 4)) {
				GenerateBonus();
			}
			if (!gamePause) {
				for (AITank* aiTank : AiTankList)
					aiTank->think(false);
			}
		}
		if(gameOver) {
				update();
				break;
		}
		update();// re-painting
	}
}

void SingleGame::playExplosion() {
	while (!gameOver) {
		sleep(milliseconds(100));
		mMutexExplosion.lock();
		for (int i = boomGifs.size() - 1; i >= 0; i--) {
			if (!boomGifs[i]->playNext())
				boomGifs.erase(boomGifs.begin() + i);
		}
		mMutexExplosion.unlock();
	}
}

void SingleGame::playBoom() {//paint gif
	for (int i = 0; i < 8; i++) {
		deadTank->getSpTank()->BSetTexture(*boomTextures[i]);
		sleep(milliseconds(150));
	}
	boomPlayed = true;
}


void SingleGame::update() {// re-painting game board with new dates.

	gameWindow->clear();
	gameMap->paint(gameWindow);

	gameWindow->draw(playerTank->getSpTank()->getSprite());// draw tank
	for (AITank* aiTank : AiTankList)
		gameWindow->draw(aiTank->getSpTank()->getSprite());// draw tank
	mMutexExplosion.lock();
	for (int i = 0; i < boomGifs.size(); i++) {
		gameWindow->draw(boomGifs[i]->sp->getSprite());
	}
	mMutexExplosion.unlock();
	for (unsigned int i = 0; i < bulletList.size(); i++) {// draw bullets one by one
		gameWindow->draw(bulletList[i]->spBullet->getSprite());
	}

	gameWindow->draw(dashBoard);
	gameWindow->draw(tank1Hp.getText());

	for (Bonus*tmp : BonusList) {
		gameWindow->draw(tmp->getSprite());
	}

	gameWindow->draw(lblP1Score.getText());
	gameWindow->draw(tank1Bullet.getText());

	if (gamePause&&gameOver == false) {
		gameWindow->draw(pauseView);
		gameWindow->draw(pauseText.getText());
		cheatCode2.show();
		cheatCode2.draw(gameWindow);
	}
	if (gameOver) {// if game end, draw text
		gameWindow->draw(pauseView);
		gameWindow->draw(gameOverText.getText());
	}
	gameWindow->display();
	//aEdit.setFocus(gameWindow);
}

SingleGame::~SingleGame() {

	//while (!boomPlayed) {
		//sleep(milliseconds(10));
	//}
	sleep(seconds(3));//waiting all thread to end

	for (AITank* aiTank : AiTankList)
		delete aiTank;
	if (playerTank)
		delete playerTank;
	for (unsigned int i = 0; i < bulletList.size(); i++) {
		delete bulletList[i];
	}

	for (unsigned int i = 0; i < boomTextures.size() - 1; i++) {
		if (boomTextures[i])
			delete boomTextures[i];
	}
	int a = 0;
	if (gameWindow)
		delete gameWindow;

}
