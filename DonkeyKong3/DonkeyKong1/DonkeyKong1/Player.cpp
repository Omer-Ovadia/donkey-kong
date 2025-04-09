#include "Player.h"

// Updates Mario's position on the board and redraws him
void Player::draw_update_Objects_place(int finalX, int finalY,bool not_to_draw)
{
    if (not_to_draw)
    {
        x = finalX;         // Update x-coordinate
        y = finalY;         // Update y-coordinate
        Current_Char = pBoard->getChar(x, y); // Save the character at the new position
        if (Current_Char == Hammer_symbol)
        {
            Is_Holding_Hammer = true;
            Current_Char = Empty_Space;
            mario = Mario_With_Hammer_Symbol;
        }
    }
    else
    {
        draw(Current_Char); // Erase Mario from the old position by redrawing the character under him
        x = finalX;         // Update x-coordinate
        y = finalY;         // Update y-coordinate
        Current_Char = pBoard->getChar(x, y); // Save the character at the new position
        if (Current_Char == Hammer_symbol)
        {
            draw(Empty_Space);
            Is_Holding_Hammer = true;
            Current_Char = Empty_Space;
            mario = Mario_With_Hammer_Symbol;
        }
        draw();             // Draw Mario at the new position
    }
}

// Calculates the maximum height Mario can jump
int Player::Get_Jump_Max(int jump_in_dir_x, int jump_y)
{
    int sum = 0;

    // Check for empty space or ladder one unit above
    if (((pBoard->getChar(jump_in_dir_x, jump_y + Up) == Empty_Space) && (jump_y + Up >= Min_Y_Board)) ||
        pBoard->getChar(jump_in_dir_x, jump_y + Up) == Ladder || pBoard->getChar(jump_in_dir_x, jump_y + Up) == Hammer_symbol)
    {
        sum += Up;
        // Check for empty space or ladder two units above
        if (((pBoard->getChar(jump_in_dir_x + last_dir.x, jump_y + Up + Up) == Empty_Space) && (jump_y + Up + Up >= Min_Y_Board)) ||
            pBoard->getChar(jump_in_dir_x + dir.x, jump_y + Up + Up) == Ladder || pBoard->getChar(jump_in_dir_x + last_dir.x, jump_y + Up + Up) == Hammer_symbol)
        {
            sum += Up;
        }
    }
    return -sum; // Negative because 'Up' is defined as -1
}

// Processes a key press and updates Mario's movement direction
void Player::keyPressed(char key) // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
{
    for (size_t i = 0; i < numKeys; i++)
    {
        if (std::tolower(key) == keys[i]) // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
        {
            if (std::tolower(key) == Hammer_symbol)
            {
                return;
            }
            else
            {
                dir = directions[i]; // Update the direction based on the pressed key
            }
            return;
        }
    }
}

// Checks if Mario has fallen to his death
bool Player::Is_mario_Fall_To_Death()
{
    return (sum_floor_in_air >= Max_Fall_Mario && (Is_Platform(pBoard->getChar(x, y + Down)) || y==Max_Y_Board+Up));
}

// Checks if Mario has won the game
bool Player::Is_mario_Win()
{
    return (Current_Char == Pauline_symbol) ? Win : Not_Won_Yet;
}

// Handles Mario's movement logic based on input and game rules
void Player::Objects_movement(bool not_to_draw)
{
    bool if_mario_in_free_fall = false; // Tracks if Mario is in free fall
    int newX = x + dir.x;              // Calculate new X position
    int newY = y + dir.y;              // Calculate new Y position
    int finalX = x;                    // Final X position after movement
    int finalY = y;                    // Final Y position after movement
    char NextChar = pBoard->getChar(newX, newY); // Character at the next position

    // Logic for climbing ladders and handling platforms
    if (Current_Char == Ladder && Is_Block(NextChar) && dir.y == Up)
    {
        finalY = newY + Up; // Move up the ladder
        dir = { Stay, Stay };
    }
    else if (Is_Block(NextChar) && pBoard->getChar(x, y + Down + Down) == Ladder && dir.y == Down)
    {
        finalY = newY + Down; // Move down the ladder
    }
    else if (Is_Block(NextChar) && Current_Char == Ladder && (last_dir.y == Down || dir.y==Down))
    {
        dir = { Stay, Stay }; // Stop movement on hitting a platform
    }
    else
    {
        if (newX < Min_X_Board || newX > Max_X_Board-1 || Is_Block(NextChar))
        {
            dir.x = Stay;
        }
        else
        {
            finalX = newX; // Update X for horizontal movement
        }
        // Check if Mario is in free fall
        if (pBoard->getChar(x + dir.x, y + Down) == Empty_Space || pBoard->getChar(x + dir.x, y + Down) == Hammer_symbol)
        {
            finalY = y + Down;
            if_mario_in_free_fall = true;
        }
        else if (dir.y != Stay) // Handle vertical movement (jumping or climbing)
        {
            NextChar = pBoard->getChar(x, newY);
            if (dir.y == Up && NextChar != Ladder)
            {
                int sum = Get_Jump_Max(finalX + last_dir.x, y); // Calculate jump height
                dir.x = last_dir.x;
                while (sum != 0)
                {
                    sum -= 1;
                    finalY += Up;
                    finalX += last_dir.x;
                    dir.y = Stay;
                    draw_update_Objects_place(finalX, finalY, not_to_draw);
                }
            }
            else if (NextChar == Ladder)
            {
                finalY = newY; // Move up/down the ladder
            }
            else 
            {
                finalX+=last_dir.x;
                dir.y = Stay;
                dir.x = last_dir.x;
            }
        }
    }

    // If position has changed, update it
    if (finalX != x || finalY != y)
    {
        draw_update_Objects_place(finalX, finalY, not_to_draw);
    }

    // Update last direction and free fall counter
    last_dir = dir;
    if (if_mario_in_free_fall)
    {
        sum_floor_in_air++;
    }
    else
    {
        sum_floor_in_air = Reset_Fall;
    }
}
