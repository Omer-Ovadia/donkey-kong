#include "GhostManager.h"

// Generates a random number for the ghost's movement direction.
int GhostManager::Random_Direction(long seed)
{
    srand(seed);
    int random_number = rand() % 100 + 1;
    return (random_number <= 5) ? -1 : 1;
}

// Function to draw the Ghost at its current position. Color functions from windows.h taken from ChatGPT
void GhostManager::draw(char c) const
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (isColored)
    {
        if (c == Ghost_symbol)
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Blue
        }
        if (c == Smart_Ghost_symbol)
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Blue
        }
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 7); // Default color
    }

    gotoxy(x, y);
    std::cout << c;
    SetConsoleTextAttribute(hConsole, 7);
}