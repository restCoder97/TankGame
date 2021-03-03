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
	BText scoreData;
	BText boardHeader;
	int MaxNumberOfScores = 10;
	int numberOfScores;

public:
	LeaderBoard() = default;
	~LeaderBoard() = default;
	void show();

};