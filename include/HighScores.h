#pragma once
/*----------------------------------------------]
[ High Scores List                              ]
[ Michael Sanders                               ]
[ ---                                           ]
[ HighScores.h                                  ]
[ This program implements a high-score tracker, ]
[  as might be found in an arcade game.         ]
[----------------------------------------------*/

#pragma once

#include "../include/Date.h"
#include "../include/Score.h"
#include <iostream>
#include <fstream>
#include <cstring>

/*----------------------]
[ FUNCTION DECLARATIONS ]
[----------------------*/
void processInputFile(std::ifstream &, Score &);
int getScoresFromFile(char*, Score*);
void writeScoresToFile(char*, Score*, int);
void sort(Score*, unsigned int);
void doScores();
void eraseScoresFile(char*);