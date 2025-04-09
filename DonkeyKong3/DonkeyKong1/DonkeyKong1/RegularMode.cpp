#include "RegularMode.h"
void RegularMode::Run(char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_location, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost)
{
	Game::Run(Board_Read_From_File, mario_location, pauline_location, donkeykong_location, hammer_location, Legend_location, ghosts_location, type_of_ghost);
}

