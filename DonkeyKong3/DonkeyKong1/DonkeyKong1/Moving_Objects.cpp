#include "Moving_Objects.h"

// Updates the Object's position and redraws it at the new location.
void Moving_Objects::draw_update_Objects_place(int finalX, int finalY,bool not_to_draw)
{
    if (not_to_draw)
    {
        x = finalX;
        y = finalY;
        Current_Char = pBoard->getChar(x, y);
        if (Current_Char == Hammer_symbol)
        {
            Current_Char = getCharAtPosition(x, y);
        }
    }
    else
    {
        // Erase Object from old position by restoring current_char
        this->draw(Current_Char);

        // Update x and y
        x = finalX;
        y = finalY;

        // Save the character under the new position
        Current_Char = pBoard->getChar(x, y);
        if (Current_Char == Hammer_symbol)
        {
            Current_Char = getCharAtPosition(x, y);
            this->draw(Empty_Space);
        }
        // Draw Object at new position
        this->draw();
    }
}

// Sets the board for the Object and initializes the character under the Object.
void Moving_Objects::setBoard(Board& board)
{
    pBoard = &board;
    Current_Char = pBoard->getChar(x, y); // Initialize underChar
}

// Checks if the current character is a wall.
bool Moving_Objects::Is_Block_By_Wall(char check)const
{
    if (check == 'Q' || (x == Min_X_Board && dir.x == Left) || (x == Max_X_Board - 1 && dir.x == Right))
        return true;
    return false;
}

// Checks if a character represents a blocking object (like a platform or obstacle)
bool Moving_Objects::Is_Block(char check)const
{
    return (check == 'Q' || check == '>' || check == '<' || check == '=' || (x == Min_X_Board && dir.x == Left) || (x == Max_X_Board - 1 && dir.x == Right));
}

// Checks if a character represents a platform
bool Moving_Objects::Is_Platform(char check) const
{
    return (check == '>' || check == '<' || check == '=');
}
