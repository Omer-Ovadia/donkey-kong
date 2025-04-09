#pragma once
#include "Game.h"

class SaveMode : public Game
{
	void Reset_File();
public:
	SaveMode() :Game(false) {}
	~SaveMode() {}
	void Start() override
	{
		Menu();
	}
	void Run(char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_location, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost) override; // Main game loop
	void Run(int& counter_iter, int& counter_act, string& mario_movement,char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_locatio, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost) override;
	void Menu() override;
	void Make_Steps_File(string fileName,int& counter_act, string& mario_movement) const;
};

