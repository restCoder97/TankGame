#include "Metal.h"
#include <vector>
#include "SFML/Graphics.hpp"

using namespace sf;

Metal::Metal(BPoint pt, Texture text) {
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(0, 0, 50, 50));
	position = pt;
	sprite.setPosition(pt);
}