#include "SmartGhost.h"

// Moves the Ghost, checking for walls and platforms.
void SmartGhost::Objects_movement(int Player_Floor,long seed, bool not_to_draw)
{
    //Decieding the ghost's target, based on player's current floor
    if (dir.y==Stay)
    {
        if (Player_Floor > y && Is_Block(pBoard->getChar(x, y + Down))&& pBoard->getChar(x, y + Down + Down)== Ladder )
        {
            dir.y = Down;
        }
        else if(Player_Floor < y && Current_Char == Ladder)
        {
            dir.y = Up;
        }
    }
    int newX = x + dir.x;
    int finalX = x;                    // Final X position after movement
    int newY = y + dir.y;              // Calculate new Y position
    int finalY = y;                    // Final Y position after movement
    char NextChar_Y = pBoard->getChar(x, newY); // Character at the next position
    char NextChar_X = pBoard->getChar(newX, y); // Character at the next position
    // Logic for climbing ladders and handling platforms
    if (Current_Char == Ladder && Is_Block(NextChar_Y) && dir.y == Up)
    {
        finalY = newY + Up; // end Move up the ladder
        dir.y = Stay;
    }
    else if (Is_Block(NextChar_Y) && pBoard->getChar(x, y + Down + Down) == Ladder && dir.y == Down)
    {
        finalY = newY + Down; // start Move down the ladder
    }
    else if (Is_Block(NextChar_Y) && Current_Char == Ladder && dir.y == Down && last_dir.y ==Down)
    {
        dir.y = Stay; // Stop movement on hitting a platform
    }
    else if (Current_Char == Ladder && NextChar_Y == Ladder && dir.y != Stay)
    {
        finalY = newY;
    }
    else
    {
        if (Is_Block_By_Wall(NextChar_X) || pBoard->getChar(newX, y + Down) == Empty_Space) 
        {
            dir.x *= -1; // Reverse horizontal direction
        }
        finalX = x + dir.x;
    }

     draw_update_Objects_place(finalX, finalY, not_to_draw);

    // Update last direction and free fall counter
    
    if (Current_Char != Ladder)
    {
        dir.x *= Random_Direction(seed);
    }
    last_dir = dir;
}






