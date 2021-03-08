#include "Game.h"

using namespace sf;
using namespace std;
using namespace std::placeholders;
using namespace sb;



BonusType setType(int time=0) {
	int nType = rand() % 7;
	int counter = 0;
	if (time <= 600) {//first 10 minutes game newclear is rare
		if (nType == 5) {
			nType += 1;
		}
	}
	return BonusType(nType);
}

void Game::generateBonus() {
	BonusType type = setType(int(gameClock.getElapsedTime().asSeconds()));
	Bonus*aBonus = nullptr;
	aBonus = new Bonus();
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

void Game::checkKeyboard() {// come to here if keyboard pressed
	if (gamePause||gameOver)
		return;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		player2Tank->switchDirection(direction::top);
		player2Tank->move();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player2Tank->switchDirection(direction::bot);
		player2Tank->move();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player2Tank->switchDirection(direction::left);
		player2Tank->move();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player2Tank->switchDirection(direction::right);
		player2Tank->move();
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
	//checkTanks();
}



void Game::keyboardDown(Event event, bool keyboard,bool textEnterd) {//come to here when keyboard button down or joystick moved;

	auto keyID = event.text.unicode;
	Keyboard::Key a = event.key.code;
	if (a == Keyboard::Key::Escape)
		gamePause = !gamePause;
	if (gamePause|| gameOver) {
		auto callbackTank1 = std::bind(&Tank::tankCheating, playerTank,_1);//placeholders c++ 11
		auto callbackTank2 = std::bind(&Tank::tankCheating, player2Tank, _1);

		if ( keyboard &&textEnterd == false) {
			if (cheatCode1.isFocusing) {
				cheatCode1.readInput(a, callbackTank1);
				if (cheatCode1.getTitle() == "win") {
					player2Tank->setHP(0);
					forcedEnd = true;
					return;
				}

			}
			else if (cheatCode2.isFocusing&& keyboard &&textEnterd == false) {
				cheatCode2.readInput(a, callbackTank2);
				if (cheatCode2.getTitle() == "win") {
					playerTank->setHP(0);
					forcedEnd = true;
					return;
				}
			}
		}

		playerTank->stop();
		player2Tank->stop();
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

	auto axispt2 = Joystick::getAxisPosition(1, Joystick::Z);

	if (axispt2 > -2 && player == 1 && keyboard == false)
		player2Tank->stop();
	if (joystick == Joystick::Z && player == 1 && axispt2 <= -5 && keyboard == false)
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

	if (a == Keyboard::Key::Up || (player == 0 && joystick == Joystick::Y && pt == -100)) {
		playerTank->switchDirection(direction::top);
		if (player != 1)
			playerTank->move();
	}

	else if (a == Keyboard::Key::Down || (player == 0 && joystick == Joystick::Y && pt == 100)) {
		playerTank->switchDirection(direction::bot);
		if (player != 1)
			playerTank->move();
	}
	else if (a == Keyboard::Key::Left || (player == 0 && joystick == Joystick::X && pt == -100)) {
		playerTank->switchDirection(direction::left);
		if (player != 1)
			playerTank->move();
	}
	else if (a == Keyboard::Key::Right || (player == 0 && joystick == Joystick::X && pt == 100)) {
		playerTank->switchDirection(direction::right);
		if (player != 1)
			playerTank->move();
	}
	//checkTanks();
}


void Game::keyboardReleased(Event event) {// come to here if keyboard realeased
	Keyboard::Key aKey = event.key.code;
	if (event.joystickButton.joystickId == 1 && event.joystickButton.button == 2) {
		player2Tank->addBullet((unsigned int)(player2Tank->getScore() / 5));
		player2Tank->setScore((unsigned int)(player2Tank->getScore() % 5));
	}

	if (event.joystickButton.joystickId == 0 && event.joystickButton.button == 2) {
		playerTank->addBullet((unsigned int)(playerTank->getScore() / 5));
		playerTank->setScore((unsigned int)(playerTank->getScore() % 5));
	}


	if (aKey == Keyboard::LShift) {
		int a = player2Tank->getScore();
		player2Tank->addBullet((unsigned int)(player2Tank->getScore() / 5));
		player2Tank->setScore((unsigned int)(player2Tank->getScore() % 5));
	}

	if (aKey == Keyboard::RShift) {
		int b = playerTank->getScore();
		playerTank->addBullet((unsigned int)(playerTank->getScore() / 5));
		playerTank->setScore((unsigned int)(playerTank->getScore() % 5));
	}


	if (event.joystickButton.joystickId == 1 && event.joystickButton.button == 1)
		player2Tank->fire(bulletList);
	if (event.joystickButton.joystickId == 0 && event.joystickButton.button == 1)
		playerTank->fire(bulletList);

	else if (aKey == sf::Keyboard::W || aKey == sf::Keyboard::S || aKey == sf::Keyboard::A || aKey == sf::Keyboard::D) {
		player2Tank->stop();
		checkKeyboard();
	}
	else if (aKey == sf::Keyboard::Up || aKey == sf::Keyboard::Down || aKey == sf::Keyboard::Left || aKey == sf::Keyboard::Right) {
		playerTank->stop();
		checkKeyboard();
	}
	if (aKey == sf::Keyboard::Space) {
		player2Tank->fire(bulletList);
	}
	if (aKey == sf::Keyboard::RControl) {
		playerTank->fire(bulletList);
	}
}


void Game::checkBullets() {// traverse bullet list, check bullets' position, if out of screen then delete it.
	for (auto i = 0; i < int(bulletList.size()); i++) {
		BPoint bulletPeak = bulletList[i]->getPeakPoint();
		if (bulletPeak.x > 1000 || bulletPeak.y >= 1000 || bulletPeak.x <= 0 || bulletPeak.y <= 0) {
			Bullet * tmp = bulletList[i];
			bulletList.erase(bulletList.begin() + i);
			delete tmp;
		}
		else if (playerTank->isContainItems(bulletPeak)) {
			Bullet * tmp = bulletList[i];
			bulletList.erase(bulletList.begin() + i);
			playerTank->damaged(tmp->nDamage);
			player2Tank->addScore(15);
			delete tmp;
		}
		else if (player2Tank->isContainItems(bulletPeak)) {
			Bullet * tmp = bulletList[i];
			bulletList.erase(bulletList.begin() + i);
			player2Tank->damaged(tmp->nDamage);
			playerTank->addScore(15);
			delete tmp;
		}
		else {
			for (auto j = 0; j < int(gameMap->vBricks.size()); j++) {
				if (gameMap->vBricks[j]->sprite.isContaining(bulletPeak)) {
					GIF*aGif = new GIF(strExplosion, gameMap->vBricks[j]->sprite.getCenter());
					boomGifs.push_back(aGif);
					Bullet*tmp = bulletList[i];
					Brick*abrk = gameMap->vBricks[j];
					bulletList.erase(bulletList.begin() + i);
					delete tmp;
					gameMap->vBricks.erase(gameMap->vBricks.begin() + j);
					delete abrk;
				}
			}

			for (auto k = 0; k < int(gameMap->vMetals.size()); k++) {
				if (gameMap->vMetals[k]->sprite.isContaining(bulletPeak)) {
					if (i < int(bulletList.size())) {
						Bullet*tmp = bulletList[i];
						bulletList.erase(bulletList.begin() + i);
						delete tmp;
					}

				}
			}
		}


	}
}

void Game::fly() {// thread function, move bullets
	while (!gameOver) {
		sleep(milliseconds(10));
		if (!gamePause) {
			for (auto i = 0; i < int(bulletList.size()); i++) {
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


void Game::update() {// re-painting game board with new dates.

	gameWindow->clear();
	gameMap->paint(gameWindow);

	gameWindow->draw(playerTank->getSpTank()->getSprite());// draw tank
	gameWindow->draw(player2Tank->getSpTank()->getSprite());// draw tank
	mMutexExplosion.lock();
	for (GIF*aGif : boomGifs) {
		gameWindow->draw(aGif->sp->getSprite());
	}
	mMutexExplosion.unlock();
	for (Bullet*aBullet:bulletList) {// draw bullets one by one
		gameWindow->draw(aBullet->spBullet->getSprite());
	}


	gameWindow->draw(dashBoard);
	gameWindow->draw(tank1Hp.getText());
	gameWindow->draw(tank2Hp.getText());
	int a = playerTank->getEffectiveBonusID();
	if (playerTank->getEffectiveBonusID() != -1) {
		BSprite*tank1Bonus = new BSprite(bonusTexturs[playerTank->getEffectiveBonusID()], BSize(50, 50), top, BPoint(1050, 150));
		gameWindow->draw(tank1Bonus->getSprite());
		delete tank1Bonus;
	}
	if (player2Tank->getEffectiveBonusID() != -1) {
		BSprite*tank2Bonus = new BSprite(bonusTexturs[player2Tank->getEffectiveBonusID()], BSize(50, 50), top, BPoint(1050, 850));
		gameWindow->draw(tank2Bonus->getSprite());
		delete tank2Bonus;
	}

	for (unsigned int i = 0; i < playerTank->getFrontPoints().size(); i++) {
			sf::Vertex point(playerTank->getFrontPoints()[i].getV2f(), sf::Color::Yellow);
			gameWindow->draw(&point, 10, sf::Points);
	}
	for (unsigned int i = 0; i < player2Tank->getFrontPoints().size(); i++) {
			sf::Vertex point(player2Tank->getFrontPoints()[i].getV2f(), sf::Color::Yellow);
			gameWindow->draw(&point, 10, sf::Points);
	}

	//coin draw
	if (player2Tank->storedBonus.size() != 0) {
		int a = player2Tank->storedBonus.top()->getId();
		String b = to_string(a);
		BText tx = BText(b, Color::Red, BPoint(100, 100), myFont);
		gameWindow->draw(tx.getText());
	}

	for (Bonus*tmp : BonusList) {
		gameWindow->draw(tmp->getSprite());
	}


	//score counter draw
	gameWindow->draw(lblP1Score.getText());
	gameWindow->draw(lblP2Score.getText());
	gameWindow->draw(tank1Bullet.getText());
	gameWindow->draw(tank2Bullet.getText());


	if (gamePause&&gameOver == false) {
		gameWindow->draw(pauseView);
		gameWindow->draw(pauseText.getText());
		if (!isOnLineGame) {
			BtContinue.show();
			BtContinue.draw(gameWindow);
		}

		if (!isOnLineGame) {
			cheatCode1.show();
			cheatCode2.show();
			cheatCode1.draw(gameWindow);
			cheatCode2.draw(gameWindow);
		}

	}
	if (gameOver) {// if game end, draw text
		gameWindow->draw(pauseView);
		gameWindow->draw(gameOverText.getText());

	}
	gameWindow->display();
	//aEdit.setFocus(gameWindow);
}


void Game::checkTanks() {// check tank for collision or running out of screen;
	while (!gameOver) {
		if (playerTank->getHp() <= 0) {
			deadTank = playerTank;
			gameOverText.setString("Red Tank Win");
			gameOverText.setFillColor(Color::Red);
			gameOver = true;
			while (!flyThreadEnd) {
				sleep(milliseconds(10));
			}
		}


		else if (player2Tank->getHp() <= 0) {
			deadTank = player2Tank;
			gameOverText.setString("Green Tank Win");
			gameOverText.setFillColor(Color::Green);
			gameOver = true;
			while (!flyThreadEnd) {
				sleep(milliseconds(10));
			}
		}

		if (gamePause)
			continue;

		vector<BPoint>tmp2 = player2Tank->getFrontPoints();
		if (playerTank->isContainItems(player2Tank->getPtMouth(), &tmp2) || player2Tank->outOfScreen())
		{
			if(player2Tank)
 				player2Tank->stop(true);
		}


		vector<BPoint>tmp1 = playerTank->getFrontPoints();
		if (player2Tank->isContainItems(playerTank->getPtMouth(), &tmp1) || playerTank->outOfScreen()) {
			playerTank->stop(true);
		}

		for (unsigned i = 0; i < gameMap->vBricks.size(); i++) {
			if (gameMap->vBricks[i]->sprite.isContaining(playerTank->getPtMouth(), &tmp1))
 				playerTank->stop(true);
			if (i< gameMap->vBricks.size()&&gameMap->vBricks[i]->sprite.isContaining(player2Tank->getPtMouth(), &tmp2))
				player2Tank->stop(true);
		}
		for (auto i = 0; i < int(gameMap->vMetals.size()); i++) {
			if (gameMap->vMetals[i]->sprite.isContaining(playerTank->getPtMouth(), &tmp1))
				playerTank->stop(true);
			if (gameMap->vMetals[i]->sprite.isContaining(player2Tank->getPtMouth(), &tmp2))
				player2Tank->stop(true);
		}

		for (auto i = 0; i <int( BonusList.size()); i++) {
			if (BonusList[i]->isContaining(playerTank->getPtMouth(), &tmp1)) {
				playerTank->eat(BonusList[i]);
				BonusList.erase(BonusList.begin() + i);
			}
		}
		for (unsigned i = 0; i < BonusList.size(); i++) {
			if (BonusList[i]->isContaining(player2Tank->getPtMouth(), &tmp1)) {
				player2Tank->eat(BonusList[i]);
				BonusList.erase(BonusList.begin() + i);
			}
		}
		player2Tank->checkingBonus();
		playerTank->checkingBonus();

		if (gameOver) {
			player2Tank->stop();
			playerTank->stop();
			player2Tank->setGameOver();
			playerTank->setGameOver();
 			animateThread = new std::thread(&Game::playBoom, this);
		}

		std::ostringstream ssScoreP1, ssScoreP2;
		ssScoreP1.str(""); //refresh score label
		ssScoreP2.str("");
		ssScoreP1 << " Pts: " << playerTank->getScore();
		ssScoreP2 << " Pts: " << player2Tank->getScore();
		lblP1Score.setString(ssScoreP1.str());
		lblP2Score.setString(ssScoreP2.str());


		std::ostringstream str1HP, str2Hp;
		std::string hp = " Hp: ";
		str1HP << hp << playerTank->getHp();
		str2Hp << hp << player2Tank->getHp();
		tank1Hp.setString(str1HP.str());
		tank2Hp.setString(str2Hp.str());

		std::ostringstream str1Bullet, str2Bullet;
		std::string Ammo = " Ammo: ";
		str1Bullet << Ammo << playerTank->getBulletAmount();
		str2Bullet << Ammo << player2Tank->getBulletAmount();
		tank1Bullet.setString(str1Bullet.str());
		tank2Bullet.setString(str2Bullet.str());

	}


}



Game::Game() {
	gameWindow = new RenderWindow(VideoMode(1100, 1000), "Game");// init window
	event = new Event();
	playerTank = new Tank(BPoint(500, 100), direction::bot, BSize(50, 50), Color(100, 255, 100, 255));//init player Tank color:r,g,b,a
	player2Tank = new Tank(BPoint(500, 900), direction::top, BSize(50, 50), Color(255, 100, 100, 255));
	flyThread = new std::thread(&Game::fly, this);// init fly thread
	GIFThread = new std::thread(&Game::playExplosion, this);

	gameMap = new GameMap();
	FloatRect gameRect = FloatRect(0, 0, 1000, 1000);
	pauseView= RectangleShape(Vector2f(1000, 1000));
	pauseView.setPosition(Vector2f(0, 0));
	pauseView.setFillColor(Color(0, 0, 0, 230));

	myFont = new Font();
	if (!myFont->loadFromFile("fonts/font2.ttf"))
	{
		std::cout << "Could not load font " << "fonts/font2.ttf" << std::endl;
		if(!myFont->loadFromFile("fonts/font1.otf")){
			std::cerr << "Font1 doesn't work either, full abort\n";
			std::exit(1);
		}
		std::cout << "Using font1 instead\n";
	}


	pauseText = BText("Paused",Color::Red, BPoint(450, 10),myFont,50);
	pauseText.GoCenter(gameRect);
	srand((unsigned int)(time(nullptr)));
	BtContinue = SButton(BSize(100, 50), BPoint(450, 400), Color::Red,"Continue (esc)", gameWindow);
	BtContinue.setTextColor(Color::Blue);

	cheatCode1 = BLineEdit(BPoint(400,10), gameWindow,"Green");
	cheatCode2 = BLineEdit(BPoint(400, 960), gameWindow,"Red");

	for (unsigned i = 0; i < strBnsTxt.size(); i++) {
		std::string path = strBnsTxt[i].insert(0, "images/");
		Texture aTx = Texture();
		try {
			if (!aTx.loadFromFile(path)) {
				throw "Can't find Bonus image!";
			}
		}
		catch(const char*a){
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
		Texture tmp;
		if (!tmp.loadFromFile(strExplosion[i])) {
			std::cerr << "Can't Open file " << strExplosion[i] << std::endl;
			std::exit(1);
		}
		boomTextures.push_back(tmp);
	}


	dashBoard.setFillColor(Color(125, 125, 125));
	dashBoard.setPosition(1000, 0);

	tank1Hp = BText("", Color::Green, BPoint(1000, 0),myFont,35);
	tank2Hp = BText("", Color::Red, BPoint(1000, 900), myFont,35);
	lblP1Score = BText(" ", Color::Green, BPoint(1000, 35), myFont);
	lblP2Score = BText("", Color::Red, BPoint(1000, 935), myFont);
	gameOverText = BText("", Color::Yellow, BPoint(0, 0), myFont, 50);
	tank1Bullet = BText("", Color::Green, BPoint(1000, 65), myFont);
	tank2Bullet = BText("", Color::Red, BPoint(1000, 965), myFont);
	gameOverText.GoCenter(gameRect);
	checkTankThread = new std::thread(&Game::checkTanks, this);

}
void Game::mouseButtonDown(Event e) {
	BPoint pt = BPoint(event->mouseButton.x, event->mouseButton.y);
	cheatCode1.setFocus(pt, gameWindow,event);
	cheatCode2.setFocus(pt, gameWindow,event);
	if (BtContinue.contains(pt))
		gamePause = false;

}

void Game::play() { // call this function to start playing
	gameWindow->requestFocus();
	gameClock.restart();


	while (gameWindow->isOpen())
	{
		if (!gameOver) {
			checkBullets();
			while (gameWindow->pollEvent(*event))// listening events
			{
				if (event->type == Event::EventType::JoystickConnected)
					event->joystickConnect.joystickId = stickConnected++;
				if (event->type == Event::EventType::Closed) {
					gameOver = true;
					gameWindow->close();
				}

				if (event->type == sf::Event::EventType::KeyPressed || event->type == sf::Event::EventType::TextEntered) {
					if (event->type == sf::Event::EventType::TextEntered)
						keyboardDown(*event, false, true);
					else
						keyboardDown(*event, true);
				}

				if (event->type == sf::Event::EventType::JoystickMoved)
					keyboardDown(*event, false);
				if (event->type == sf::Event::EventType::KeyReleased || event->type == sf::Event::EventType::JoystickButtonReleased)//|| Event::EventType::JoystickButtonPressed)
					keyboardReleased(*event);
				if (event->type == sf::Event::EventType::MouseButtonPressed) {
					mouseButtonDown(*event);
				}
			}
			int time = int(gameClock.getElapsedTime().asSeconds());
			int r = rand();
			if ((time % 10) == r %10 && time !=0 && !(BonusList.size() >= 4)) {
				generateBonus();
			}
		}
		if(gameOver) {
			if (boomPlayed) {
				update();
				sleep(milliseconds(2000));
				break;
			}
			else if (forcedEnd) {
				update();
				sleep(milliseconds(2000));
				break;
			}
		}
		update();// re-painting

	}
}

void Game::playExplosion() {
	while (!gameOver) {
		sleep(milliseconds(100));
		mMutexExplosion.lock();
		for (int i = boomGifs.size()-1; i >= 0; i--) {
			if(!boomGifs[i]->playNext())
				boomGifs.erase(boomGifs.begin()+i);
		}
		mMutexExplosion.unlock();

	}
}

void Game::playBoom() {//paint gif
	for (int i = 0; i < 8; i++) {
		if (deadTank) {
			deadTank->getSpTank()->BSetTexture(boomTextures[i]);
			sleep(milliseconds(150));
		}
	}
	boomPlayed = true;
}



Game::~Game() {

	sleep(seconds(1));//give player 1 second to view result
	if (player2Tank)
		delete player2Tank;
	if (playerTank)
		delete playerTank;
	for (unsigned int i = 0; i < bulletList.size(); i++) {
		if(bulletList[i])
			delete bulletList[i];
	}
	for (unsigned int i = 0; i < coinVec.size(); i++) {
		if (coinVec[i])
			delete coinVec[i];
	}

	if (flyThread&&flyThread->joinable()) {
		flyThread->detach();
		delete flyThread;
	}
	if (animateThread&&animateThread->joinable()) {
		animateThread->detach();
		delete animateThread;
	}
	if (checkTankThread&&checkTankThread->joinable()) {
		checkTankThread->detach();
		delete checkTankThread;
	}
	for (unsigned int i = 0; i < BonusList.size(); i++) {
		if (BonusList[i]) {
			BonusList[i] = new Bonus();
			delete BonusList[i];
		}

	}


	if (gameMap)
		delete gameMap;

	if (myFont)
		delete myFont;

	if (event)
		delete event;

	if (gameWindow)
		delete gameWindow;

}
