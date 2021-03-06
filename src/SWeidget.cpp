#include"SWeidget.h"



SWidget::SWidget(BSize size, BPoint pt, Color backgroundColor, string title, RenderWindow* mainWindow) {
	bSize = size;
	setSize(Vector2f(bSize.width, bSize.height));
	position = pt;
	setPosition(pt.getV2f());
	setBackgroundColor(backgroundColor);
	setFillColor(getBackgroundColor());
	setText(title, 20, Color::Blue, BPoint(pt.x + 50, pt.y + 25));
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
	mainWindow->draw(this->title.getText());// draw text on the wighet
}


string SWidget::getTitle() {// get title string
	return title.getString();
}

void  SWidget::setTittleSize(int n) {
	return title.setCharacterSize(n);
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
	font.loadFromFile("fonts\\font2.ttf");
	title = BText(text, textColor, BPoint(0, 0), nullptr, fontSize, "fonts//font2.ttf");
	FloatRect tmp = FloatRect(this->getPosition().x, this->getPosition().y, bSize.width, bSize.height);//tmp is the bounding rect
	title.GoCenter(tmp);//this will set the text's Align
}

BText SWidget::getText() {
	return title;
}