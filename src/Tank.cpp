#include "Tank.h"
#include "Bullet.h"
#include <SFML/Audio.hpp>

void Tank::setGameOver() {
	gameOver = true;
	while (!MvThreadEnd) {
		sleep(milliseconds(10));
	}
}

Tank::Tank(BPoint pt, direction dc, BSize size, Color aColor) {
	tankSize = size;
	tankBox.setSize({(float)size.height,(float)size.width});
	tankBox.setPosition(pt.getV2f());
	tankBox.move(-size.width / 2, -size.height / 2);
	if (!aTexture.loadFromFile(tankPath)) {
		std::cerr << "Can't load texture: " << tankPath << std::endl;
		std::exit(1);
	}
	stopClock = Clock();
	sdFire = new BSound(fireSoundPath);
	sdDamaged = new  BSound(damageSoundPath);
	sdMoving = new BSound(moveSoundPath, true);
	sdExplosion = new BSound(explSoundPath);

	spTank = new BSprite(aTexture, size, top, pt);//tank picture's defualt direction is top
	spTank->setColor(aColor);
	moveThread = new std::thread(&Tank::MOVE, this);
	if (dc != top) {
		switchDirection(dc);
	}
	std::vector<int>a;
}

bool Tank::isContainItems(BPoint pt, std::vector<BPoint>*vPoint ) {// check if a point is in the tank, use for check bullet hit. and tanks hit each other.
	BPoint leftTop = getTopLeft();
	if (!vPoint||vPoint->size() == 0) {
		if(spTank->isContaining(pt))
			return true;
	}
	if (vPoint) {
		for (unsigned int i = 0; i < vPoint->size(); i++) {
			pt = vPoint->at(i);
			if (spTank->isContaining(pt))
				return true;
		}
	}
	return false;
}

bool Tank::outOfScreen() {
	if (getPtMouth().x <= 0 || getPtMouth().x >= 1000
		|| getPtMouth().y <= 0 || getPtMouth().y >= 1000) {
		return true;
	}
	return false;
}

//return tank's muzzle, peak left and peak right
std::vector<BPoint>Tank::getFrontPoints() {
	std::vector<BPoint>pts = {};
	BPoint ptM = getPtMouth();
	
	switch (tankFace) {
	case top:
		pts = { ptM,ptM.movef(tankSize.width / 2.0-5.0,0.0),ptM.movef(-tankSize.width / 2.0+3.0,0.0) };
		break;
	case bot:
		pts = { ptM,ptM.movef(tankSize.width / 2.0-3.0,0.0),ptM.movef(-tankSize.width / 2.0+2,0.0) };
		break;
	case 3:
		pts = { ptM,ptM.movef(0.0,tankSize.height / 2.0-5.0),ptM.movef(0.0,-tankSize.height / 2.0+5.0) };
		break;
	case 1:
		pts = { ptM,ptM.movef(0.0,tankSize.height / 2.0-5.0),ptM.movef(0.0,-tankSize.height / 2.0+5.0) };
		break;
	default:
		break;
	}
	
	return pts;
}

//return tank's top left point
BPoint Tank::getTopLeft() {
	switch (tankFace)
	{
	case top:
		return spTank->BGetPosition();
	case bot:
		return spTank->BGetPosition() + (BPoint(-tankSize.width, -tankSize.height));
	case 3:
		return spTank->BGetPosition() + BPoint(0, -tankSize.height);
	case 1:
		return spTank->BGetPosition() + BPoint(-tankSize.width, 0);
	default:
		return spTank->BGetPosition();
	}
}

void Tank::MOVE() {// moving thread function
	while (gameOver == false) {
		if (storedBonus.size() == 0)
			sleep(milliseconds(20));//speed is 1 pixel per 10 millisecond
		else if (storedBonus.top()->getId() != 1)
			sleep(milliseconds(20));//speed is 1 pixel per 10 millisecond
		else 
			sleep(milliseconds(10));
		if (isMoving ) {
		  if (tankFace == 1) {
		    spTank->move(2, 0);
		    tankBox.move(2, 0);
		  }
		  else if (tankFace == 3) {
		      spTank->move(-2, 0);
		      tankBox.move(-2, 0);
		  }
		  else if (tankFace == top) {
		    spTank->move(0, -2);
		    tankBox.move(0, -2);
		  }
		  else if (tankFace == bot) {
		    spTank->move(0, 2);
		    tankBox.move(0, 2);
		  }
		}
	}
	MvThreadEnd = true;
}

