/*--------------------------------]
[ High Scores List                ]
[ Michael Sanders                 ]
[ ---                             ]
[ Score.cpp                       ]
[ This file contains definitions  ]
[  for the overloaded constructor ]
[  and public methods for the     ]
[  Scores class.                  ]
[--------------------------------*/
#pragma warning(disable : 4996)
#include "../include/Score.h"

/* overloaded char* constructor
Parses data into a Score object. It is assumed data is stored in the
 char* in the following format:
 [0-19]:  name data followed by spaces
 [20-31]: date data followed by spaces
 [32-35]: score data followed by spaces
*/
Score::Score(char* str) {
	//parse string into name, date, score 
	char inName[20] = "", inDate[15] = "", inScore[5] = "";
	int points = 0;
	strncpy(inName, str, 17);    //name
	strtok(inName, "\n\t");      //clean
	strncpy(inScore, str + 20, 5); //score
	strtok(inScore, "\n\t");      //clean
	strncpy(inDate, str + 25, 12); //date
	strtok(inDate, "\n\t");     //clean
	//convert score to int
	points = strtol(inScore, nullptr, 0);

	//set Score data
	strcpy(name, inName);
	date = inDate;
	score = points;
}

//public methods
/* print
Prints the Score data to the console
*/
void Score::print() const {
	std::cout << std::setw(20) << name;
	std::cout << std::setw(5) << score;
	std::cout << std::setw(12) << date << std::endl;
}

//overloaded operators
/* operator<<
Formats Score data and delivers to the ostream object on the lhs of
 the operator<<.
PRE: ref to ostream to output to, const ref to Score object to print
POST: private member data of Score object is delivered to ostream.
RETURN: ref to ostream to allow chaining calls (ex. cout << Score1 << Score2)
*/
std::ostream& operator<< (std::ostream& ostm, const Score &rhs) {
	//deliver info to ostream
	ostm << std::left << std::setw(20) << rhs.name;
	ostm << std::left << std::setw(4) << rhs.score;
	ostm << std::left << std::setw(10) << rhs.getDate();

	return ostm; //allows chaining calls
}

/* operator<
Compares two Score objects based on the score.
RETURN: true if score data in lhs Score is less than rhs score
		false otherwise
*/
bool Score::operator< (const Score &rhs) {
	return this->getScore() < rhs.getScore();
}