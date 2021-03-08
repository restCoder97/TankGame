#include "Bonus.h"


void Bonus::setImage(Texture&aText) {
	setTextureRect(IntRect(0, 0, 50, 50));
	BSetTexture(aText);
}

void Bonus::effictive() {
	effectiveClock.restart();
	effecting = true;
}
void Bonus::pauseEffictive() {
	effectiveTime -= int(effectiveClock.getElapsedTime().asSeconds());
	effecting = false;
}

bool Bonus::checkEffecting() {
	int nT = int(effectiveClock.getElapsedTime().asSeconds());
	if (nT < effectiveTime)
		effecting = true;
	else {
		expired = true;
		effecting = false;
	}
	return effecting;
}

