/*-------------------------------]
[ High Scores List               ]
[ Michael Sanders                ]
[ ---                            ]
[ Date.cpp                       ]
[ This file contains definitions ]
[  for the Date class overloaded ]
[  constructor and public member ]
[  functions.                    ]
[-------------------------------*/
#pragma warning(disable : 4996)
#include "../include/Date.h"

/* overloaded constructor
Takes a char* to a C-String and parses into a tm struct.
The tm struct is then converted into a time_t with the date.
Handles 4 cases:
1) empty string enters - makes Date today's date
2) nonesense string entered (ex. 15236237) - makes Date today's date
3) date entered as DDMonYY, where Mon is Cstring
4) date entered as MM/DD/YY(YY) or MM-DD-YY(YY)
*/
Date::Date(const char* str) {
	char mon[8] = "", day[8] = "", year[8] = "";

	//parse data into ints
	char* catchData = const_cast<char*>(str);
	catchData = strtok(catchData, "-/");

	//empty string or nonsense number
	if (!strcmp(catchData, "           ") || strtol(catchData, nullptr, 0) > 31) {
		//make date today's date
		myDate = time(nullptr);
		return;
	}

	//DDMonYY format
	else if (strlen(catchData) > 2) {
		strncpy(day, catchData, 2); //day 
		strncpy(mon, catchData + 2, 3); //month

		//ugly if statements to change abbv. month to number
		if (!strcmp(mon, "Jan")) strcpy(mon, "01");
		else if (!strcmp(mon, "Feb")) strcpy(mon, "02");
		else if (!strcmp(mon, "Mar")) strcpy(mon, "03");
		else if (!strcmp(mon, "Apr")) strcpy(mon, "04");
		else if (!strcmp(mon, "May")) strcpy(mon, "05");
		else if (!strcmp(mon, "Jun")) strcpy(mon, "06");
		else if (!strcmp(mon, "Jul")) strcpy(mon, "07");
		else if (!strcmp(mon, "Aug")) strcpy(mon, "08");
		else if (!strcmp(mon, "Sep")) strcpy(mon, "09");
		else if (!strcmp(mon, "Oct")) strcpy(mon, "10");
		else if (!strcmp(mon, "Nov")) strcpy(mon, "11");
		else if (!strcmp(mon, "Dec")) strcpy(mon, "12");
		else {}

		strncpy(year, catchData + 5, 2); //year
	}

	//delimited format
	else {
		strcpy(mon, catchData);            //month 
		catchData = strtok(nullptr, "-/"); //advance to next token
		strcpy(day, catchData);            //day
		catchData = strtok(nullptr, "-/"); //advance
		strcpy(year, catchData);           //year
	}

	//process char* to integral and adjust based on standard date Jan 1, 1900
	int int_day = strtol(day, nullptr, 10);     //day of month, 1-31
	int int_mon = strtol(mon, nullptr, 10) - 1; //months since Jan, 0-11 
	int int_year = strtol(year, nullptr, 10);   //year since 1900
	if (int_year > 2000) //make 07 -> 2007, etc.
		int_year -= 2000;  //strip off 2 largest digits
	int_year += 100;     //add 100 years (counts from 1900) 

	//make tm struct
	struct tm MakeTimes = { 0,0,0,0};
	MakeTimes.tm_mday = int_day;
	MakeTimes.tm_mon = int_mon;
	MakeTimes.tm_year = int_year;
	myDate = mktime(&MakeTimes);
}

/* print
Prints the time_t myDate as MM/DD/YYYY
*/
void Date::print() const {
	char buffer[16];
	strftime(buffer, 11, "%m/%d/%Y", localtime(&myDate)); //format Date
	std::cout << buffer << std::endl; //send to console
}

/* operator<<
Formats myDate to MM/DD/YYYY and delivers the data to the ostream
 object on the lhs of the operator<<.
PRE: ref to ostream to output to, const ref to Date object to print
POST: private member data of Date object is delivered to ostream.
RETURN: ref to ostream to allow chaining calls (ex. cout << Date1 << Date2)
*/
std::ostream& operator<< (std::ostream& ostm, const Date &rhs) {

	char outDate[12];
	strftime(outDate, 11, "%m/%d/%Y", localtime(&rhs.myDate)); //format date
	ostm << std::setw(12) << outDate << std::endl; //pass data to ostream

	return ostm; //allows chaining calls
}