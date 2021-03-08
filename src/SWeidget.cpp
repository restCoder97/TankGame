#include"SWeidget.h"

SWidget::SWidget(BSize size, BPoint pt, Color backgroundColor, string strTitle, RenderWindow* mainWindow) {
	bSize = size;
	setSize(Vector2f(bSize.width, bSize.height));
	position = pt;
	setPosition(pt.getV2f());
	setBackgroundColor(backgroundColor);
	setFillColor(getBackgroundColor());
	setText(strTitle, 20, Color::Blue, BPoint(pt.x + 50, pt.y + 25));
	hide();// hide the widget after creat
};
void SWidget::show() {
	setPosition(Vector2f(position.x, position.y));// move widget to correct position
}
void SWidget::hide() {
	setPosition(-1000, -1000);//move it out of screen
}

void SWidget::draw(RenderWindow* mainWindow) {
	mainWindow->draw(*this);//draw this wighet
	mainWindow->draw(this->title->getText());// draw text on the wighet
}


string SWidget::getTitle() {// get title string
	return title->getString();
}

void  SWidget::setTittleSize(int n) {
	return title->setCharacterSize(n);
}


// The below two functions need to be repaired, more or so the isMouseOver
bool SWidget::contains(BPoint pt) {
	FloatRect tmp = RectangleShape::getGlobalBounds();
	if (RectangleShape::getGlobalBounds().contains(pt.getV2f()))
		return true;
	return false;
}

void SWidget::setBackgroundColor(Color bgdColor) {
	backgroundColor = bgdColor;
}

Color SWidget::getBackgroundColor() {
	return backgroundColor;
}

void SWidget::setText(string text, int fontSize, Color textColor, BPoint pt) {
	font.loadFromFile("fonts/font2.ttf");
	title = new BText(text, textColor, BPoint(0, 0), nullptr, fontSize, "fonts/font2.ttf");
	FloatRect tmp = FloatRect(this->getPosition().x, this->getPosition().y, bSize.width, bSize.height);//tmp is the bounding rect
	title->GoCenter(tmp);//this will set the text's Align
}

BText SWidget::getText() {
	return *title;
}

SWidget :: ~SWidget() {
	title = new BText();
	delete title;
 }

namespace sb {
	void SButton::buttonDown(BPoint mouseClickedpt, std::function<void(void)>doSomthing) {
		if (this->contains(mouseClickedpt)) {	// if clicked in the Button
			doSomthing(); // Calls the playButtonDown function
		}
	}//doSomthing is a function in the main. when user click this button you gonna call this function/ You need to learn how to pass a function as a parameter
}



BLineEdit::BLineEdit(BPoint pt, RenderWindow*fatherWindow, std::string str , BSize size ,
	Color textColor, Color backgroundColor ) :
	SWidget(size, pt, backgroundColor, str,
		fatherWindow) {
	baseWindow = fatherWindow;
	FloatRect tmp = FloatRect(position.x, position.y, bSize.width, bSize.height);
	title->setAlignment(tmp, centerLeft);
}



void BLineEdit::readInput(int keyCode, std::function<void(std::string)>doSomthing ) {
	char c = getKey(keyCode);
	int nC = int(c);
	string textString = title->getString();
	if ((c >= 97 && c <= 122) || (c == '.') || (nC >= 48 && nC <= 57)) {
		FloatRect tmp = title->getGlobalBounds();
		if (!(tmp.width >= bSize.width - 20))
			textString += c;
	}

	else if (c == '\b')
		textString = textString.substr(0, textString.size() - 1);
	else if (c == '\n') {
		if (doSomthing != NULL)
			doSomthing(textString);
	}

	title->setString(textString);
	string a = title->getString();
	FloatRect tmp = FloatRect(this->getPosition().x, this->getPosition().y, bSize.width, bSize.height);
	title->setAlignment(tmp, centerLeft);
}

void BLineEdit::setFocus(BPoint pt, Window*w, Event*event) {
	if (!contains(pt)) {
		isFocusing = false;
		setBackgroundColor(Color::White);
		return;
	}

	m_event = event;
	isFocusing = true;
	setBackgroundColor(Color(128, 128, 0, 50));

}



char BLineEdit::getKey(int keyCode)
{
	if (keyCode == 58) {//enter
		isFocusing = false;
		return  '\n';
	}
	else if (keyCode == sf::Keyboard::Key::Period) {
		return'.';
	}
	else if (keyCode == 59)
		return  '\b';
	else if (keyCode >= 0 && keyCode <= 25) {//a-z
		char result = static_cast<char>(keyCode + 97);
		return result;
	}
	else if (keyCode >= 26 && keyCode <= 35) {//keyboard numbers 123.....
		keyCode -= 26;
		char result = static_cast<char>(keyCode + 48);
		return result;
	}
	return ' ';
}
