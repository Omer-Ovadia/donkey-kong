#pragma once
#include "Moving_Objects.h"

class Barrel : public Moving_Objects
{
    // Barrel release position: defines where the barrel is released on the board.
    Location Barrel_Release;

    // Counter for how far the barrel has fallen in air, used to determine explosion.
    int sum_floor_in_air;

    // Function to draw the barrel at its current position.
    void draw(char c) const override;

public:
    Barrel()
        : Moving_Objects(), Barrel_Release(0, 0), sum_floor_in_air(Reset_Fall) {}
    // Default constructor: initializes the barrel at a neutral position.

    Barrel(int _x, int _y)
        : Moving_Objects(_x + Right, _y), Barrel_Release(_x, _y + Down), sum_floor_in_air(Reset_Fall) {}
    // Overloaded constructor: initializes barrel's release position and initial state based on given coordinates.

    Barrel(Location& donkeykong_location)
        : Moving_Objects(donkeykong_location.x + Right, donkeykong_location.y),
        Barrel_Release(donkeykong_location.x, donkeykong_location.y + Down),
        sum_floor_in_air(Reset_Fall) {}
    // Overloaded constructor: places barrel relative to Donkey Kong's location.

    ~Barrel() {}
    // Destructor: cleans up resources (if any).

    // Public function to draw the barrel at its current position.
    void draw() const override 
    {
        draw(Barrel_symbol);
    }

    // Moves the barrel based on its direction, checking for obstacles and platforms.
    void Objects_movement(bool not_to_draw) override;
    void Objects_movement(int Player_Floor,long seed,bool not_to_draw) override
    {
        Objects_movement(not_to_draw);
    }


    // Checks if the barrel has fallen enough to trigger an explosion.
    bool Is_Barrel_Fall_To_Explosion();

    // Checks if the barrel is in the release position (where it was spawned).
    bool Is_In_Release_Place() const;

    // Resets the barrel's position and state.
    void Reset();

    bool isColored = false; // Flag indicating if the barrel should be drawn with color.
};
