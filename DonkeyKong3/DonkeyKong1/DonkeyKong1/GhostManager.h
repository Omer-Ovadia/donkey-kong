#pragma once
#include "Moving_Objects.h"
#include <cstdlib>

class GhostManager : public Moving_Objects
{
protected:
    // Random number to decide the side of the ghost's movement
    int Random_Direction(long seed);

    // Function to draw the Ghost at its current position.
    void draw(char c) const override;

    void draw() const override = 0;

public:
    GhostManager(int _x, int _y) : Moving_Objects(_x, _y) {}
    ~GhostManager() { draw(' '); }

    // Moves the Ghost based on its direction, checking for obstacles and platforms.
    void Objects_movement(bool not_to_draw) override = 0;
    void Objects_movement(int Player_Floor,long seed,bool not_to_draw) override = 0;

    // Returns the next x position of the Ghost.
    int Next_X() const
    {
        return dir.x + x;
    }
    bool isColored = false;
};

