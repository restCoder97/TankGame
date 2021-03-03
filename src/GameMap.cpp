#include "GameMap.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Brick.h"
#include "Metal.h"
#include <cstdlib>
#include <cstring>
#include"BGraphic.h"
const string strBrick = "images/brickes.png";
const string strMetal = "images/metal.png";
void GameMap::setUpTiles()
{
	fstream brickPos = fstream("binFile/brickPos.bin", ios::binary | ios::in);
	fstream metalPos = fstream("binFile/metalPos.bin", ios::binary | ios::in);
	if (!brickPos.is_open() || !metalPos.is_open() || !bricksTexture.loadFromFile(strBrick)||!metalTexture.loadFromFile(strMetal)) {
		cerr << "Unable to Open map file" << endl;
		exit(1);
	}

    while (!brickPos.eof()) {
        int x;
        int y;
        brickPos.read(reinterpret_cast<char*>(&y), sizeof(int));
        brickPos.read(reinterpret_cast<char*>(&x), sizeof(int));
		BPoint pt  = BPoint(x, y);
        Brick* aBrick = new Brick(pt,bricksTexture);
        vBricks.push_back(aBrick);
    }

    while (!metalPos.eof()) {
        int x;
        int y;
        metalPos.read(reinterpret_cast<char*>(&y), sizeof(int));
        metalPos.read(reinterpret_cast<char*>(&x), sizeof(int));
		BPoint pt = BPoint(x, y);
        Metal* aMetal = new Metal(pt,metalTexture);
        vMetals.push_back(aMetal);
    }
}

GameMap::GameMap(){
    gridLength = 20;
    setUpTiles();
}


void GameMap::paint(sf::RenderWindow* window){
	for (auto i = 0; i < vBricks.size(); ++i) {
		vBricks[i]->sprite.setTexture(bricksTexture);
		window->draw(*dynamic_cast<sf::Sprite*>(&vBricks[i]->sprite));
	}
        
	for (auto i = 0; i < vMetals.size(); ++i) {
		vMetals[i]->sprite.setTexture(metalTexture);
		window->draw(*dynamic_cast<sf::Sprite*>(&vMetals[i]->sprite));
	}
        
}
