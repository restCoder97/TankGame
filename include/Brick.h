#pragma once

#include "SFML/Graphics.hpp"
#include <string>
#include"BGraphic.h"
class Brick
{
public:
    sf::Vector2f pos;
    sf::Texture texture;
    BSprite sprite;
	BPoint postion;

    const bool isPassable = false;
    const bool isDestroyable = true;

    Brick(BPoint pt,Texture);
};

