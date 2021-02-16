#include "Tank.h"
#include "Bullet.h"
#include "Coin.h"

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
	//moveThread->join();
	if (dc != top) {
		switchDirection(dc);
	}
	std::vector<int>a;
	
	//animatedThread = std::thread(&Tank::playAnimation,this);
}

bool Tank::isContainItems(BPoint pt, std::vector<BPoint>*vPoint ) {// check if a point is in the tank, use for check bullet hit. and tanks hit each other.
	BPoint leftTop = getTopLeft();
	if (!vPoint||vPoint->size() == 0) {
		//if (pt.x <= (leftTop.x + tankSize.width) && pt.x >= leftTop.x && pt.y <= leftTop.y + tankSize.height && pt.y >= leftTop.y)
		if(spTank->isContaining(pt))
			return true;
	}
	if (vPoint) {
		for (unsigned int i = 0; i < vPoint->size(); i++) {
			pt = vPoint->at(i);
			//if (pt.x <= (leftTop.x + tankSize.width) && pt.x >= leftTop.x && pt.y <= leftTop.y + tankSize.height && pt.y >= leftTop.y)
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
		pts = { ptM.move(tankSize.width / 2-3,0),ptM.move(-tankSize.width / 2+3,0) };
		break;
	case bot:
		pts = { ptM.move(tankSize.width / 2-3,0),ptM.move(-tankSize.width / 2+2,0) };
		break;
	case 3:
		pts = { ptM.move(0,tankSize.height / 2-5),ptM.move(0,-tankSize.height / 2+2) };
		break;
	case 1:
		pts = { ptM.move(0,tankSize.height / 2-2),ptM.move(0,-tankSize.height / 2+2) };
		break;
	default:
		break;
	}
	pts.push_back(ptM);
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
		sleep(milliseconds(7));//speed is 1 pixel per 10 millisecond
		if (isMoving) {
		  if (tankFace == 1) {
		    spTank->move(1, 0);
		    tankBox.move(1, 0);
		  }
		  else if (tankFace == 3) {
		      spTank->move(-1, 0);
		      tankBox.move(-1, 0);
		  }
		  else if (tankFace == top) {
		    spTank->move(0, -1);
		    tankBox.move(0, -1);
		  }
		  else if (tankFace == bot) {
		    spTank->move(0, 1);
		    tankBox.move(0, 1);
		  }
		}
	}
	MvThreadEnd = true;
}

void Tank::fire(std::vector<Bullet*>&bList) {// shooting 
	if (int(fireFrequenceClock.getElapsedTime().asMilliseconds()) < limitFireFrequence)
		return;
	BPoint aPoint = getPtMouth();
	Bullet*aBullet = new Bullet(getPtMouth(), tankFace);
	bList.push_back(aBullet);// add a bullet to the game
	sdFire->play();
	fireFrequenceClock.restart();
}

// get damage
void Tank::damaged(int damage) {
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

void eat() {};// eating bonus 

//start moving
void Tank::move() {
	int nStopTime = int(stopClock.getElapsedTime().asMilliseconds());
	if (isMoving == false ) {
		sdMoving->play();
	}
	if (nStopTime > 50||justSwitchDic) {
		isMoving = true;
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
	sdMoving->stop();
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
}
