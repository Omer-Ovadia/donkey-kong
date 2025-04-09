#include "Barrel.h"

// Moves the barrel, checking for walls, platforms, and handling gravity.
void Barrel::Objects_movement(bool not_to_draw)
{
    int newX = x + dir.x; // Proposed new X position.
    int newY = y + dir.y; // Proposed new Y position.
    int finalX = x;       // Final X position after evaluation.
    int finalY = y;       // Final Y position after evaluation.
    char NextChar = pBoard->getChar(newX, newY); // Character at the proposed position.

    if (Is_Block_By_Wall(NextChar)) { // Change direction when hitting a wall.
        dir.x *= -1; // Reverse horizontal direction.
    }
    else {
        finalX = newX;
        if (pBoard->getChar(x + dir.x, y + Down) == Empty_Space) { // Free fall in air.
            finalY = y + Down;
            sum_floor_in_air++; // Increase fall count.
        }
        else { // Adjust direction based on the type of floor.
            sum_floor_in_air = Reset_Fall; // Reset fall count.
            if (pBoard->getChar(newX, y + Down) == Left_Floor)
                dir.x = Left; // Move left on left-sloping floor.
            if (pBoard->getChar(newX, y + Down) == Right_Floor)
                dir.x = Right; // Move right on right-sloping floor.
        }
    }

    // If position changed, update position.
    if (finalX != x || finalY != y) 
    {
        draw_update_Objects_place(finalX, finalY, not_to_draw);
    }
    last_dir = dir; // Save the last direction.
}

// Checks if the barrel has fallen far enough to explode.
bool Barrel::Is_Barrel_Fall_To_Explosion()
{
    // If the barrel's fall counter exceeds the maximum allowed or it hits the ground, it explodes.
    return sum_floor_in_air >= Max_Fall_Barrel &&
        (Is_Platform(pBoard->getChar(x, y + Down)) || y + Down == Max_Y_Board);
}

// Checks if the barrel is at the release position.
bool Barrel::Is_In_Release_Place() const
{
    return y == Barrel_Release.y; // Compare current position with release position.
}

// Resets the barrel to its initial position.
void Barrel::Reset() {
    draw(' '); // Erase the barrel from its current position.
    dir.x = Right; // Reset direction to right.
    x = Barrel_Release.x + Right; // Reset X position.
    y = Barrel_Release.y + Up;    // Reset Y position.
}

// Function to draw the barrel at its current position. Color functions from windows.h used for visual effects.
void Barrel::draw(char c) const {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Get console handle.
    if (c == Barrel_symbol && isColored) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Set to blue color.
    }
    else {
        SetConsoleTextAttribute(hConsole, 7); // Default color.
    }

    gotoxy(x, y); // Move cursor to barrel's position.
    std::cout << c; // Print the barrel's character.
    SetConsoleTextAttribute(hConsole, 7); // Reset to default color.
}
