#pragma once

#include "SFML/Graphics.hpp"
#include <string>
#include"BGraphic.h"

class Metal
{
public:
    sf::Vector2f pos;
    sf::Texture texture;
    BSprite sprite;
	BPoint position;
    const bool isPassable = false;
    const bool isDestroyable = false;
	~Metal() {};
	Metal(BPoint, Texture);
};

