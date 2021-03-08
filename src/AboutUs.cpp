#include "AboutUs.h"
#include<sstream>
#include<iomanip>
using namespace sf;

//show
void AboutUs::show() { //call to paint the leaderboard
	Event*event;
	event = new Event();
	aboutWindow = new RenderWindow(VideoMode(800, 800), "AboutUs"); //init window
	while (aboutWindow->isOpen()) {
		while (aboutWindow->pollEvent(*event)) {

			if (event->type == Event::EventType::Closed) {
				aboutWindow->close();
				delete event;
			}
			aboutWindow->clear();
			aboutWindow->requestFocus();
			//show header
			std::ostringstream ssHeader;
			ssHeader << "About the developers:";
			devHeader = BText(ssHeader.str(), Color::Red, BPoint(100, 25), myFont, 50);
			devHeader.setAlignment(FloatRect(0, 100, 800, 800), Align::centerTop);
			aboutWindow->draw(devHeader.getText());

			std::ostringstream ssDevData;
			ssDevData.str(""); //refresh data label
			ssDevData << std::left << std::setw(20) << "Lead:   " << std::right << std::setw(21) << "Youxing (Bill) Chen\n"
				<< std::left << std::setw(20) << "Art Director:" << std::right << std::setw(20) << "       Yuntian Wu\n"
				<< std::left << std::setw(20) << "Front End:   " << std::right << std::setw(20) << "       Sriram Atluri\n"
				<< std::left << std::setw(20) << "Back End:    " << std::right << std::setw(20) << "    Vladimir Kataev\n"
				<< std::setw(44) << std::right << "Michael Sanders\n"
				<< std::left << std::setw(20)<<"\nInstructor: "<< std::right << std::setw(20)<<"    Joe Bentley\n";
			devNames = BText(ssDevData.str(), Color::Green, BPoint(110, 90), myFont);
			devNames.GoCenter(FloatRect(0, 100, 800, 800));
			aboutWindow->draw(devNames.getText());

			aboutWindow->display();

		}
	}
	//delete event;
}
