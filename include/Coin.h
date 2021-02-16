#pragma once

#include<SFML/Graphics.hpp>
#include"../include/BGraphic.h"

class Coin {
private:
  sf::RectangleShape coin;

public:
  Coin(sf::Vector2f size) {
    coin.setSize(size);
    coin.setFillColor(sf::Color::Yellow);
  }
  
  void drawTo(sf::RenderWindow &window) {
    window.draw(coin);
  }

  sf::FloatRect getGlobalBounds() {
    return coin.getGlobalBounds();
  }
  
  /* debugging function
  void changeFillColor() {
    coin.setFillColor(sf::Color::Green);
  }
  */

  //used to initially locate and "delete" coins (TEMP SOLUTION)
  void setPos(sf::Vector2f newPos) {
    coin.setPosition(newPos);
  }

};
