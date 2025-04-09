#pragma once

#include <cstring>
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include "utils.h"  
#include "Constants.h"  
using namespace Constants;
using std::ifstream;

class Board
{
    // 2D array to store the current state of the board (including changes during the game).
    char currentBoard[Max_Y_Board][Max_X_Board + 1]; // +1 for null terminator to store strings correctly.

public:
    // Resets the board to its original state and updates the number of lives and score at the top.
    void reset(int Lives, int Score, Location& Legend_location, char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1]);

    // Prints the current state of the board to the console, with options for coloring and hiding specific items.
    void print(bool isColored, int Lives, int Score, Location& Legend_location, bool Not_Print_Hammer, Location& hammer_location);

    // Returns the character at a specific (x, y) position on the board.
    char getChar(int x, int y) const;

    // Updates the score display on the board dynamically during the game.
    void Update_Score(int Score, Location& Legend_location);
};
