#include "Game.h"

using namespace sf;
using namespace std;


void Game::CheckKeyboard() {// come to here if keyboard pressed
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
	checkTanks();
}


void Game::KeyboardDown(Event event, bool keyboard) {//come to here when keyboard button down or joystick moved;
	
	auto keyID = event.text.unicode;
	Keyboard::Key a = event.key.code;
	if (a == Keyboard::Key::Escape)
		gamePause = !gamePause;
	if (gamePause|| gameOver) {
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
	checkTanks();
}


void Game::KeyboardReleased(Event event) {// come to here if keyboard realeased
	Keyboard::Key aKey = event.key.code;
	if (event.joystickButton.joystickId == 1 && event.joystickButton.button == 1)
		player2Tank->fire(bulletList);
	if (event.joystickButton.joystickId == 0 && event.joystickButton.button == 1)
		playerTank->fire(bulletList);

	else if (aKey == sf::Keyboard::W || aKey == sf::Keyboard::S || aKey == sf::Keyboard::A || aKey == sf::Keyboard::D) {
		player2Tank->stop();
		CheckKeyboard();
	}
	else if (aKey == sf::Keyboard::Up || aKey == sf::Keyboard::Down || aKey == sf::Keyboard::Left || aKey == sf::Keyboard::Right) {
		playerTank->stop();
		CheckKeyboard();
	}
	if (aKey == sf::Keyboard::Space) {
		player2Tank->fire(bulletList);
	}
	if (aKey == sf::Keyboard::RControl) {
		playerTank->fire(bulletList);
	}
}


void Game::checkBullets() {// traverse bullet list, check bullets' position, if out of screen then delete it.
	for (unsigned int i = 0; i < bulletList.size(); i++) {
		BPoint bulletPeak = bulletList[i]->getPeekPoint();
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
			for (int j = 0; j < gameMap->vBricks.size(); j++) {
				if (gameMap->vBricks[j]->sprite.isContaining(bulletPeak)) {
					GIF*aGif = new GIF(strExplosion, gameMap->vBricks[j]->sprite.getCenterPoint());
					boomGifs.push_back(aGif);
					Bullet*tmp = bulletList[i];
					Brick*abrk = gameMap->vBricks[j];
					bulletList.erase(bulletList.begin() + i);
					delete tmp;
					gameMap->vBricks.erase(gameMap->vBricks.begin() + j);
					delete abrk;
				}
			}

			for (int k = 0; k < gameMap->vMetals.size(); k++) {
				if (gameMap->vMetals[k]->sprite.isContaining(bulletPeak)) {
					if (i < bulletList.size()) {
						Bullet*tmp = bulletList[i];
						bulletList.erase(bulletList.begin() + i);
						delete tmp;
					}
					
				}
			}
		}
		

	}
}