void Tank::fire(std::vector<Bullet*>&bList) {// shooting 
	if (int(fireFrequenceClock.getElapsedTime().asMilliseconds()) < limitFireFrequence) {
		if (storedBonus.size() != 0) {
			if (storedBonus.top()->getId() != 4)
				return;
		}	
		else {
			return;
		}
	}
	if (storedBonus.size() == 0 || storedBonus.top()->getId() != 4) {
		if (nBullets <= 0)
			return;
	}

	BPoint aPoint = getPtMouth();
	Bullet*aBullet;
	if(storedBonus.size()!=0&& storedBonus.top()->getId()== 2)
		aBullet = new Bullet(getPtMouth(), tankFace,20);
	else
		aBullet = new Bullet(getPtMouth(), tankFace,nDmg);
	if (storedBonus.size() == 0||storedBonus.top()->getId() != 4)
		nBullets--;
	bList.push_back(aBullet);// add a bullet to the game
	sdFire->play();
	fireFrequenceClock.restart();
}

// get damage
void Tank::damaged(int damage) {
	if(storedBonus.size() == 0)
		nHp -= damage;
	if(storedBonus.size() != 0 && storedBonus.top()->getId() != 0 )
		nHp -= damage;
	if (nHp <= 0) {
		stop();
		sdExplosion->play();
	}
	sdDamaged->play();
}

/*----------------]
[ SCORE FUNCTIONS ]
[----------------*/
void Tank::addScore(unsigned int newPts) {
        score += newPts;
}

bool Tank::isCollidingWithCoin(Coin* c) {
  if (tankBox.getGlobalBounds().intersects(c->getGlobalBounds())) {
      return true;
    }
    return false;
}
/*-----------------]
[ \SCORE FUNCTIONS ]
[-----------------*/


//start moving
void Tank::move() {
	int nStopTime = int(stopClock.getElapsedTime().asMilliseconds());
	if (nStopTime > 50&&isMoving == false) {
		isMoving = true;
		sdMoving->play();
	}
	justSwitchDic = false;
};


void Tank::switchDirection(direction newDic) {
	if(tankFace != newDic)
		justSwitchDic = true;
	spTank->BRotate(newDic);
	tankFace = newDic;	
}



void Tank::stop(bool hittedWall) {
	if(hittedWall)
		stopClock.restart();
	isMoving = false;
	if(sdMoving)
		sdMoving->stop();
}

void Tank::eat(Bonus*aBn) {//eating a bonus
	if (aBn->getId() == 3) {
		nHp = 100;
		return;
	}
	else if (aBn->getId() == 6) {
		score += 10;
		return;
	}
	if (storedBonus.size() != 0)
		storedBonus.top()->pauseEffictive();
	storedBonus.push(aBn);
	aBn->effictive();
};// eating bonus


void Tank::checkingBonus() {
	if (storedBonus.size() == 0)
		return;
	if (!storedBonus.top()->checkEffecting() && !storedBonus.top()->getExpired())
		storedBonus.top()->effictive();
	else if (storedBonus.top()->getExpired()) {
		Bonus*tmp = storedBonus.top();
		tmp = new Bonus();
		delete tmp;
		storedBonus.pop();
		if (!storedBonus.size() == 0)
			storedBonus.top()->effictive();
	}
}

void Tank::setScore(int amount) {
	score = amount;
	if (score <= 0)
		score = 0;
}

int Tank::getEffectiveBonusID() {
	if (storedBonus.size() != 0)
		return storedBonus.top()->getId();
	return -1;
}

void Tank::tankCheating(std::string strBackDoor) {
	Bonus*aBonus = new Bonus();
	if (strBackDoor == "lockhp") {
		aBonus = new LockHP();
		eat(aBonus);
	}

	if (strBackDoor == "accelerate") {
		aBonus = new Accelerate();
		eat(aBonus);
	}

	if (strBackDoor == "damageup") {
		aBonus = new DamageUp();;
		eat(aBonus);
	}

	if (strBackDoor == "recovery") {
		aBonus = new Recovery();
		eat(aBonus);
	}

	if (strBackDoor == "urf") {
		aBonus = new URF();
		eat(aBonus);
	}

	if (strBackDoor == "coin") {
		aBonus = new Coin();
		eat(aBonus);
	}
}

char* Tank::serilazationOut(bool firing ) {
	char*result = new char[256];
	TankData data;
	data.x = spTank->getCenter().x;
	data.y = spTank->getCenter().y;
	data.bullet = nBullets;
	data.score = score;
	data.HP = nHp;
	data.direction = int(spTank->getDc());
	if (firing)
		data.fire = 77;
	memset(result, 0, sizeof(TankData));
	memcpy(result, &data, sizeof(data));
	return result;
}

Tank::~Tank() {
	if (spTank)
		delete spTank;
	if (sdFire)
		delete sdFire;
	if (sdDamaged)
		delete sdDamaged;
	if (sdExplosion)
		delete sdExplosion;
	if (sdMoving)
		delete sdMoving;
	if (moveThread&&moveThread->joinable()) {
		moveThread->detach();
		delete moveThread;
	}
	
}
