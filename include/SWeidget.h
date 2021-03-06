#pragma once
#include"BGraphic.h"
#include<thread>
#include <functional>
using namespace std;
using namespace sf;
class SWidget : public RectangleShape {
protected:
	BPoint position;// the lefttop position
	BSize bSize;// the widget's size is also the rectanleShape's size
	BText title; // text display on the wighet
	Color backgroundColor;// this Rect's background color
	Color textColor;// the title's color
	Font font; //title font
	int fontSize;//title size
	RenderWindow* baseWindow;//father window
public:
	SWidget() {};
	SWidget(BSize size, BPoint pt, Color backgroundColor, string title, RenderWindow* mainWindow);
	void show();//show the widget
	void hide();//hide the widget

	void draw(RenderWindow* mainWindow);//draw the widget
	void setTextColor(Color clr) {// set tittle color
		title.setFillColor(clr);
	}
	//Set and Get Functions:
	void setBackgroundColor(Color);
	Color getBackgroundColor();
	void setText(string, int, Color, BPoint);
	BText getText();
	string getTitle();
	void  setTittleSize(int n);
	// The below two functions need to be repaired, more or so the isMouseOver
	bool contains(BPoint pt);
};



class SButton : public SWidget {
	
public:
	SButton() {};
	SButton(BSize size, BPoint pt, Color clr, string btnName, RenderWindow* wdw) : SWidget(size, pt, clr, btnName, wdw) {}; // Newly added constructor
	void buttonDown(BPoint mouseClickedpt, std::function<void(void)>doSomthing) {
		if (this->contains(mouseClickedpt)) {	// if clicked in the Button
			doSomthing(); // Calls the playButtonDown function
		}
	}//doSomthing is a function in the main. when user click this button you gonna call this function/ You need to learn how to pass a function as a parameter


};


class BLineEdit :public SWidget {
	
	Event*m_event;
	
public:
	bool isFocusing = false;
	BLineEdit(BPoint pt,RenderWindow*fatherWindow, std::string str = "", BSize size = BSize(200, 30),
		Color textColor = Color::Black, Color backgroundColor = Color::White) :
		SWidget(size, pt, backgroundColor, str, 
			fatherWindow) {
		baseWindow = fatherWindow;
		FloatRect tmp = FloatRect(position.x, position.y, bSize.width, bSize.height);
		title.setAlignment(tmp, centerLeft);
	}
	
	BLineEdit() {};
	
	void readInput(int keyCode,std::function<void(std::string)>doSomthing = NULL){
		char c = getKey(keyCode);
		int nC = int(c);
		string textString = title.getString();
		if ((c >= 97 && c <= 122) || (c == '.') || (nC >= 48 && nC <= 57)) {
			FloatRect tmp = title.getGlobalBounds();
			if(!(tmp.width >= bSize.width-20))
				textString += c;
		}
			

		else if (c == '\b')
			textString = textString.substr(0, textString.size() - 1);
		else if (c == '\n') {
			if(doSomthing!=NULL)
				doSomthing(textString);
		}
			
		title.setString(textString);
		string a = title.getString();
		FloatRect tmp = FloatRect(this->getPosition().x, this->getPosition().y, bSize.width, bSize.height);
		title.setAlignment(tmp, centerLeft);
	}

	void setFocus(BPoint pt,Window*w,Event*event) {
		if (!contains(pt)) {
			isFocusing = false;
			setBackgroundColor(Color::White);
			return;
		}
			
		m_event = event;
		isFocusing = true;
		setBackgroundColor(Color(128, 128, 0,50));

	}


	//Credit prof Bently
	char getKey(int keyCode)
	{
		if (keyCode ==58) {//enter
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
			char result = static_cast<char>(keyCode+48);
			return result;
		}
		return ' ';
	}


};
