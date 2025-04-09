#include <iostream> // Provides basic input/output functionality
#include "utils.h"  // Header file where these functions are declared

// Function to move the console cursor to a specific (x, y) coordinate
void gotoxy(int x, int y) // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
{
    std::cout.flush(); // Flushes the output buffer to ensure the cursor position update is immediate
    COORD coord;       // Create a COORD structure to store the x and y coordinates
    coord.X = x;       // Set the X coordinate (horizontal position)
    coord.Y = y;       // Set the Y coordinate (vertical position)
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Use Windows API to set cursor position
}
// Function to get char from console at a specific (x, y) coordinate (from ChatGPT)
char getCharAtPosition(int x, int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
    wchar_t buffer[1];
    DWORD charsRead; 
    COORD coord = { (SHORT)x, (SHORT)y };

    if (ReadConsoleOutputCharacterW(hConsole, buffer, 1, coord, &charsRead) && charsRead == 1) 
    {
        char result = '\0';  
        if (buffer[0] <= 0x7F) 
        {
            result = static_cast<char>(buffer[0]);
        }
        return result;
    }
    else 
    {
        return '\0';
    }
}

// Function to show or hide the console cursor
void ShowConsoleCursor(bool showFlag) // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE); // Get the handle to the console output
    CONSOLE_CURSOR_INFO cursorInfo;              // Structure to store cursor information
    GetConsoleCursorInfo(out, &cursorInfo);      // Retrieve the current cursor settings
    cursorInfo.bVisible = showFlag;              // Update the cursor visibility based on showFlag
    SetConsoleCursorInfo(out, &cursorInfo);      // Apply the updated cursor settings
}
