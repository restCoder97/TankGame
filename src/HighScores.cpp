#include "../include/HighScores.h"
#pragma warning(disable : 4996)
/*----------------------------------------------]
[ High Scores List                              ]
[ Michael Sanders                               ]
[ ---                                           ]
[ HighScores.cpp                                ]
[ This program implements a high-score tracker, ]
[  as might be found in an arcade game.         ]
[----------------------------------------------*/

/*---------------------]
[ FUNCTION DEFINITIONS ]
[---------------------*/
/* processInputFile
Read in one line from the input file, and use it to
 construct a new Score.
PRE: ifstream associated with an input file with last game record,
	 Score pointer to collect new input data
POST: a Score object with new game data is assigned to the
	  input Score pointer
RETURN: void
*/
void processInputFile(std::ifstream &ifstm, Score &scr) {

	//get line from file (new game record)
	char line[40];
	ifstm.getline(line, 40);

	if (!ifstm.eof()) {       //only process if there is data to process 
		Score freshScore(line); //pass line to Score constructor for processing 
		scr = freshScore;       //assign newly-made Score to scr
	}
}

/* getScoresFromFile
Read in top scores data from binary file and store in Score array.
 All the top scores are read in anew every program loop.
PRE: link to binary top scores file, pointer to Score array to hold
	 top score data
POST: Score array is populated with Score objects containing the top
	  score data
RETURN: number of scores stored in Score array
*/
int getScoresFromFile(char* file, Score* scr) {

	//initialize variables
	int countScores = 0;
	char aScore[45] = "";

	//open as binary file
	std::ifstream inFile;
	inFile.open(file, std::ios::binary);

	//make new binary file if it doesn't open (first time only)
	if (inFile.fail()) {
		std::ofstream outFile;
		outFile.open(file, std::ios::binary);
		outFile.close();
		return 0; //exit out on first time through
	}

	//read in a line
	while (inFile.getline(aScore, 40, '\n')) {

		//process line
		char name[25] = "";
		strncpy(name, aScore, 20);
		char date[15] = "";
		strncpy(date, aScore + 24, 10);
		char score_char[5] = "";
		strncpy(score_char, aScore + 20, 3);
		int score_int = strtol(score_char, nullptr, 10);

		//construct score from binary data
		Score freshScore(name, score_int, date);
		scr[countScores] = freshScore;

		countScores++;
	}

	inFile.close();
	return countScores;
}

/* writeScoresToFile
Write the contents of the Score array holding top scores to a binary file.
 This overwrites the last output file and stores the array contents anew
 every program loop.
PRE: link to output file, Score array of top scores, number of Scores to
	 write to output file
POST: output file is generated which stores top score data
RETURN: void
*/
void writeScoresToFile(char* file, Score* scr, int numScores) {

	//open binary file
	std::ofstream ofs;
	ofs.open(file, std::ios::binary);

	//write all scores to new file
	if (ofs.is_open()) {
		for (int i = 0; i < numScores; i++)
			ofs << scr[i]; //write to file 
		ofs.close(); //close file
	}
	else std::cout << "Error opening file!" << std::endl;
}

/* sort
If the new score data from the input file should be stored as a top
 score, this function places the new score in its proper location in
 the top scores Score array. Because the array is sorted every time
 a new Score is added as a top score, only the new Score might be in
 the wrong location. So, this function checks whether the new Score
 is higher than the score at the location before it, and shifts the
 new Score to a higher position until it encounters a score higher
 than it.
PRE: a new Score to add to the top scores Score array, number of
	 scores in Score array
POST: the order of Scores in the Score array may be adjusted
RETURN: void
*/
void sort(Score* scr, unsigned int numScores) {

	Score newScr = scr[numScores - 1];
	Score tmp;
	for (int i = numScores - 1; i > 0; i--) {
		if (scr[i - 1] < newScr) { //if new score is larger than preceeding score...
			tmp = scr[i - 1];       //...swap scores
			scr[i - 1] = newScr;
			scr[i] = tmp;
		}
		else break; //stop when new score is smaller than preceeding score
	}
}

/*-------------------------------------------]
[ doScores()                                 ]
[ ---                                        ]
[ In doScores(),                             ]
[ 1) get winner data from most recent game   ]
[ 2) read in the binary top scores file      ]
[    and store in an array of Scores.        ]
[ 3) determine whether the new score should  ]
[    be added to the top scores. Add new     ]
[    score if there are fewer than 10 top    ]
[    scores, or if the new score is larger   ]
[    than an existing score.                 ]
[ 4) print out the top scores.               ]
[ 5) overwrite the top scores binary file    ]
[    with the new top scores data.           ]
[-------------------------------------------*/
void doScores()
{
	const int MaxNumberOfScores = 10;
	Score scores[MaxNumberOfScores];
	char InputFile[] = "gameRecord.txt";
	char ScoresFile[] = "topscores.txt";

	std::ifstream fin(InputFile);
	if (!fin) {
		std::cerr << "Unable to open input file " << InputFile << std::endl;
		exit(1);
	}

	Score newScore;
	unsigned int numScores;        //number of scores to be processed
	bool updateScores = false;     //flag to indicate a change in the scores array

	processInputFile(fin, newScore); //read recent game record from input file

	//read binary Scores file
	numScores = getScoresFromFile(ScoresFile, scores);

	//determine whether to add newScore as a top score
	if (numScores < MaxNumberOfScores) {  //Less than 10 Scores
		scores[numScores++] = newScore;
		updateScores = true;
	}
	else if (scores[numScores - 1] < newScore) { //newScore > lowest of Top Scores
		scores[numScores - 1] = newScore;          //add newScore to Top Scores
		updateScores = true;
	}
	else updateScores = false;                   //no change to scores array

	if (updateScores) {
		sort(scores, numScores);                   //sort Scores

		/*
		for (auto i = 0U; i < numScores; ++i)
		  std::cout << std::left << std::setw(3) << i+1 << scores[i];
		std::cout << "-------------------------------------\n";
		*/

		//Write binary Scores file
		writeScoresToFile(ScoresFile, scores, numScores);
	}
}

//eraseScoresFile
void eraseScoresFile(char* file) { remove(file); }