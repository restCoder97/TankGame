#pragma once
#include <SFML/Graphics.hpp>
#include<vector>
#include <SFML/Audio.hpp>
#include<string>
#include<iostream>
using namespace sf;

// Bill Chen's Graphic library
enum direction { top, right, bot, left };

class BSize {
public:
	int width;
	int height;
	BSize() {
		width = 0;
		height = 0;
	}
	BSize(int w, int h) {
		width = w;
		height = h;
	}


};

class BSound: public Sound{

	SoundBuffer*sdb = nullptr;
public:
	BSound() = default;
	BSound(std::string path,bool loop = false) {
		sdb = new SoundBuffer();
		if (!sdb->loadFromFile(path)) {
			std::cerr << "Can't find sound file: " << path<< std::endl;
			std::exit(1);
		}
		setBuffer(*sdb);
		setLoop(loop);
		setVolume(100);
	}

	~BSound() {
		if (sdb)
			delete sdb;
	}


};


class BPoint {
public:
	float x;
	float y;
	BPoint(float nX, float nY) { x = nX; y = nY; };
	Vector2f getV2f() const { return Vector2f((float)x, float(y)); };
	BPoint(int nX, int nY) { x = float(nX); y = float(nY); }
	BPoint() {};
	BPoint operator+(const BPoint&b) {
		BPoint pt(0,0);
		pt.x = this->x + b.x;
		pt.y = this->y + b.y;
		return pt;
	}

	BPoint move(int mX, int mY) {// taking move amount, move the point and return the point after moving
		return BPoint(x + mX, y + mY);
	}
};


class BLine {
public:
	Vector2f initPoint;
	Vector2f endPoint;
	BLine(BPoint i, BPoint e) { initPoint = Vector2f(i.x, i.y); endPoint = Vector2f(e.x, e.y); };
	BLine() { initPoint = Vector2f(0, 0); endPoint = Vector2f(0, 0); };
};



// over-write Sprite function to suit our game
class BSprite :public Sprite {

	BSize spSize;// sprite size
	direction dic;//current direction
	Texture*BTexture;// texture
public:


	Texture* getBTexture() {
		return BTexture;
	}

	void setSize(BSize aSize) {
		spSize = aSize;
	}

	BSprite() = default;
	//construct a sprite with given texture, size, direction, center point
	BPoint getCenterPoint() {
		return (BGetPosition().move(spSize.width / 2, spSize.height / 2));
	}
	BSprite(Texture texture, BSize size, direction textureDic, BPoint ptCenterStart) :Sprite(texture) {

		spSize = size;
		BSetTexture(texture);
		setPosition(ptCenterStart);
		move(-size.width / 2, -size.height / 2);
		dic = textureDic;

		BTexture = new Texture(texture);
	}

	void BSetTexture(Texture texture) {
		setTexture(texture);
		float widthScale = (static_cast<float>(spSize.width) / texture.getSize().x);
		float heightScale = static_cast<float>(spSize.height) / texture.getSize().y;
		BTexture = new Texture(texture);
		setScale(widthScale, heightScale);
	}

	BPoint getPeakPoint() {// grabe vector graph's peak point ex: tank's muzzle, Bullet's warhead
		int x = int(getPosition().x);
		int y = int(getPosition().y);
		if (dic == bot)
			return BPoint(x - spSize.width / 2 + 2, y);
		else if (dic == top)
 			return BPoint(x + spSize.width / 2 - 2, y);
		else if (dic == 3)
			return BPoint(x, y - spSize.height / 2 + 2);
		else
			return BPoint(x, y + spSize.height / 2 - 2);
	}

	BPoint BGetPosition() {
		return BPoint(getPosition().x, getPosition().y);
	}
	//rotate around center point; take the new direction, compare with old direction and determine how many degree need rotate
	void BRotate(direction newDic) {// rotating around the center position

		if (dic == newDic) {
			dic = newDic;
			return;
		}

		else if (abs(dic - newDic) == 2)
		{
			rotate(180.0f);
			Vector2f a = getPosition();
			if (newDic == bot)
				move(spSize.width, spSize.height);
			else if (newDic == top)
				move(-spSize.width, -spSize.height);
			else if (newDic == 3)
				move(-spSize.width, spSize.height);
			else if (newDic == 1)
				move(spSize.width, -spSize.height);
		}

		else if (newDic - dic > 0 && newDic - dic != 3 || newDic - dic == -3)
		{
			rotate(90.0f);
			if (newDic == bot)
				move(0, +spSize.height);
			else if (newDic == top)
				move(0, -spSize.height);
			else if (newDic == 3)
				move(-spSize.width, 0);
			else if (newDic == 1)
				move(+spSize.width, 0);
		}

		else {
			rotate(-90.0f);
			if (newDic == bot)
				move(+spSize.width, 0);
			else if (newDic == top)
				move(-spSize.width, 0);
			else if (newDic == 3)
				move(0, +spSize.height);
			else if (newDic == 1)
				move(0, -spSize.height);
		}
		dic = newDic;
	}

	bool isContaining(const BPoint & pt, const std::vector<BPoint>*pts= nullptr) {
		if ( getGlobalBounds().contains(pt.getV2f()))
			return true;
		else if(pts){
			
			for (int i = 0; i < pts->size(); i++) {
				if (getGlobalBounds().contains(pts->at(i).getV2f()))
					return true;
			}
		}
		return false;
	}

	void setPosition(BPoint pt) {

		return Sprite::setPosition(pt.x,pt.y);
	}

	void move(int x, int y){//override move function
		Sprite::move(float(x), float(y));
	}

	~BSprite() {
		//Sprite::~Sprite();
		delete BTexture;
	}
};
