#pragma once
#include "GhostManager.h"

class SmartGhost : public GhostManager
{

public:
        SmartGhost(int _x, int _y) : GhostManager(_x, _y) {}
        ~SmartGhost() { GhostManager::draw(' '); }

        // Public function to draw the Ghost at its current position.
        void draw() const override
        {
            GhostManager::draw(Smart_Ghost_symbol);
        }
        // Moves the Ghost based on its direction, checking for obstacles and platforms.
        void Objects_movement(int Player_Floor,long seed,bool not_to_draw) override;

        // Moves the Ghost based on its direction, checking for obstacles and platforms.
        void Objects_movement(bool not_to_draw) override
        {
            Objects_movement(0, 0, not_to_draw);
        }
    };
