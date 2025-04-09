#pragma once // Ensures this header file is included only once during compilation
#include "Moving_Objects.h"

// Class representing the player (Mario) in the Donkey Kong game
class Player: public Moving_Objects // most of the information in this class learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
{
    char mario;           // Character representing Mario
    int sum_floor_in_air;     // Tracks consecutive falls in the air
    bool Is_Holding_Hammer; // index if the player with hammer

    int Get_Jump_Max(int jump_in_dir_x, int jump_y); // Calculates max jump height

    // Private helper methods
    void draw(char c) const override // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
    {
        gotoxy(x, y);       // Moves the cursor to Mario's position
        std::cout << c;     // Draws the character at the specified position
    }
    void draw_update_Objects_place(int finalX, int finalY, bool not_to_draw) override; // Updates Mario's position

public:
    Player(int _x, int _y) : Moving_Objects(_x, _y), mario(Mario_symbol), sum_floor_in_air(Reset_Fall), Is_Holding_Hammer(false) { dir.x = Stay; last_dir.x = Stay; };
    Player(Location& mario_location) : Moving_Objects(mario_location.x, mario_location.y),mario(Mario_symbol), sum_floor_in_air(Reset_Fall), Is_Holding_Hammer(false) { dir.x = Stay; last_dir.x = Stay; };
    ~Player() {};
    // Public methods for interacting with the player
    void draw() const override // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
    {
        draw(mario); // Draws Mario at his current position
    }
    void Objects_movement(bool not_to_draw) override;    // Moves Mario based on input or logic 
    void Objects_movement(int Player_Floor,long seed,bool not_to_draw) override
    {
        Objects_movement(not_to_draw);
    }
    void keyPressed(char key);           // Processes player input // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
    bool Is_mario_Fall_To_Death();       // Checks if Mario has fallen to his death
    bool Is_mario_Win();                 // Checks if Mario has won the game
    bool Get_Is_Holding_Hammer() const {return Is_Holding_Hammer;};
};
