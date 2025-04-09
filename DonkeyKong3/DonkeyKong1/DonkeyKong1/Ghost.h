#pragma once
#include "GhostManager.h"

class Ghost : public GhostManager
{
    long local_seed;
public:
    Ghost(int _x, int _y) : GhostManager(_x, _y) {}
    ~Ghost() { GhostManager::draw(' '); }

    // Public function to draw the Ghost at its current position.
    void draw() const override
    {
        GhostManager::draw(Ghost_symbol);
    }

    // Moves the Ghost based on its direction, checking for obstacles and platforms.
    void Objects_movement(bool not_to_draw) override;
    void Objects_movement(int Player_Floor,long seed, bool not_to_draw) override
    {
        local_seed = seed;
        Objects_movement(not_to_draw);
    }
    long GetSeed()
    {
        return local_seed;
    }
};
