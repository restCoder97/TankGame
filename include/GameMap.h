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
    //vector<vector<int>> potField; //which grid is brick(1), metal(2)
    vector<Brick*> vBricks;
    vector<Metal*> vMetals;
    int gridLength;
    GameMap();
    void paint(sf::RenderWindow* window);
};
