#include "Game.h"

using namespace sf;
using namespace std;

BonusType setType(int time=0) {
	int nType = rand() % 7;
	int counter = 0;
	if (time <= 600) {//first 10 minutes game newclear is rare
		if (nType == 5) {
			nType = rand() % 7;
		}
	}
	return BonusType(nType);
}



void Game::GenerateBonus() {
	BonusType type = setType(int(gameClock.getElapsedTime().asSeconds()));
	Bonus*aBonus= new Bonus();
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
	int x = rand()% 950;
	aBonus->setCenterAt(BPoint(x, y));
	BonusList.push_back(aBonus);

}

void Game::CheckKeyboard() {// come to here if keyboard pressed
	if (gamePause||gameOver)
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


void Game::KeyboardDown(Event event, bool keyboard) {//come to here when keyboard button down or joystick moved;

	auto keyID = event.text.unicode;
	Keyboard::Key a = event.key.code;
	if (a == Keyboard::Key::Escape)
		gamePause = !gamePause;
	if (gamePause|| gameOver) {
		playerTank->stop();

		for(AITank* player2Tank: player2Tanks)
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
		playerTank->stop();
	if (joystick == Joystick::Z && player == 1 && axispt2 <= -5 && keyboard == false)
		playerTank->move();

	if (keyID == 0x16 || (player == 1 && joystick == Joystick::Y && pt == -100)) {
		playerTank->switchDirection(direction::top);
		if (player != 1)
			playerTank->move();
	}
	else if (keyID == 0x12 || (player == 1 && joystick == Joystick::Y && pt == 100)) {
		playerTank->switchDirection(direction::bot);
		if (player != 1)
			playerTank->move();
	}
	else if ((keyID == 0 && keyboard) || (player == 1 && joystick == Joystick::X && pt == -100)) {
		playerTank->switchDirection(direction::left);
		if (player != 1)
			playerTank->move();
	}
	else if (keyID == 0x3 || (player == 1 && joystick == Joystick::X && pt == 100)) {
		playerTank->switchDirection(direction::right);
		if (player != 1)
			playerTank->move();
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
	if (event.joystickButton.joystickId == 1 && event.joystickButton.button == 2) {
		playerTank->addBullet((unsigned int)(5));
		playerTank->setScore((unsigned int)(5));
	}

	if (event.joystickButton.joystickId == 0 && event.joystickButton.button == 2) {
		playerTank->addBullet((unsigned int)(playerTank->getScore() / 5));
		playerTank->setScore((unsigned int)(playerTank->getScore() % 5));
	}


	if (aKey == Keyboard::LShift) {
		playerTank->addBullet((unsigned int)(playerTank->getScore() / 5));
		playerTank->setScore((unsigned int)(playerTank->getScore() % 5));
	}

	if (aKey == Keyboard::RShift) {
		playerTank->addBullet((unsigned int)(playerTank->getScore() / 5));
		playerTank->setScore((unsigned int)(playerTank->getScore() % 5));
	}


	if (event.joystickButton.joystickId == 1 && event.joystickButton.button == 1)
		playerTank->fire(bulletList);
	if (event.joystickButton.joystickId == 0 && event.joystickButton.button == 1)
		playerTank->fire(bulletList);

	else if (aKey == sf::Keyboard::W || aKey == sf::Keyboard::S || aKey == sf::Keyboard::A || aKey == sf::Keyboard::D) {
		playerTank->stop();
		CheckKeyboard();
	}
	else if (aKey == sf::Keyboard::Up || aKey == sf::Keyboard::Down || aKey == sf::Keyboard::Left || aKey == sf::Keyboard::Right) {
		playerTank->stop();
		CheckKeyboard();
	}
	if (aKey == sf::Keyboard::Space) {
		playerTank->fire(bulletList);
	}
	if (aKey == sf::Keyboard::RControl) {
		playerTank->fire(bulletList);
	}
}


void Game::checkBullets() {// traverse bullet list, check bullets' position, if out of screen then delete it.
	Bullet* tmp;
	for (unsigned int i = 0; i < bulletList.size(); i++) {
		tmp = bulletList[i];
		BPoint bulletPeak = tmp->getPeekPoint();
		if (bulletPeak.x > 1000 || bulletPeak.y >= 1000 || bulletPeak.x <= 0 || bulletPeak.y <= 0) {
			//Bullet * tmp = bulletList[i];
			bulletList.erase(bulletList.begin() + i);
			//delete tmp;
		}
		else if (playerTank->isContainItems(bulletPeak)) {
			//Bullet * tmp = bulletList[i];
			bulletList.erase(bulletList.begin() + i);
			playerTank->damaged(tmp->nDamage);
			//player2Tank->addScore(15);
			//delete tmp;
		}
		else if (player2Tanks.size()) {
			for(AITank* player2Tank: player2Tanks){
				//Bullet * tmp = bulletList[i];
				if(player2Tank->isContainItems(bulletPeak)){
					bulletList.erase(bulletList.begin() + i);
					player2Tank->damaged(tmp->nDamage);
					playerTank->addScore(15);
					delete tmp;

				}
			}
		}
		else {
			for (int j = 0; j < gameMap->vBricks.size(); j++) {
				if (gameMap->vBricks[j]->sprite.isContaining(bulletPeak)) {
					GIF*aGif = new GIF(strExplosion, gameMap->vBricks[j]->sprite.getCenterPoint());
					boomGifs.push_back(aGif);
					//Bullet*tmp = bulletList[i];
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
						//Bullet*tmp = bulletList[i];
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
	for(AITank* player2Tank : player2Tanks)
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
	//gameWindow->draw(tank2Hp.getText());
	//coin draw
	/*
	if (player2Tank->storedBonus.size() != 0) {
		int a = player2Tank->storedBonus.top()->getId();
		String b = to_string(a);
		BText tx = BText(b, Color::Red, BPoint(100, 100), MyFont);
		gameWindow->draw(tx.getText());
	}
	*/

	for (Bonus*tmp : BonusList) {
		gameWindow->draw(tmp->getSprite());
	}


	//score counter draw
	gameWindow->draw(lblP1Score.getText());
	//gameWindow->draw(lblP2Score.getText());
	gameWindow->draw(tank1Bullet.getText());
	//gameWindow->draw(tank2Bullet.getText());



	if (gamePause&&gameOver == false) {
		gameWindow->draw(pauseView);
		gameWindow->draw(pauseText.getText());
	}
	if (gameOver) {// if game end, draw text
		gameWindow->draw(pauseView);
		gameWindow->draw(gameOverText.getText());
	}
	gameWindow->display();
	//aEdit.setFocus(gameWindow);
}


void Game::checkTanks() {// check tank for collision or running out of screen;
	if (playerTank->getHp() <= 0) {
		deadTank = playerTank;
		gameOverText.setString("Red Tank Win");
		gameOverText.setFillColor(Color::Red);
		gameOver = true;
		while (!flyThreadEnd) {
			sleep(milliseconds(10));
		}
	}


	else if (player2Tanks.size() == 0) {
		//deadTank = player2Tank;
		gameOverText.setString("Green Tank Win");
		gameOverText.setFillColor(Color::Green);
		gameOver = true;
		while (!flyThreadEnd) {
			sleep(milliseconds(10));
		}
	}

	for(AITank* player2Tank: player2Tanks){
		vector<BPoint>tmp2 = player2Tank->getFrontPoints();
		if(playerTank->isContainItems(player2Tank->getPtMouth(),&tmp2)||player2Tank->outOfScreen())
		{
			player2Tank->think(true);
		}
	}

	vector<BPoint>tmp1 = playerTank->getFrontPoints();
	for(AITank* player2Tank: player2Tanks){
		if (player2Tank->isContainItems(playerTank->getPtMouth(),&tmp1) || playerTank->outOfScreen()){
			playerTank->stop(true);
		}
	}

	for (int i = 0; i < gameMap->vBricks.size(); i++) {
		if (gameMap->vBricks[i]->sprite.isContaining(playerTank->getPtMouth(),&tmp1))
			playerTank->stop(true);


		for(AITank* player2Tank: player2Tanks){
			vector<BPoint>tmp2 = player2Tank->getFrontPoints();
			if (gameMap->vBricks[i]->sprite.isContaining(player2Tank->getPtMouth(), &tmp2))
				player2Tank->think(true);
			}
	}



	for (int i = 0; i < gameMap->vMetals.size(); i++) {
		if (gameMap->vMetals[i]->sprite.isContaining(playerTank->getPtMouth(), &tmp1))
			playerTank->stop(true);
		for(AITank* player2Tank: player2Tanks){
			vector<BPoint>tmp2 = player2Tank->getFrontPoints();
			if (gameMap->vMetals[i]->sprite.isContaining(player2Tank->getPtMouth(), &tmp2))
				player2Tank->think(true);
		}
	}
	for (int i = 0; i < BonusList.size(); i++) {
		if (BonusList[i]->isContaining(playerTank->getPtMouth(), &tmp1)) {
			playerTank->eat(BonusList[i]);
			BonusList.erase(BonusList.begin() + i);
		}

	}
	for (int i = 0; i < BonusList.size();i++) {
		for(AITank* player2Tank: player2Tanks){
			if (BonusList[i]->isContaining(player2Tank->getPtMouth(), &tmp1)) {
				player2Tank->eat(BonusList[i]);
				BonusList.erase(BonusList.begin() + i);
			}
		}
	}
	//player2Tank->checkingBonus();
	playerTank->checkingBonus();

	if (gameOver) {
		playerTank->stop();
		for(AITank* player2Tank: player2Tanks){
			player2Tank->setGameOver();
			player2Tank->stop();
		}
		playerTank->setGameOver();
		animateThread = new std::thread(&Game::playBoom, this);
	}

	std::ostringstream ssScoreP1, ssScoreP2;
	ssScoreP1.str(""); //refresh score label
	ssScoreP2.str("");
	ssScoreP1 << " Pts: " << playerTank->getScore();
	//ssScoreP2 << " Pts: " << player2Tank->getScore();
	lblP1Score = BText(ssScoreP1.str(), Color::Green, BPoint(1000, 35),MyFont);
	//lblP2Score = BText(ssScoreP2.str(), Color::Red, BPoint(1000, 935),MyFont);


	std::ostringstream str1HP, str2Hp;
	std::string hp = " Hp: ";
	str1HP << hp << playerTank->getHp();
	//str2Hp << hp << player2Tank->getHp();
	tank1Hp.setString(str1HP.str());
	//tank2Hp.setString(str2Hp.str());

	std::ostringstream str1Bullet, str2Bullet;
	std::string Ammo = " Ammo: ";
	str1Bullet << Ammo << playerTank->getBulletAmount();
	//str2Bullet << Ammo << player2Tank->getBulletAmount();
	tank1Bullet = BText(str1Bullet.str(), Color::Green, BPoint(1000, 65), MyFont);
	//tank2Bullet = BText(str2Bullet.str(), Color::Red, BPoint(1000, 965), MyFont);

}



Game::Game() {
	gameWindow = new RenderWindow(VideoMode(1100, 1000), "Game");// init window
	flyThread = new std::thread(&Game::FLY, this);// init fly thread
	GIFThread = new std::thread(&Game::playExplosion, this);
	gameMap = new GameMap();
	playerTank = new Tank(BPoint(500, 900), direction::top, BSize(50, 50), Color(100, 255, 100, 255));//init player Tank color:r,g,b,a
	//player2Tank = new AITank(BPoint(500, 100), direction::bot, BSize(50, 50), Color(255, 100, 100, 255), playerTank,  gameMap, &bulletList);
	player2Tanks.push_back(new AITank(BPoint(500, 100), direction::bot, BSize(50, 50), Color(255, 100, 100, 255), playerTank,  gameMap, &bulletList));
	player2Tanks.push_back(new AITank(BPoint(100, 100), direction::bot, BSize(50, 50), Color(255, 100, 100, 255), playerTank,  gameMap, &bulletList));
	player2Tanks.push_back(new AITank(BPoint(500, 500), direction::bot, BSize(50, 50), Color(255, 100, 100, 255), playerTank,  gameMap, &bulletList));
	player2Tanks.push_back(new AITank(BPoint(800, 800), direction::bot, BSize(50, 50), Color(255, 100, 100, 255), playerTank,  gameMap, &bulletList));



	FloatRect gameRect = FloatRect(0, 0, 1000, 1000);
	pauseView= RectangleShape(Vector2f(1000, 1000));
	pauseView.setPosition(Vector2f(0, 0));
	pauseView.setFillColor(Color(0, 0, 0, 230));
	pauseText = BText("Paused",Color::Red, BPoint(450, 10),MyFont,50);
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

	tank1Hp = BText("", Color::Green, BPoint(1000, 0),MyFont,35);
	//tank2Hp = BText("", Color::Red, BPoint(1000, 900), MyFont,35);
	gameOverText = BText("", Color::Yellow, BPoint(0, 0), MyFont, 50);
	gameOverText.GoCenter(gameRect);

}

void Game::play() { // call this function to start playing
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
				if (event.type == Event::EventType::Closed)
					gameWindow->close();
				if (event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::TextEntered)
					KeyboardDown(event, true);
				if (event.type == sf::Event::EventType::JoystickMoved)
					KeyboardDown(event, false);
				if (event.type == sf::Event::EventType::KeyReleased || event.type == sf::Event::EventType::JoystickButtonReleased)//|| Event::EventType::JoystickButtonPressed)
					KeyboardReleased(event);
			}
			int time = int(gameClock.getElapsedTime().asSeconds());
			int r = rand();
			if ((time % 10) == r %10 && time !=0 && !(BonusList.size() >= 4)) {
				GenerateBonus();
			}

			//player2Tank->think();
			for(AITank* player2Tank: player2Tanks)
				player2Tank->think();

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


	for(AITank* player2Tank: player2Tanks)
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
