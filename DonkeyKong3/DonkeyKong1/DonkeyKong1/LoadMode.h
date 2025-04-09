#pragma once
#include "Game.h"

class LoadMode : public Game
{
protected:
	void Menu() override;
	void Run_If_You_Can_Load();
	list<string> Steps; //List of steps
	list<string> Result; //List of results
	vector <int> iteration;//Stores iterations of steps
	vector <char> key_of_iteration;//Stores keys of steps
	vector <int> iteration_of_Result;//Stores iterations of result
	vector <int> status;//Stores loss/win of result
	int status_score = 0;
	int loop;
	bool If_Steps_File_By_Rule(string name_file, vector <int>& iteration, vector <char>& key_of_iteration);
	bool If_Result_File_By_Rule(string name_file, vector <int>& iteration_of_Result, vector <int>& status);
	bool Is_Same_Screen(string name_file1, string name_file2);
	bool isNumber(string& str);
	bool validateAndParseString(const string& input, int& num, char& c);
	bool validateAndParseString_Result(const string& input, int& num, char& stats);

public:
	LoadMode() :Game(false) {}
	LoadMode(bool is_silent) :Game(is_silent) {}
	~LoadMode() {}
	void Start() override
	{
		Run_If_You_Can_Load();
	}
	void Run(char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_location, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost) override; // Main game loop
	void Run(int& counter_iter, int& counter_act, string& mario_movement, char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_locatio, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost) override {}};