void Game::FLY() {// thread function, move bullets
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


void Game::update() {// re-painting game board with new dates.

	gameWindow->clear();
	gameMap->paint(gameWindow);
	gameWindow->draw(playerTank->getSpTank()->getSprite());// draw tank
	gameWindow->draw(player2Tank->getSpTank()->getSprite());// draw tank
	mMutexExplosion.lock();
	for (int i = 0; i < boomGifs.size(); i++) {
		gameWindow->draw(boomGifs[i]->sp->getSprite());
	}
	mMutexExplosion.unlock();
	for (unsigned int i = 0; i < bulletList.size(); i++) {// draw bullets one by one
		gameWindow->draw(bulletList[i]->spBullet->getSprite());
	}
	
	//painting two tank's front line(three points):
	/*for (unsigned int i = 0; i < playerTank->getFrontPoints().size(); i++) {
		sf::Vertex point(playerTank->getFrontPoints()[i].getV2f(), sf::Color::Yellow);
		gameWindow->draw(&point, 10, sf::Points);
	}
	for (unsigned int i = 0; i < player2Tank->getFrontPoints().size(); i++) {
		sf::Vertex point(player2Tank->getFrontPoints()[i].getV2f(), sf::Color::Yellow);
		gameWindow->draw(&point, 10, sf::Points);
	}*/
	gameWindow->draw(dashBoard);
	gameWindow->draw(tank1Hp.getText());
	gameWindow->draw(tank2Hp.getText());
	//coin draw
	for (unsigned int i = 0; i < coinVec.size(); i++) {
	  coinVec[i]->drawTo(*gameWindow);
	}
	//score counter draw
	lblP1Score.setFont(*MyFont);
	lblP2Score.setFont(*MyFont);
	gameWindow->draw(lblP1Score);
	gameWindow->draw(lblP2Score);
	if (gamePause&&gameOver == false) {
		gameWindow->draw(pauseView);
		gameWindow->draw(pauseText.getText());
	}
	if (gameOver) {// if game end, draw text
		gameWindow->draw(pauseView);
		gameWindow->draw(gameOverText.getText());
	}
	gameWindow->display();
}


void Game::checkTanks() {// check tank for collision or running out of screen;
	if (playerTank->getHp() <= 0) {
		deadTank = playerTank;
		gameOverText.setString("Red Tank Win");
		//gameOverText.setFillColor(Color::Red);
		gameOver = true;
		while (!flyThreadEnd) {
			sleep(milliseconds(10));
		}
	}

	else if (player2Tank->getHp() <= 0) {
		deadTank = player2Tank;
		gameOverText.setString("Green Tank Win");
		//gameOverText.setFillColor(Color::Green);
		gameOver = true;
		while (!flyThreadEnd) {
			sleep(milliseconds(10));
		}
	}

	
	vector<BPoint>tmp2 = player2Tank->getFrontPoints();
	if(playerTank->isContainItems(player2Tank->getPtMouth(),&tmp2)||player2Tank->outOfScreen())
	{
		player2Tank->stop(true);
	}

	
	vector<BPoint>tmp1 = playerTank->getFrontPoints();
	if (player2Tank->isContainItems(playerTank->getPtMouth(),&tmp1) || playerTank->outOfScreen()){
		playerTank->stop(true);
	}

	for (int i = 0; i < gameMap->vBricks.size(); i++) {
		if (gameMap->vBricks[i]->sprite.isContaining(playerTank->getPtMouth(),&tmp1))
			playerTank->stop(true);
		if (gameMap->vBricks[i]->sprite.isContaining(player2Tank->getPtMouth(), &tmp2))
			player2Tank->stop(true);
	}

	for (int i = 0; i < gameMap->vMetals.size(); i++) {
		if (gameMap->vMetals[i]->sprite.isContaining(playerTank->getPtMouth(), &tmp1))
			playerTank->stop(true);
		if (gameMap->vMetals[i]->sprite.isContaining(player2Tank->getPtMouth(), &tmp2))
			player2Tank->stop(true);
	}

	if (gameOver) {
		player2Tank->stop();
		playerTank->stop();
		player2Tank->setGameOver();
		playerTank->setGameOver();
		animateThread = new std::thread(&Game::playBoom, this);
	}

	std::ostringstream str1HP, str2Hp;
	std::string hp = " Hp: ";
	str1HP << hp << playerTank->getHp();
	str2Hp << hp << player2Tank->getHp();
	tank1Hp.setString(str1HP.str());
	tank2Hp.setString(str2Hp.str());
}

void Game::checkCoins() { //check for intersection with coin
  //score counter sstreams
  std::ostringstream ssScoreP1, ssScoreP2;
  ssScoreP1.str(""); //refresh score label
  ssScoreP2.str("");
  ssScoreP1 << " Pts: " << playerTank->getScore();
  ssScoreP2 << " Pts: " << player2Tank->getScore();
  lblP1Score.setString(ssScoreP1.str());
  lblP2Score.setString(ssScoreP2.str());
  
  for (unsigned int i = 0; i < coinVec.size(); i++) {
    if (playerTank->isCollidingWithCoin(coinVec[i])) {
      coinVec[i]->setPos({12500,12500}); //TEMP "DELETE"
      playerTank->addScore(10);
      ssScoreP1.str("");
      ssScoreP1 << " Pts: " << playerTank->getScore();
      lblP1Score.setString(ssScoreP1.str());
    }
    if (player2Tank->isCollidingWithCoin(coinVec[i])) {
      coinVec[i]->setPos({12500,12500}); //TEMP "DELETE"
      player2Tank->addScore(10);
      ssScoreP2.str("");
      ssScoreP2 << " Pts: " << player2Tank->getScore();
      lblP2Score.setString(ssScoreP2.str());
    }
  }
}

Game::Game() {
	gameWindow = new RenderWindow(VideoMode(1100, 1000), "Game");// init window
	playerTank = new Tank(BPoint(500, 900), direction::top, BSize(50, 50), Color(100, 255, 100, 255));//init player Tank color:r,g,b,a
	player2Tank = new Tank(BPoint(500, 100), direction::bot, BSize(50, 50), Color(255, 100, 100, 255));
	flyThread = new std::thread(&Game::FLY, this);// init fly thread
	GIFThread = new std::thread(&Game::playExplosion, this);
	gameMap = new GameMap();
	FloatRect gameRect = FloatRect(0, 0, 1000, 1000);
	pauseView= RectangleShape(Vector2f(1000, 1000));
	pauseView.setPosition(Vector2f(0, 0));
	pauseView.setFillColor(Color(0, 0, 0, 230));
	pauseText = BText("Paused",Color::Red, BPoint(450, 10),50);
	pauseText.GoCenter(gameRect);
	




	//coin stuff
	Coin* coin1 = new Coin({15,15});
	Coin* coin2 = new Coin({15,15});
	Coin* coin3 = new Coin({15,15});
	Coin* coin4 = new Coin({15,15});
	Coin* coin5 = new Coin({15,15});
	Coin* coin6 = new Coin({15,15});
	Coin* coin7 = new Coin({15,15});
	Coin* coin8 = new Coin({15,15});
	Coin* coin9 = new Coin({15,15});
	coinVec.push_back(coin1);
	coinVec.push_back(coin2);
	coinVec.push_back(coin3);
	coinVec.push_back(coin4);
	coinVec.push_back(coin5);
	coinVec.push_back(coin6);
	coinVec.push_back(coin7);
	coinVec.push_back(coin8);
	coinVec.push_back(coin9);
	coinVec[0]->setPos({350,400});
	coinVec[2]->setPos({350,550});
	coinVec[4]->setPos({490,470});
	coinVec[3]->setPos({640,400});
	coinVec[1]->setPos({640,550});
	coinVec[5]->setPos({490,310});
	coinVec[6]->setPos({490,620});
	coinVec[7]->setPos({770,550});
	coinVec[8]->setPos({210,550});

	//score counter stuff
	lblP1Score.setCharacterSize(35);
	lblP2Score.setCharacterSize(35);
	lblP1Score.setPosition({1000, 35});
	lblP2Score.setPosition({1000, 135});
	lblP1Score.setStyle(sf::Text::Bold);
	lblP2Score.setStyle(sf::Text::Bold);
	lblP1Score.setFillColor(Color::Green);
	lblP2Score.setFillColor(Color::Red);

	//Bonus aBonus = Bonus();


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
	MyFont = new Font();
	if (!MyFont->loadFromFile("fonts/font2.ttf"))
	{
		std::cerr << "Could not load font " << "fonts/font2.ttf" << std::endl;
		std::exit(1);
	}

	dashBoard.setFillColor(Color(125, 125, 125));
	dashBoard.setPosition(1000, 0);

	tank1Hp = BText("", Color::Green, BPoint(1000, 0),35);
	tank2Hp = BText("", Color::Green, BPoint(1000, 100), 35);
	gameOverText = BText("", Color::Yellow, BPoint(0, 0), 50);
	gameOverText.GoCenter(gameRect);
	
}

void Game::play() { // call this function to start playing
	gameWindow->requestFocus();
	clock.restart();
	
	
	while (gameWindow->isOpen())
	{
		if (!gameOver) {
			checkBullets();
			checkTanks();
			checkCoins();
			while (gameWindow->pollEvent(event))// listening events
			{
				if (event.type == Event::EventType::JoystickConnected)
					event.joystickConnect.joystickId = stickConnected++;
				if (event.type == Event::EventType::Closed)
					gameWindow->close();
				if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::TextEntered)
					KeyboardDown(event, true);
				if (event.type == sf::Event::EventType::JoystickMoved)
					KeyboardDown(event, false);
				if (event.type == sf::Event::EventType::KeyReleased || event.type == sf::Event::EventType::JoystickButtonReleased)//|| Event::EventType::JoystickButtonPressed)
					KeyboardReleased(event);
			}
		}
		else {
			if (boomPlayed) {
				update();
				//break;
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
		deadTank->getSpTank()->BSetTexture(*boomTextures[i]);
		sleep(milliseconds(150));
	}
	boomPlayed = true;
}



Game::~Game() {

	while (!boomPlayed) {
		sleep(milliseconds(10));
	}
	if (player2Tank)
		delete player2Tank;
	if (playerTank)
		delete playerTank;
	for (unsigned int i = 0; i < bulletList.size(); i++) {
		delete bulletList[i];
	}
	for (unsigned int i = 0; i < coinVec.size(); i++) {
		delete coinVec[i];
	}
	for (unsigned int i = 0; i < boomTextures.size()-1; i++) {
		if(boomTextures[i])
			delete boomTextures[i];
	}
	int a = 0;
	if (gameWindow)
		delete gameWindow;

}
