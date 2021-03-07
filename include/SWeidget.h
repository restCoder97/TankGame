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
	BText*title = nullptr; // text display on the wighet
	Color backgroundColor;// this Rect's background color
	Color textColor;// the title's color
	Font font; //title font
	int fontSize;//title size
	RenderWindow* baseWindow;//father window
public:
	SWidget() {};
	SWidget(BSize size, BPoint pt, Color backgroundColor, string title, RenderWindow* mainWindow);
	~SWidget();
	void show();//show the widget
	void hide();//hide the widget
	
	void draw(RenderWindow* mainWindow);//draw the widget
	void setTextColor(Color clr) {// set tittle color
		title->setFillColor(clr);
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


namespace sb {
	class SButton : public SWidget {
	public:
		SButton() {};
		SButton(BSize size, BPoint pt, Color clr, string btnName, RenderWindow* wdw) : SWidget(size, pt, clr, btnName, wdw) {}; // Newly added constructor
		void buttonDown(BPoint mouseClickedpt, std::function<void(void)>doSomthing);
	};
}


class BLineEdit :public SWidget {
	Event*m_event;
public:
	bool isFocusing = false;
	BLineEdit() {};
	BLineEdit(BPoint pt, RenderWindow*fatherWindow, std::string str = "", BSize size = BSize(200, 30),
		Color textColor = Color::Black, Color backgroundColor = Color::White);
	void readInput(int keyCode, std::function<void(std::string)>doSomthing = NULL);
	void setFocus(BPoint pt, Window*w, Event*event);
	char getKey(int keyCode);
};
