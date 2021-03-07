#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Brick.h"
#include "Metal.h"


using namespace std;

class GameMap
{
    sf::Vector2i playerPos;
    void setUpTiles();
	Texture bricksTexture;
	Texture metalTexture;


public:
    vector<Brick*> vBricks;
    vector<Metal*> vMetals;
    int gridLength;
    GameMap();
	~GameMap() {
		for (Brick*i : vBricks) {
			if(i)
				delete i;
		}
		for (Metal*i : vMetals) {
			if(i)
				delete i;
		}
	}
    void paint(sf::RenderWindow* window);
};
