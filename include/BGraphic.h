#pragma once
#include <SFML/Graphics.hpp>
#include<vector>
#include <SFML/Audio.hpp>
#include<string>
#include<iostream>
using namespace sf;

// Bill Chen's Graphic library
enum direction { top, right, bot, left };
enum Align {topLeft,topRight, botLeft,botRight, centerLeft,centerRight,centerTop,centerBot,center,null};

class BSize {
public:
	float width;
	float height;
	BSize() {
		width = 0;
		height = 0;
	}
	BSize(int w, int h) {
		width = float(w);
		height = float(h);
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
	BPoint(float fX, float fY) { x = fX; y = fY; };
	BPoint(float fX, int nY) { x = fX; y = nY; };
	BPoint(int nX, float fY) { x = nX; y = fY; };
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

	direction dic;//current direction
	Texture*BTexture = nullptr;// texture
	Mutex mMutex;

public:

	BSize spSize;// sprite size

	Texture* getBTexture() {
		return BTexture;
	}

	void setSize(BSize aSize) {
		spSize = aSize;
	}

	BSprite() = default;
	//construct a sprite with given texture, size, direction, center point
	BPoint getCenterPoint() {
		return (BGetPosition().move(int(spSize.width / 2), int(spSize.height / 2)));
	}
	BSprite(Texture texture, BSize size, direction textureDic, BPoint ptCenterStart) :Sprite(texture) {

		spSize = size;
		BSetTexture(texture);
		setPosition(ptCenterStart);
		move(-size.width / 2, -size.height / 2);
		dic = textureDic;

		BTexture = new Texture(texture);
	}


	void BSetTexture(Texture&texture) {
		mMutex.lock();
		setTexture(texture);
		float widthScale = (static_cast<float>(spSize.width) / texture.getSize().x);
		float heightScale = static_cast<float>(spSize.height) / texture.getSize().y;
		BTexture = &texture;
		setScale(widthScale, heightScale);
		mMutex.unlock();
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
		Sprite::setPosition(pt.x,pt.y);
	}

	void setCenterAt(BPoint pt) {
		Sprite::setPosition(pt.x - spSize.width / 2, pt.y - spSize.height / 2);
	}

	void move(int x, int y){//override move function
		Sprite::move(float(x), float(y));
	}
	Sprite getSprite() {
		Sprite tmp;
		tmp = *dynamic_cast<Sprite*>(this);
		tmp.setTexture(*BTexture);
		return tmp;
	}
	~BSprite() {
		//Sprite::~Sprite();
		if(BTexture)
			delete BTexture;
	}
};






class BText :public Text {
	Font*myFont;
	Align mAlign = null;
	FloatRect bdRect;
public:
	BText(const String & string, Color aColor, BPoint pt = BPoint(0, 0), Font*ft = nullptr, unsigned int characterSize = 30,const std::string strFileName = "fonts//font2.ttf"
		):Text(string,Font(),characterSize) {
		if (ft)
			myFont = ft;
		else {
			myFont = new Font();
			if (!myFont->loadFromFile(strFileName)) {
				std::cerr << "Can't open font file" << strFileName << std::endl;
				throw "Can't open font file";
				return;
			}
		}
		setStyle(sf::Text::Bold);
		setFillColor(aColor);
		setPosition(pt.getV2f());
	}
	void setAlignment(FloatRect boundRect,Align align) {
		setFont(*myFont);
		FloatRect CGRect = Text::getLocalBounds();
		float width = CGRect.width;
		float height = CGRect.height;
		switch (align)
		{
		case center:
			GoCenter(boundRect);
			break;
		case topLeft:
			setPosition(Vector2f(boundRect.left,boundRect.top));
			break;
		case topRight:
			setPosition(Vector2f(boundRect.width-width, boundRect.top));
			break;
		case botLeft:
			setPosition(Vector2f(boundRect.left, boundRect.height-height));
			break;
		case botRight:
			setPosition(Vector2f(boundRect.width-width, boundRect.top-height));
			break;
		case centerLeft:
			GoCenter(boundRect);
			move(-boundRect.width / 2 + width / 2, 0);
			break;
		case centerRight:
			GoCenter(boundRect);
			move(boundRect.width / 2 - width / 2, 0);
			break;
		case centerTop:
			GoCenter(boundRect);
			move(0, -boundRect.height / 2 + height / 2);
			break;
		case centerBot:
			GoCenter(boundRect);
			move(0, +boundRect.height / 2 - height / 2);
			break;

		default:
			break;
		}
		mAlign = align;
		bdRect = boundRect;
	};

	void GoCenter(FloatRect boundRect) {
		setFont(*myFont);
		FloatRect tmp = Text::getLocalBounds();
		Vector2f a = Vector2f(boundRect.left+float(boundRect.width/ 2), boundRect.top+float(boundRect.height / 2));
		setPosition(a);
		move(-tmp.width /2,  -tmp.height / 2);
		bdRect = boundRect;
		mAlign = center;
	};

	BText() {};
	Text getText() {
		setAlignment(bdRect,mAlign);
		Text tmp = *dynamic_cast<Text*>(this);
		tmp.setFont(*myFont);
		return tmp;
	}


};

class BLineEdit:public RectangleShape {

	Color colorText;
	Color colorBackground;
	bool isFocusing=false;
	Event event;
	std::string strText;
	BSize selfSize;
public:
	BText text;
	BLineEdit(BPoint pt, std::string str = "" , BSize size = BSize(200, 30),
		Color textColor = Color::Black, Color backgroundColor = Color::White) :RectangleShape(Vector2f(size.width,size.height)){
		selfSize = size;
		setPosition(pt.getV2f());
		colorBackground = backgroundColor;
		colorText = textColor;
		strText = str;
		text = BText(str, textColor, pt, nullptr, 20 );
		FloatRect tmp = FloatRect(pt.x, pt.y, size.width, size.height);
		text.setAlignment(tmp, centerLeft);
	}

	void setBackgroundColor(Color aColor) {
		colorBackground = aColor;
		setFillColor(aColor);
	}
	void setTextColor(Color aColor) {
		colorText = aColor;
		text.setFillColor(aColor);
	}

	void setFocus(Window*w) {
		isFocusing = true;
		while (w->isOpen()&&isFocusing)
		{
			while (w->pollEvent(event))// listening events
			{
				if (event.type == sf::Event::EventType::KeyPressed) {
					char c = getKey();
					strText+=c;
					text.setString(strText);
					FloatRect tmp = FloatRect(this->getPosition().x, this->getPosition().y, selfSize.width, selfSize.height);
					text.GoCenter(tmp);
				}
			}
		}
	}


	//Credit prof Bently
	char getKey()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			return  'a';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
			return  'b';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
			return  'c';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			return  'd';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			return  'e';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			return  'f';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
			return  'g';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
			return  'h';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
			return  'i';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
			return  'j';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
			return  'k';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			return  'l';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
			return  'm';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
			return  'n';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			return  'o';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			return  'p';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			return  'q';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			return  'r';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			return  's';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
			return  't';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
			return  'u';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
			return  'v';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			return  'w';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			return  'x';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
			return  'y';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			return  'z';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			return  ' ';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			return  '\n';
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
			return  '\b';
		return ' ';
	}


};
