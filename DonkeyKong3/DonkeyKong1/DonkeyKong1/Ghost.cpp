#include "Ghost.h"

// Moves the Ghost, checking for walls and platforms.
void Ghost::Objects_movement(bool not_to_draw)
{
    int newX = x + dir.x;
    int finalX = x;
    char NextChar = pBoard->getChar(newX, y);

    // Change direction when hitting a wall or edge
    if (Is_Block_By_Wall(NextChar) || pBoard->getChar(newX, y + Down) == Empty_Space)
    {
        dir.x *= -1; // Reverse horizontal direction
        newX = x + dir.x;
    }

    finalX = newX;
    draw_update_Objects_place(finalX, y, not_to_draw);
    dir.x *= Random_Direction(GetSeed());
}
