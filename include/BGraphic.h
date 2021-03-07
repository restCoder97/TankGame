#pragma once
#pragma warning(disable : 4996)
#include <SFML/Graphics.hpp>
#include<vector>
#include <SFML/Audio.hpp>
#include<string>
#include<iostream>
#include<cstring>
using namespace sf;

// Bill Chen's Graphic library

enum direction { top, right, bot, left };
enum Align { topLeft, topRight, botLeft, botRight, centerLeft, centerRight, centerTop, centerBot, center, null };


class BSize {
public:
	float width;
	float height;
	BSize();
	BSize(int w, int h);


};

class BSound : public Sound {

	SoundBuffer*sdb = nullptr;
public:
	BSound() = default;
	BSound(std::string path, bool loop = false);
	~BSound();


};


class BPoint {
public:
	float x;
	float y;
	BPoint(float fX, float fY) { x = fX; y = fY; };
	BPoint(float fX, int nY) { x = fX; y = float(nY); };
	BPoint(int nX, float fY) { x = float(nX); y = fY; };
	Vector2f getV2f() const { return Vector2f((float)x, float(y)); };
	BPoint(int nX, int nY) { x = float(nX); y = float(nY); }
	BPoint() {};

	BPoint operator+(const BPoint&b) {
		BPoint pt(0, 0);
		pt.x = this->x + b.x;
		pt.y = this->y + b.y;
		return pt;
	}

	BPoint move(int mX, int mY);
	BPoint movef(double  mX, double mY);

};






// over-write Sprite function to suit our game
class BSprite :public Sprite 
{

	BSize spSize;// sprite size
	direction dic;//current direction
	Texture*BTexture = nullptr;// texture
	Mutex mMutex;

public:

	BSprite() = default;
	Texture* getBTexture();
	void setSize(BSize aSize);
	direction getDc();
	//construct a sprite with given texture, size, direction, center point
	BPoint getCenterPoint();
	BSprite(Texture texture, BSize size, direction textureDic, BPoint ptCenterStart) ;
	void BSetTexture(Texture&texture);
	BPoint getPeakPoint();
	BPoint getCenter();
	//rotate around center point; take the new direction, compare with old direction and determine how many degree need rotate
	void BRotate(direction newDic);
	bool isContaining(const BPoint & pt, const std::vector<BPoint>*pts = nullptr);
	void setPosition(BPoint pt);
	BPoint BGetPosition();
	void setCenterAt(BPoint pt);
	Sprite getSprite();
	~BSprite();
};






class BText :public Text {
	Font myFont;
	Align mAlign = Align::null;
	FloatRect bdRect;
public:
	BText() {};
	BText(const String & string, Color aColor, BPoint pt = BPoint(0, 0), Font*ft = nullptr, unsigned
		int characterSize = 30, const std::string strFileName = "fonts//font2.ttf");
	void setAlignment(FloatRect boundRect, Align align);
	void GoCenter(FloatRect boundRect);
	Text& getText();
	
};


struct TankData {
	float x;
	float y;
	int direction;
	int fire = 0;
	int score;
	int bullet;
	int HP;
};



class GIF {
public:

	int indexOfPic = 0;
	BSprite*sp = nullptr;
	std::vector<Texture*>txs;
	Mutex mMutex;

	GIF(std::vector<std::string>fileName, BPoint pt);
	bool playNext();
};

