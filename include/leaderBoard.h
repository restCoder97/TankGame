#pragma once
/*----------------------------------------------]
[ LeaderBoard                                   ]
[ Michael Sanders                               ]
[ ---                                           ]
[ LeaderBoard.h                                 ]
[ This class paints top score data to a Render  ]
[ Window.                                       ]
[----------------------------------------------*/
#pragma once
#include "BGraphic.h"
#include "HighScores.h"
using namespace sf;

class LeaderBoard {
private:
	RenderWindow* leaderboardWindow;
	BText nameData;
	BText pointData;
	BText dateData;
	BText boardHeader;
	int MaxNumberOfScores = 10;
	int numberOfScores;
	Font* MyFont;

public:
	LeaderBoard() {
		MyFont = new Font();
		if (!MyFont->loadFromFile("fonts/font2.ttf")) { //load font
			std::cerr << "Could not load font " << "fonts/font2.ttf" << std::endl;
			std::exit(1);
		}
	};
	~LeaderBoard() {
		if (leaderboardWindow)
			delete leaderboardWindow;
		if (MyFont)
			delete MyFont;
	};
	void show();

};