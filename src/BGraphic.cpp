#include "BGraphic.h"
BSize::BSize() {
	width = 0;
	height = 0;
}
BSize::BSize(int w, int h) {
	width = float(w);
	height = float(h);
}


BSound::BSound(std::string path, bool loop) {
	sdb = new SoundBuffer();
	if (!sdb->loadFromFile(path)) {
		std::cerr << "Can't find sound file: " << path << std::endl;
		std::exit(1);
	}
	setBuffer(*sdb);
	setLoop(loop);
	setVolume(100);
}

BSound::~BSound() {
	if (sdb)
		delete sdb;
}


BPoint BPoint::move(int mX, int mY) {// taking move amount, move the point and return the point after moving
	x += mX;
	y += mY;
	return BPoint(x + mX, y + mY);
}
BPoint BPoint::movef(double  mX, double mY) {
	//x += float(mX);
	//y += float(mY);
	return BPoint(float(x + mX), float(y + mY));
}





//construct a sprite with given texture, size, direction, center point

BSprite::BSprite(Texture texture, BSize size, direction textureDic, BPoint ptCenterStart) :Sprite(texture) {

	spSize = size;
	BSetTexture(texture);
	setPosition(ptCenterStart);
	move(-size.width / 2, -size.height / 2);
	dic = textureDic;

	BTexture = new Texture(texture);
}

Texture* BSprite::getBTexture() {
	return BTexture;
}

void BSprite::setSize(BSize aSize) {
	spSize = aSize;
}
direction BSprite::getDc() {
	return dic;
}

BPoint BSprite::getCenterPoint() {
	return (BGetPosition().move(int(spSize.width / 2), int(spSize.height / 2)));
}


void BSprite::BSetTexture(Texture&texture) {
	setTexture(texture);
	float widthScale = (static_cast<float>(spSize.width) / texture.getSize().x);
	float heightScale = static_cast<float>(spSize.height) / texture.getSize().y;
	BTexture = &texture;
	setScale(widthScale, heightScale);
}

BPoint BSprite::getPeakPoint() {// grabe vector graph's peak point ex: tank's muzzle, Bullet's warhead
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



BPoint BSprite::getCenter() {
	return BPoint(getPosition().x + spSize.width / 2, getPosition().y + spSize.height / 2);
}

//rotate around center point; take the new direction, compare with old direction and determine how many degree need rotate
void BSprite::BRotate(direction newDic) {// rotating around the center position

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

bool BSprite::isContaining(const BPoint & pt, const std::vector<BPoint>*pts ) {
	if (getGlobalBounds().contains(pt.getV2f()))
		return true;
	else if (pts) {
		for (unsigned i = 0; i < unsigned(pts->size()); i++) {
			if (getGlobalBounds().contains(pts->at(i).getV2f()))
				return true;
		}
	}
	return false;
}

void BSprite::setPosition(BPoint pt) {
	Sprite::setPosition(pt.x, pt.y);
}
BPoint BSprite::BGetPosition() {
	return BPoint(getPosition().x, getPosition().y);
}

void BSprite::setCenterAt(BPoint pt) {
	Sprite::setPosition(pt.x - spSize.width / 2, pt.y - spSize.height / 2);
}

Sprite BSprite::getSprite() {
	Sprite tmp;
	tmp = *dynamic_cast<Sprite*>(this);
	tmp.setTexture(*BTexture);
	return tmp;
}
BSprite::~BSprite() {
	BTexture = new Texture();
	delete BTexture;
}


BText::BText(const String & string, Color aColor, BPoint pt, Font*ft, unsigned
	int characterSize , const std::string strFileName
) :Text(string, Font(), characterSize) {
	if (ft)
		myFont = *ft;
	else {
		if (!myFont.loadFromFile(strFileName)) {
			std::cerr << "Can't open font file" << strFileName << std::endl;
			exit(-1);
			return;
		}
	}
	setStyle(sf::Text::Bold);
	setFillColor(aColor);
	setPosition(pt.getV2f());
}
void BText::setAlignment(FloatRect boundRect, Align align) {
	setFont(myFont);
	FloatRect CGRect = Text::getLocalBounds();
	float width = CGRect.width;
	float height = CGRect.height;
	switch (align)
	{
	case center:
		GoCenter(boundRect);
		break;
	case topLeft:
		setPosition(Vector2f(boundRect.left, boundRect.top));
		break;
	case topRight:
		setPosition(Vector2f(boundRect.width - width, boundRect.top));
		break;
	case botLeft:
		setPosition(Vector2f(boundRect.left, boundRect.height - height));
		break;
	case botRight:
		setPosition(Vector2f(boundRect.width - width, boundRect.top - height));
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
		move(0, -boundRect.height / 2 + height / 2 + 2);
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

void BText::GoCenter(FloatRect boundRect) {
	setFont(myFont);
	FloatRect tmp = Text::getLocalBounds();
	Vector2f a = Vector2f(boundRect.left + float(boundRect.width / 2), boundRect.top + float(boundRect.height / 2));
	setPosition(a);
	move(-tmp.width / 2, -tmp.height);
	bdRect = boundRect;
	mAlign = center;
};

Text& BText::getText() {
	setAlignment(bdRect, mAlign);
	Text&tmp = *dynamic_cast<Text*>(this);
	tmp.setFont(myFont);
	return tmp;
}


GIF::GIF(std::vector<std::string>fileName, BPoint pt) {

	for (unsigned i = 0; i < fileName.size(); i++) {
		Texture*tmp = new Texture();
		if (!tmp->loadFromFile(fileName[i])) {
			std::cerr << "Can't Open file " << fileName[i] << std::endl;
			std::exit(1);
		}
		txs.push_back(tmp);
	}
	sp = new BSprite(*txs[0], BSize(50, 50), top, pt);
}

bool GIF::playNext() {
	indexOfPic++;
	if (indexOfPic == txs.size() || indexOfPic < 0)
		return false;
	mMutex.lock();
	sp->BSetTexture(*txs[indexOfPic]);
	mMutex.unlock();
	return true;
}