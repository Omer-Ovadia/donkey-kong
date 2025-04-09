#pragma once

namespace Constants
{

    // Constants and enums for player controls, movement, and game state:

    static constexpr char keys[] = { 'w', 'a', 'x', 'd', 's','p' };
    // Keys for player controls: 

    static enum directions { Up = -1, Left = -1, Down = 1, Right = 1, Stay = 0 };
    // Movement directions: negative values represent upward/leftward motion, 
    // positive values represent downward/rightward motion, zero indicates no movement

    static enum Wins_State { Win = true, Not_Won_Yet = false };
    // Winning state: 'Win' indicates game won, 'Not_Won_Yet' indicates ongoing game

    static enum Board_Limits { Max_X_Board = 80, Min_X_Board = 0, Max_Y_Board = 25, Min_Y_Board = 0 };
    // Defines the maximum and minimum coordinates of the game board

    static constexpr size_t numKeys = sizeof(keys) / sizeof(keys[0]);
    // Calculates the number of player control keys

    // All game objects
    static enum Objects {
        Ladder = 'H',
        Empty_Space = ' ',
        Right_Floor = '>',
        Left_Floor = '<',
        Barrel_symbol = 'O',
        Mario_symbol = '@',
        Pauline_symbol = '$',
        DonkeyKong_symbol = '&',
        Ghost_symbol = 'x',
        Smart_Ghost_symbol = 'X',
        Hammer_symbol = 'p', 
        Mario_With_Hammer_Symbol = 'M',
        Wall_Symbol = 'Q',
        Legend_Symbol = 'L'
    };

    // Explosion limits: defines critical fall distances for barrel and Mario
    static enum Explosion_Limits {
        Reset_Fall = 0,
        Max_Fall_Barrel = 8,
        Max_Fall_Mario = 5
    };

    // Rules for the indexes used in file names
    static enum File_Name_Rule {
        First_Index_On_File_Name = 0,
        Min_Length_Board_Name = 15,
        suffix_length=7
    };

    // Enumerations for key game rules and state
    static enum Orders { ESC = 27 };
    // Escape key for exiting or pausing

    static enum Lives_Rules { Default_Lives = 3, Death_Lives = 0, Life_Lost = -1 };
    // Lives management: initial lives, death condition, and loss per event

    static enum Score_Rules { Default_Score = 0, Down_Score = -10, Plus_Score = 10, Default = 0};
    // Scoring system: default score, penalty for certain actions, and bonus for others

    static enum Menu_Rules {
        Start_Game = '1',
        Show_Instructions = '8',
        Exit_Menu = '9',
        Default_Choice_Menu = '0',
        Back = '8',
        Next = '9'
    };
    // Menu options and default behavior

    static enum Indexes_Of_Barrels {
        Max_Barrel = 5,
        Reset_Index_Barrel = 0,
        Reset_Index_Barrel_After_Exp = -1
    };

    static enum Counter_Index_Symbol {
        mario_index = 0,
        pauline_index = 1,
        donkey_kong_index = 2,
        hammer_index = 3,
        legend_index = 4
    };

    static enum Ghost_Type_Index {
        Regular_Ghost = 0,
        Smart_Ghost = 1
    };

    static enum ColorPick {
        Without_Color = '1',
        With_Color = '2'
    };

    static enum Pace {
        Regular_Pace = 80,
        Load_pace = 40,
        Silent_pace = 0
    };

    static const int Size_Of_Symbol = 5;
    static const int next_level = 1;
    static const int Lines_Of_Logo = 8;

    // Total number of key game symbols defined in Counter_Index_Symbol

    static constexpr const char* Prefix = "dkong_";
    static constexpr const char* Suffix = ".screen";
    static constexpr const char* Suffix_Steps = ".steps";
    static constexpr const char* Suffix_Result = ".result";
    // Prefix and suffix for file naming convention

    struct Location
    {
        int x, y;
        Location() {};
        Location(int _x, int _y) : x(_x), y(_y) {};
    };

    // Structure to represent directions using x and y coordinates.
    struct Direction { int x, y; };

    // Array of directions for movement (Up, Down, Left, Right, Stay).
    static constexpr Direction directions[] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0}, {0, 0} };
}
