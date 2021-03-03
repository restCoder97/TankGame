#pragma once
#include "BGraphic.h"
const int BulletSpeed = 5;
const BSize bulletSize = BSize(20, 20);
class Bullet {
public:

	BSprite*spBullet;
	direction dirFly;
	Texture aTexture;
	int nDamage = 30;
	const std::string bulletPath = "bullet.png";

	Bullet(BPoint startPoint, direction dic) {
		if (aTexture.loadFromFile(bulletPath)) {
			int a = 0;
		}
		spBullet = new BSprite(aTexture, bulletSize,top, startPoint);
		dirFly = dic;
		Vector2f tmp = spBullet->getPosition();
		spBullet->BRotate(dic);
		tmp = spBullet->getPosition();
		int a = 0;
	}

	BPoint getPeekPoint() {
		
		return spBullet->getPeakPoint();
	}
};