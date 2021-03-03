#include "Brick.h"
#include <vector>
#include "SFML/Graphics.hpp"

using namespace sf;

Brick::Brick(BPoint pt, Texture texture) {
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setSize(BSize(50, 50));
	sprite.setTextureRect(IntRect(0, 0, 50, 50));
	sprite.setPosition(pt);
}