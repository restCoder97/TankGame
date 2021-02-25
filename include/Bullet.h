#pragma once
#include "BGraphic.h"
const int BulletSpeed = 10;
const BSize bulletSize = BSize(20, 20);
class Bullet {
public:

	BSprite*spBullet;
	direction dirFly;
	Texture aTexture;
	int nDamage = 10;
	const std::string bulletPath = "images/bullet.png";
	Bullet(BPoint startPoint, direction dic,unsigned int damage = 10);
	BPoint getPeekPoint();
};
