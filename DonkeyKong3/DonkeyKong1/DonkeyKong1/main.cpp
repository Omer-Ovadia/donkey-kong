#include <iostream>
#include <memory>
#include <string>
#include "Game.h"
#include "SaveMode.h"
#include "LoadMode.h"
#include "SilentMode.h"
#include "RegularMode.h"

//Function to check the type of game mode we will play, based on the command arguments
std::unique_ptr<Game> Type_Of_Game(int argc, char** argv)
{
    if (argc == 2 && std::string(argv[1]) == "-save")
    {
        return std::make_unique<SaveMode>();
    }
    if (argc == 2 && std::string(argv[1]) == "-load")
    {
        return std::make_unique<LoadMode>();
    }
    if (argc == 3 && std::string(argv[1]) == "-load" && std::string(argv[2]) == "-silent") 
    {
        return std::make_unique<SilentMode>();
    }
    return std::make_unique<RegularMode>();
}

int main(int argc, char** argv)
{
    //Checks the type of game from the console and plays the fitting game mode
    auto type_of_Game = Type_Of_Game(argc, argv);
    type_of_Game->Start();
    return 0;
}
