#pragma once

#include <SFML/Graphics.hpp>
#include "GameTile.h"
#include <vector>
#include "Brick.h"
#include "Metal.h"


using namespace std;

class GameMap
{
    sf::Vector2i playerPos;
    void setUpTiles();

public:
    vector<Brick*> vBricks;
    vector<Metal*> vMetals;
    int gridLength;
    GameMap();
    void paint(sf::RenderWindow* window);
};
