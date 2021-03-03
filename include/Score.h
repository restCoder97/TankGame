#pragma once
#pragma warning(disable : 4996)
/*--------------------------------]
[ High Scores List                ]
[ Michael Sanders                 ]
[ ---                             ]
[ Score.h                         ]
[ This header file contains the   ]
[  declaration of the Score class ]
[  and short definitions.         ]
[--------------------------------*/

#pragma once

#include <fstream>
#include "Date.h"

class Score {
private:
	char name[25];
	unsigned int score;
	Date date;

public:
	//default constructor
	Score() : name(""), score(0), date() {}

	//overloaded constructors
	Score(char* str, int num, Date inDate)
	{
		strcpy(name, str); score = num; date = inDate;
	}
	Score(std::string str, int num, Date inDate)
	{
		strcpy(name, str.c_str()); score = num; date = inDate;
	}
	Score(char *);

	//getters
	char* getName() { return name; }
	int getScore() const { return score; }
	Date getDate() const { return date; }
	//setters
	void setName(char* str) { strcpy(name, const_cast<char*>(str)); }
	void setScore(int num) { score = num; }
	void setDate(Date inDate) { date = inDate; }

	//public methods
	void print() const;

	//overloaded operators
	friend std::ostream& operator<< (std::ostream&, const Score &);
	bool operator< (const Score &);
};