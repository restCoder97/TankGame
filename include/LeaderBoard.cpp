#include "leaderBoard.h"
#include<sstream>
using namespace sf;

//show
void LeaderBoard::show() { //call to paint the leaderboard
	Event*event;
	event = new Event();
	leaderboardWindow = new RenderWindow(VideoMode(1100, 1000), "LeaderBoard"); //init window
	while (leaderboardWindow->isOpen()) {
		while (leaderboardWindow->pollEvent(*event)) {
			
			if (event->type == Event::EventType::Closed) {
				leaderboardWindow->close();
				delete event;
			}
				

			leaderboardWindow->clear();
			
			leaderboardWindow->requestFocus();

			Font* MyFont = new Font();
			if (!MyFont->loadFromFile("fonts/font2.ttf")) { //load font
				std::cerr << "Could not load font " << "fonts/font2.ttf" << std::endl;
				std::exit(1);
			}

			//show header
			std::ostringstream ssHeader;
			ssHeader << "Top scores:";
			boardHeader = BText(ssHeader.str(), Color::Red, BPoint(100, 25), MyFont, 50);
			leaderboardWindow->draw(boardHeader.getText());
			const int MaxNumberOfScores = 10;
			Score scores[MaxNumberOfScores];
			char ScoresFile[] = "topscores.txt";
			int numberOfScores = getScoresFromFile(ScoresFile, scores);

			std::ostringstream ssScoreData;
			for (int i = 0; i < MaxNumberOfScores; i++) {
				ssScoreData.str(""); //refresh score label
				ssScoreData << scores[i];
				scoreData = BText(ssScoreData.str(), Color::Green, BPoint(110, 90 * (i + 1)), MyFont);
				leaderboardWindow->draw(scoreData.getText());
			}
			leaderboardWindow->display();

		}
	}
	


}