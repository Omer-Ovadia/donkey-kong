#pragma once // Ensures this header file is included only once during compilation

#include <iostream>   // For standard input/output operations
#include <Windows.h>  // For Windows API functions
#include <conio.h>    // For keyboard input functions like _kbhit() and _getch()
#include <cstdlib>    // For system() function
#include <vector>      // For vector data structure
#include <fstream>       // Used for reading screen files and game data from text files.
#include <filesystem>    // Used to iterate through the "Screens" directory and validate file names.
#include <string>        // Used for handling file names, input data, and messages displayed to the player.
#include <list>          // Used to store and manage the list of available screens for the game.
#include <algorithm>     // Used for operations like sorting the list of screens and managing containers.
#include "utils.h"    
#include "Board.h"    
#include "Barrel.h"   
#include "Player.h"
#include "GhostManager.h"
#include "SmartGhost.h"
#include "Ghost.h"
#include "Constants.h"
#include "Moving_Objects.h"

// Typedef for vector of ghosts
using VectorOfGhosts = std::vector<std::vector<GhostManager*>>;
using TypeOfGhosts = std::vector<std::vector<int*>>;
using VectorOfLocationofGhost = std::vector<std::vector<Location*>>;
namespace fs = std::filesystem;
using namespace Constants;
using namespace std;

class Game
{
protected:
    VectorOfGhosts ghosts;
    TypeOfGhosts type_of_ghost;
    VectorOfLocationofGhost ghosts_location;
    // Logo art displayed in the menu
    const char* logo[Max_X_Board] =
    {
          "                                                                                ",
          "                                                                                ",
          "                                                                                ",
          "                  ___           _              _  __                            ",
          "                 |   \\ ___ _ _ | |_____ _  _  | |/ /___ _ _  __ _              ",
          "                 | |) / _ \\ ' \\| / / -_) || | | ' </ _ \\ ' \\/ _` |          ",
          "                 |___/\\___/_||_|_\\_\\___|\\_, | |_|\\_\\___/_||_\\__, |       ",
          "                                        |__/                |___/               ",
    };

   // Attributes for game state
    int Lives = Default_Lives;     // Tracks remaining lives
    int Score = Default_Score;     // Tracks current score
    char Menu_Choice;              // Stores the player's menu selection
    char colorChoice;              // Stores the player's color selection
    bool Is_Win = Not_Won_Yet;     // Tracks if the player has won the game
    list<string> Screens;          // List of available game screens
    int Start_Index_Screen;        // Current screen index
    bool Read_Screen = false;      // Indicates if screens have been read
    long seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());

    // Private member functions
    int Menu_Screens();
    bool Check_If_No_Screens();
    void Number_Of_File();
    bool Is_File_Name_In_Format(const string& name_file, const char* sufix, int min_length);
    void Print_Instructions() const; // Displays game instructions
    virtual void Run(char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_location, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost); // Main game loop
    virtual void Run(int& counter_iter, int& counter_act, string& mario_movement, char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_locatio, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost)=0;
    void Pause_Game() const;         // Pauses the game
    bool Is_Hit(Player& Mario, Barrel barim[], int Size_Barrel, const VectorOfGhosts& ghosts); // Checks if Mario is hit by a barrel
    bool Is_Mario_In_Exp_Radius(int mario_x, int mario_y, int barrel_x, int barrel_y); // Checks if Mario is within the explosion radius
    void Return_After_ESC() const;  // Handles game state after the ESC key is pressed
    void printLogo() const;         // Prints the logo in the menu
    void Hammer_Destroyed_Enemies(Player& Mario, Barrel barim[], int Size_Barrel, VectorOfGhosts& ghosts, Board& board, Location& Legend_location);
    void Next_Level() const;        // Advances to the next level
    bool Read_The_Screen(string& name_board, char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_location, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost);
    bool Is_Platform(char check) { return (check == '>' || check == '<' || check == '='); } // Checks if a character represents a platform
    bool is_silent;
public:
    Game(bool _is_silent = false): is_silent(_is_silent) {}
    ~Game() {
        for (auto& row : ghosts) {
            for (auto& ghost : row) {
                delete ghost;  // Free each allocated Ghost
            }
        }
        for (auto& row : type_of_ghost) {
            for (auto& ghostType : row) {
                delete ghostType;  // Free each allocated int
            }
        }

        for (auto& row : ghosts_location) {
            for (auto& loc : row) {
                delete loc;  // Free each allocated Location
            }
        }
    }
    virtual void Start() = 0;
    virtual void Menu();                    // Displays the game menu
    bool isGameColored = false;      // Tracks if the player has chosen to play with color
};
