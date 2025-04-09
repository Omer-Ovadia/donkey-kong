#pragma once
#include <iostream> // For input/output operations
#include <random>   // for random number to dir of ghost
#include "utils.h"
#include "Board.h"  // Includes the Board class for managing the game board
#include "Constants.h"
#include <filesystem>
#include <cstdlib>
#include <conio.h>

using namespace Constants;

class Moving_Objects 
{

protected:
    // Object's initial position on the board.
    int x, y;

    // Current direction of the Object's movement and last direction.
    Direction dir{ Right, Stay };
    Direction last_dir{ Right, Stay };

    // Pointer to the Board object for interaction with the game board.
    Board* pBoard = nullptr;

    // Stores the character under the Object's current position.
    char Current_Char;

    // Virtual methods for derived classes
    virtual void draw_update_Objects_place(int finalX, int finalY, bool not_to_draw);
    virtual void draw(char c) const = 0;

public:
    // Constructor and Destructor
    Moving_Objects(int _x = 1, int _y = 0) : x(_x), y(_y) {}
    virtual ~Moving_Objects() {};

    // Getters and Setters for x and y
    int getX() const { return x; }
    void setX(int _x) { x = _x; }

    int getY() const { return y; }
    void setY(int _y) { y = _y; }

    // Getters and Setters for direction
    Direction getDirection() const { return dir; }
    void setDirection(Direction _dir) { dir = _dir; }

    // Set and Get Board
    Board* getBoard() const { return pBoard; }
    void setBoard(Board& board);

    // Virtual methods for derived classes
    virtual void draw() const = 0;
    virtual void Objects_movement(bool not_to_draw) = 0;
    virtual void Objects_movement(int Player_Floor, long seed,bool not_to_draw) = 0;

    // Checks if the character is a wall.
    bool Is_Block_By_Wall(char check)const;

    // Utility functions
    bool Is_Block(char check)const ; // Checks if a character represents a block
    bool Is_Platform(char check)const; // Checks if a character represents a platform

};
