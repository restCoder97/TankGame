#include "leaderBoard.h"
#include<sstream>
using namespace sf;

//show
void LeaderBoard::show() { //call to paint the leaderboard
	Event*event;
	event = new Event();
	leaderboardWindow = new RenderWindow(VideoMode(1000, 1000), "LeaderBoard"); //init window
	while (leaderboardWindow->isOpen()) {
		while (leaderboardWindow->pollEvent(*event)) {
			
			if (event->type == Event::EventType::Closed) {
				leaderboardWindow->close();
				delete event;
			}
				

			leaderboardWindow->clear();
			
			leaderboardWindow->requestFocus();

			

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
			for (int i = 0; i < numberOfScores; i++) {
				ssScoreData.str(""); //refresh score label
				ssScoreData << scores[i].getName();
				nameData = BText(ssScoreData.str(), Color::Green, BPoint(110, 90 * (i + 1)), MyFont);
				ssScoreData.str(""); //refresh score label
				ssScoreData << scores[i].getScore();
				pointData = BText(ssScoreData.str(), Color::Green, BPoint(280, 90 * (i + 1)), MyFont);
				ssScoreData.str(""); //refresh score label
				ssScoreData << scores[i].getDate();
				dateData = BText(ssScoreData.str(), Color::Green, BPoint(330, 90 * (i + 1)), MyFont);
				leaderboardWindow->draw(nameData.getText());
				leaderboardWindow->draw(pointData.getText());
				leaderboardWindow->draw(dateData.getText());
			}
			leaderboardWindow->display();

		}
	}
	


}