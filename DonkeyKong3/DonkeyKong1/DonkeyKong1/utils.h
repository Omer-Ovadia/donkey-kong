#pragma once // Ensures the header file is included only once during compilation
#include <iostream>
#include <Windows.h> // Provides access to the Windows API functions and structures
#include "utils.h"   // Includes the header file for utility functions

// Declaration of the gotoxy function
// Moves the console cursor to a specific (x, y) coordinate
void gotoxy(int x, int y); // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165

char getCharAtPosition(int x, int y);

// Declaration of the ShowConsoleCursor function
// Toggles the visibility of the console cursor
void ShowConsoleCursor(bool showFlag); // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165// this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165