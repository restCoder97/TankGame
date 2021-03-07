#pragma once
/*-------------------------------]
[ High Scores List               ]
[ Michael Sanders                ]
[ ---                            ]
[ Date.h                         ]
[ This header file contains the  ]
[  declaration of the Date class ]
[  and short definitions.        ]
[-------------------------------*/

#pragma once

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstring>
#include <cstdlib>

class Date {
private:
	time_t myDate;

public:
	//default constructor
	Date() { myDate = time(nullptr); } //create Date object containing current date

	//overloaded constructors
	Date(time_t inTime) { myDate = time(&inTime); } //Date in time_t format
	Date(const char*); //accept dates with formats "mm/dd/yy", "mm-dd-yy", "ddmonyy", or "mm/dd/yyyy"

	//copy constructor
	Date(const Date&) = default; //default copy constructor ok, no ptr

	//getter
	time_t getMyDate() const { return myDate; }

	//public methods
	void print() const;

	//operator overloads
	//make friend to allow access to private member data 
	friend std::ostream& operator<< (std::ostream&, const Date &);
};