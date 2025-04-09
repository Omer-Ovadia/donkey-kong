#include "Board.h"

// Resets the board and updates the lives count.
void Board::reset(int Lives, int Score, Location& Legend_location, char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1])
{
    // Copy the original board from the provided file into the current board array.
    for (int i = 0; i < Max_Y_Board; i++)
    {
        memcpy(currentBoard[i], Board_Read_From_File[i], Max_X_Board + 1);
    }

    // Update the lives display in the Legend place on the board.
    currentBoard[Legend_location.y][Legend_location.x + 6] = '0' + Lives; // Lives in ones place.
    currentBoard[Legend_location.y][Legend_location.x + 16] = '0' + Score / 100; // Hundreds place of score.
    currentBoard[Legend_location.y][Legend_location.x + 17] = '0' + (Score / 10) % 10; // Tens place of score.
    currentBoard[Legend_location.y][Legend_location.x + 18] = '0' + Score % 10; // Ones place of score.
}

// Dynamically updates the score on the board during gameplay.
void Board::Update_Score(int Score, Location& Legend_location)
{
    // Convert the score into its string representation and display it at the appropriate location.
    std::string str;
    str = std::to_string((Score / 100));
    gotoxy(Legend_location.x + 16, Legend_location.y); // Move the cursor to the hundreds place.
    std::cout << str; // Print the hundreds digit.
    str = std::to_string(((Score / 10) % 10));
    gotoxy(Legend_location.x + 17, Legend_location.y); // Move the cursor to the tens place.
    std::cout << str; // Print the tens digit.
    str = std::to_string((Score % 10));
    gotoxy(Legend_location.x + 18, Legend_location.y); // Move the cursor to the ones place.
    std::cout << str; // Print the ones digit.
}

// Prints the current state of the board to the console, applying colors and handling specific item states.
void Board::print(bool isColored, int Lives, int Score, Location& Legend_location, bool Not_Print_Hammer, Location& hammer_location)
{
    // Update the lives and score display in the Legend place.
    currentBoard[Legend_location.y][Legend_location.x + 6] = '0' + Lives;
    currentBoard[Legend_location.y][Legend_location.x + 16] = '0' + Score / 100;
    currentBoard[Legend_location.y][Legend_location.x + 17] = '0' + (Score / 10) % 10;
    currentBoard[Legend_location.y][Legend_location.x + 18] = '0' + Score % 10;

    // If the hammer should not be displayed, replace its position with empty space.
    if (Not_Print_Hammer)
    {
        currentBoard[hammer_location.y][hammer_location.x] = Empty_Space;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Handle for console color settings.

    // Iterate through the board to display each character with optional color.
    for (int i = 0; i < Max_Y_Board; i++)
    {
        for (int j = 0; j < Max_X_Board; j++)
        {
            char c = currentBoard[i][j];
            if (isColored)
            {
                switch (c)
                {
                case '=': // Color for specific floor type.
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                    break;
                case '$': // Color for Pauline's symbol.
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    break;
                case '&': // Color for Donkey Kong's symbol.
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    break;
                case '>': // Color for right-sloping floors.
                case '<': // Color for left-sloping floors.
                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    break;
                default: // Default color for other characters.
                    SetConsoleTextAttribute(hConsole, 7);
                    break;
                }
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 7); // Default color if coloring is disabled.
            }

            std::cout << c; // Print the character to the console.
        }

        if (i != Max_Y_Board - 1)
            std::cout << '\n'; // Print a newline except for the last row.
    }

    SetConsoleTextAttribute(hConsole, 7); // Reset the console color to default.
}

// Returns the character at a specific (x, y) position on the board.
char Board::getChar(int x, int y) const
{
    return currentBoard[y][x]; // Return the character from the current board at the given coordinates.
}
