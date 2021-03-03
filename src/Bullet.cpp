#include "Bullet.h"
Bullet::Bullet(BPoint startPoint, direction dic,unsigned damage) {
	if (!aTexture.loadFromFile(bulletPath)) {
		int a = 0;
	}
	spBullet = new BSprite(aTexture, bulletSize, top, startPoint);
	dirFly = dic;
	Vector2f tmp = spBullet->getPosition();
	spBullet->BRotate(dic);
	tmp = spBullet->getPosition();
	nDamage = damage;
}

BPoint Bullet::getPeekPoint() {
	return spBullet->getPeakPoint();
}