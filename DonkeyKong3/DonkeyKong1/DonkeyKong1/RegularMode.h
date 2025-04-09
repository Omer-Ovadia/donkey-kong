#pragma once
#include "Game.h"

class RegularMode : public Game
{
public:
	RegularMode() :Game(false) {}
	~RegularMode() {}
	void Start() override
	{
		Game::Menu();
	}
	void Run(char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_location, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost) override;
	void Run(int& counter_iter, int& counter_act, string& mario_movement, char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_locatio, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost) override {}
};

