#include "LoadMode.h"

void LoadMode::Run_If_You_Can_Load()
{
    string name_file;
    const char* suf_step = Suffix_Steps;
    string directoryPath = fs::current_path().string();
    try 
    {
        for (const auto& entry : fs::directory_iterator(directoryPath))
        {
            if (entry.path().extension() == Suffix_Steps)
            {
                 name_file = entry.path().filename().string(); // Extract the file name
                 // Validate the file name format before adding it to the list of screens
                  if (Is_File_Name_In_Format(name_file, Suffix_Steps, Min_Length_Board_Name-1))
                  {
                      Steps.push_back(name_file); // Add the valid file name to the Screens list
                  }
            }
            else if (entry.path().extension() == Suffix_Result)
            {
                name_file = entry.path().filename().string(); // Extract the file name
                // Validate the file name format before adding it to the list of screens
                if (Is_File_Name_In_Format(name_file, Suffix_Result, Min_Length_Board_Name))
                {
                    Result.push_back(name_file); // Add the valid file name to the Screens list
                }
            }
        }
    }
    catch (const fs::filesystem_error& e) // Catch and handle any filesystem-related errors (from ChatGpt)
    {
        system("CLS"); // Clear the console screen
        // Display an error message to inform the user of the issue
        std::cout << "\n\n\n\n\n\n\n                      You Have a problem with the files:\n\n\n"
            << "Error: " << e.what() << "\n\n"
            << "                    Press ESC to return and see the name of file\n";

        Return_After_ESC(); // Wait for the user to press ESC to return to the menu
    }
    Steps.sort(); // Sort the list of screen names alphabetically
    Result.sort(); // Sort the list of screen names alphabetically
    if (Steps.empty())
    {
        system("CLS"); // Clear the console screen
        std::cout << "\n\n\n\n\n\n\n\n\n                You Have a problem, There are no steps file available\n"
            << "                        Press ESC to finish\n";
        Return_After_ESC(); // Wait for the user to press ESC
        return;            // Indicate that there are no screens
    }
    Menu();
}
void LoadMode::Menu()
{
    ShowConsoleCursor(false); // Hide the console cursor for a cleaner display
    bool Continue_After_Task; // Flag to track if the menu loop should continue

    Is_Win = Not_Won_Yet;   // Reset win state
    Lives = Default_Lives;  // Reset lives to the default value
    Score = Default_Score;  // Reset score to the default value
    system("CLS");          // Clear the console screen

    if (Check_If_No_Screens()) // Exit if no screens are available
    {
        return;
    }

    // Initialize iterators for screens, steps, and result lists
    auto Place_In_List_Screens = Screens.begin();
    auto Place_In_List_Steps = Steps.begin();
    auto Place_In_List_Result = Result.begin();

    // Variables to store gameplay data
    Location mario_location, pauline_location, donkeykong_location, hammer_location, Legend_location;
    VectorOfLocationofGhost ghosts_location(Max_Y_Board);
    TypeOfGhosts type_of_ghost(Max_Y_Board);
    char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1];

    // Iterate through all screens
    for (int i = 0; i < Screens.size(); i++)
    {
        if (Lives == Death_Lives) // Stop if lives are exhausted
        {
            system("CLS");
            std::cout << "\n\n\n\n\n\n\n\n\n\n                       You finished watching the recording\n"
                << "                               Press ESC to Exit\n\n\n\n\n\n\n\n\n\n";
            Return_After_ESC();
            break;
        }

        // Check if the current screen matches the steps file
        if (!Is_Same_Screen(*Place_In_List_Screens, *Place_In_List_Steps))
        {
            system("CLS");
            std::cout << "\n\n\n\n\n\n\n\n\n\n             This screen:" << *Place_In_List_Screens << " doesn't have a fitting steps file\n"
                << "                       Press 9 to continue to the next level\n";
            Next_Level(); // Move to the next level
            if (next(Place_In_List_Screens) != Screens.end())
            {
                std::advance(Place_In_List_Screens, 1); // Advance to the next screen
            }
        }
        else
        {
            // Check if result file exists and matches the steps file
            if (Result.empty() || !Is_Same_Screen(*Place_In_List_Result, *Place_In_List_Steps))
            {
                system("CLS");
                if (is_silent) // Silent mode: skip result validation
                {
                    std::cout << "\n\n\n\n\n\n\n\n\n\n             This screen:" << *Place_In_List_Screens << " doesn't have a fitting result file\n"
                        << "                            We can't check the result\n\n"
                        << "                                 Press 9 to Exit\n\n\n\n\n\n\n\n\n";
                    Next_Level();
                    Is_Win = Not_Won_Yet;
                    break;
                }
                else // Allow continuing without checking the result
                {
                    std::cout << "\n\n\n\n\n\n\n\n\n\n             This screen:" << *Place_In_List_Screens << " doesn't have a fitting result file\n"
                        << "            Press 9 to continue loading the level without checking the result\n";
                    Next_Level();
                }
            }

            // Read the current screen and initialize gameplay
            if (Read_The_Screen(*Place_In_List_Screens, Board_Read_From_File, mario_location, pauline_location, donkeykong_location, hammer_location, Legend_location, ghosts_location, type_of_ghost))
            {
                loop = Default;
                iteration.clear();
                key_of_iteration.clear();
                If_Steps_File_By_Rule(*Place_In_List_Steps, iteration, key_of_iteration); // Parse the steps file
                iteration_of_Result.clear();
                status.clear();
                if (!Result.empty())
                {
                    If_Result_File_By_Rule(*Place_In_List_Result, iteration_of_Result, status); // Parse the result file
                }
                Is_Win = Not_Won_Yet;

                // Run the game loop
                while (Lives != Death_Lives && !Is_Win)
                {
                    this->Run(Board_Read_From_File, mario_location, pauline_location, donkeykong_location, hammer_location, Legend_location, ghosts_location, type_of_ghost);
                    if (Is_Win) // Display win message if the player wins the level
                    {
                        Score += Plus_Score; // Increment the score
                        if (is_silent && status_score != Score) // Result mismatch in silent mode
                        {
                            std::cout << "\n\n\n\n\n\n          You failed the result test, your score is: " << status_score << " but should be: " << Score << "\n"
                                << "                  Press 9 to continue loading the next level \n";
                            Next_Level();
                        }
                        else if (is_silent && status_score == Score) // Result match in silent mode
                        {
                            std::cout << "\n\n                   You passed the result test, your score is: " << status_score << "\n"
                                << "                     Press 9 to continue loading the next level\n";
                            Next_Level();
                        }
                        system("CLS");
                        if (next(Place_In_List_Result) != Result.end())
                        {
                            std::advance(Place_In_List_Result, next_level); // Move to the next result
                        }
                        else
                        {
                            if (is_silent) // End the game in silent mode if out of results
                            {
                                Lives == Death_Lives;
                                break;
                            }
                        }
                        break;
                    }
                }
            }

            // Advance to the next screen and steps
            if (next(Place_In_List_Screens) != Screens.end())
            {
                std::advance(Place_In_List_Screens, 1);
            }
            if (next(Place_In_List_Steps) != Steps.end())
            {
                std::advance(Place_In_List_Steps, 1);
            }
            else
            {
                break;
            }
        }
    }

    // Handle the end of the game
    if (Is_Win)
    {
        system("CLS");
        if (!is_silent) // Display the success message
        {
            std::cout << "\n\n\n\n\n\n\n\n\n\n                  You Are Amazing, You Won All Levels with Score of:" << Score << "\n"
                << "                       You finished watching the recording\n"
                << "                               Press ESC to Exit\n\n\n\n\n\n\n\n\n\n";
        }
        else // Display the success message in silent mode
        {
            std::cout << "\n\n\n\n\n\n\n\n\n\n                  You Are Amazing, You Won and passed all the tests\n"
                << "                               Press ESC to Exit\n\n\n\n\n\n\n\n\n\n";
        }
        Return_After_ESC();
        return;
    }

    if (!is_silent) // Display final message if not in silent mode
    {
        system("CLS");
        std::cout << "\n\n\n\n\n\n\n\n\n\n                       You finished watching the recording\n"
            << "                               Press ESC to Exit\n\n\n\n\n\n\n\n\n\n";
        Return_After_ESC();
    }
}
void LoadMode::Run(char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_locatio, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost)
{
    bool isColored = this->isGameColored;
    int pace = Load_pace;
    if (is_silent)
    {
        pace = Silent_pace;
    }
    system("CLS"); // Clear the screen //this function i found in interntet search in https://www.geeksforgeeks.org/how-to-clear-console-in-cpp/
    ShowConsoleCursor(false); // Hide the console cursor // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
    Board board;
    board.reset(Lives, Score, Legend_location, Board_Read_From_File); // Initialize the board
    bool Not_Print_Hammer = false;
    if (!is_silent)
    {
        board.print(isColored, Lives, Score, Legend_location, Not_Print_Hammer, hammer_location); // Print the board
    }
    bool Mario_Is_Died = false; // Track if Mario has died
    Player mario(mario_location);
    VectorOfGhosts ghosts(Max_Y_Board);

    for (int y = 0; y < Max_Y_Board; ++y)
    {
        for (int i = 0; i < ghosts_location[y].size(); ++i)
        {
            Location* loc = ghosts_location[y][i];
            if (loc)
            {
                if (*type_of_ghost[y][i] == Regular_Ghost)
                {
                    ghosts[loc->y].push_back(new Ghost(loc->x, loc->y));
                }
                else
                {
                    ghosts[loc->y].push_back(new SmartGhost(loc->x, loc->y));

                }
            }
        }
    }

    for (const auto& row : ghosts)
    {
        for (const auto& ghost : row)
        {
            if (ghost)
            {
                ghost->setBoard(board);// Initialize ghosts with the board
                ghost->isColored = isColored; // Initialize ghosts with the board
            }
        }
    }


    int Barrel_Index = Reset_Index_Barrel, Barrel_Index_After_Exp = Reset_Index_Barrel_After_Exp, Next_Barrel_Ready_To_Go = Reset_Index_Barrel;
    Barrel bar[Max_Barrel];
    for (int i = 0; i < Max_Barrel; i++)
    {
        bar[i] = Barrel(donkeykong_location);
        bar[i].setBoard(board); // Initialize barrels with the board
        bar[i].isColored = isColored; // Initialize barrels with the board
    }

    mario.setBoard(board); // Set the board for Mario
    if (!is_silent)
    {
        mario.draw(); // Draw Mario on the screen
    }
    // Main game logic loop
    while (!Is_Win)
    {
        if (!Mario_Is_Died)
        {
            Mario_Is_Died = Is_Hit(mario, bar, Barrel_Index, ghosts); // Check if Mario is hit
        }
        if (Mario_Is_Died)
        {
            if (is_silent)
            {
                if (loop != iteration_of_Result.front())
                {
                    if (status.front() == 1)
                    {
                        system("CLS");
                        std::cout << "\n\n\n\n\n\n\n\n\n\n              You died here in: "<< loop <<" but won in the reuslt file in: "<< iteration_of_Result.front() <<"\n"
                            << "                                Press ESC to Exit\n\n\n\n\n\n\n\n\n\n";
                        Return_After_ESC();
                        Lives = Death_Lives;
                    }
                    else
                    {
                        system("CLS");
                        std::cout << "\n\n\n\n\n\n\n\n\n\n         You died in the wrong iteration: " << loop << " instead of: " << iteration_of_Result.front() << " (from file)" << "\n"
                            << "                               Press ESC to Exit\n\n\n\n\n\n\n\n\n\n";

                        Return_After_ESC();
                        Lives = Death_Lives;
                    }
                    
                }
                else if (status.front() == 1)
                {
                    system("CLS");
                    std::cout << "\n\n\n\n\n\n\n\n\n\n               You died in iteretion: "<<loop <<", but files said you won here\n"
                        << "                               Press ESC to Exit\n\n\n\n\n\n\n\n\n\n";
                    Return_After_ESC();
                    Lives = Death_Lives;
                }
                if (!iteration_of_Result.empty())
                {
                    iteration_of_Result.erase(iteration_of_Result.begin());
                    status.erase(status.begin());
                }
            }
            Lives--; // Decrease lives if Mario dies
            Score += Down_Score;
            if (Score < Default_Score)
            {
                Score = Default_Score;
            }
            break;
        }
        if (!iteration.empty() && loop == iteration.front())
        {
            mario.keyPressed(key_of_iteration.front()); // Update Mario's movement direction
            if (mario.Get_Is_Holding_Hammer() && std::tolower(key_of_iteration.front()) == Hammer_symbol)
            {
                Hammer_Destroyed_Enemies(mario, bar, Barrel_Index, ghosts, board, Legend_location);
            }
            if (!iteration.empty())
            {
                iteration.erase(iteration.begin());
                key_of_iteration.erase(key_of_iteration.begin());
            }
        }

        Sleep(pace); // Delay for smoother gameplay // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
        mario.Objects_movement(is_silent); // Move Mario based on current direction
        if (!Mario_Is_Died)
        {
            Mario_Is_Died = Is_Hit(mario, bar, Barrel_Index, ghosts); // Check if Mario is hit
        }
        // Barrel logic
        if (bar[Next_Barrel_Ready_To_Go].Is_In_Release_Place())
        {
            Next_Barrel_Ready_To_Go = (Next_Barrel_Ready_To_Go + 1) % Max_Barrel;
            if (Barrel_Index < Max_Barrel - 1)
            {
                Barrel_Index++;
            }
        }

        // Barrel movements and explosions
        for (int i = 0; i <= Barrel_Index; i++)
        {
            if (bar[i].Is_Barrel_Fall_To_Explosion())
            {
                Mario_Is_Died = Is_Mario_In_Exp_Radius(mario.getX(), mario.getY(), bar[i].getX(), bar[i].getY());
                bar[i].Reset();
                Barrel_Index_After_Exp = i;
            }

            if ((Barrel_Index_After_Exp != i))
            {
                bar[i].Objects_movement(is_silent);
            }

            if (Barrel_Index_After_Exp == i && Barrel_Index_After_Exp == Next_Barrel_Ready_To_Go)
            {
                Barrel_Index_After_Exp = Reset_Index_Barrel_After_Exp;
                bar[i].Objects_movement(is_silent);
            }
        }
        for (int j = 0; j < 25; j++)
        {
            for (int i = 0; i < ghosts[j].size(); i++)
            {
                for (int m = i + 1; m < ghosts[j].size(); m++)
                {
                    if (ghosts[j][i]->Next_X() == ghosts[j][m]->getX() && ghosts[j][m]->Next_X() == ghosts[j][i]->getX())
                    {
                        Direction newDir = ghosts[j][i]->getDirection(); // Get the current direction
                        newDir.x *= -1;                                  // Modify the x direction
                        ghosts[j][i]->setDirection(newDir);              // Update the direction in the ghost

                        newDir = ghosts[j][m]->getDirection(); // Get the current direction
                        newDir.x *= -1;                                  // Modify the x direction
                        ghosts[j][m]->setDirection(newDir);              // Update the direction in the ghost

                    }
                }
                ghosts[j][i]->Objects_movement(mario.getY(), seed, is_silent);
            }
        }
        Is_Win = mario.Is_mario_Win(); // Check if Mario has won
        if (Is_Win && is_silent)
        {
                if (loop != iteration_of_Result.front()-1)
                {
                    system("CLS");
                    std::cout << "\n\n\n\n\n\n\n\n\n\n            You won in wrong iteration: "<< loop + 1 <<" instead of: " << iteration_of_Result.front()<<" (from file)" << "\n"
                        << "                               Press ESC to Exit\n\n\n\n\n\n\n\n\n\n";
                    if (!iteration_of_Result.empty())
                    {
                        iteration_of_Result.erase(iteration_of_Result.begin());
                        status.erase(status.begin());
                    }
                    Return_After_ESC();
                }
                else
                {
                    if (status.front() == 0)
                    {
                        system("CLS");
                        std::cout << "\n\n\n\n\n\n\n\n\n\n              You won in iteretion: " << loop + 1 << " but files said you died here\n"
                            << "                             Press ESC to Exit\n\n\n\n\n\n\n\n\n\n";
                        if (!iteration_of_Result.empty())
                        {
                            iteration_of_Result.erase(iteration_of_Result.begin());
                            status.erase(status.begin());
                        }
                        Return_After_ESC();
                    }
                    
                }
        }
        loop++;
    }
}
//Checks for screen prefix in 2 different files
bool LoadMode::Is_Same_Screen(string name_file1, string name_file2)
{
    name_file1.erase(name_file1.size() - strlen(Suffix), name_file1.size()-1);
    name_file2.erase(name_file2.size() - strlen(Suffix_Steps), name_file2.size()-1);
    return (name_file1 == name_file2);
}
//Checks if the steps file is fitting the format
bool LoadMode::If_Steps_File_By_Rule(string name_file, vector <int>& iteration, vector <char>& key_of_iteration)
{
    ifstream file(name_file); // Open the file for reading
    string line; // Stores each line from the screen file
    std::getline(file, line);
    if (line != "1" && line != "0")
    {
        system("CLS"); // Clear the console screen
        std::cout << "\n\n\n\n\n\n\n                      Error in Color Choice: " << name_file
            << "\n\n                       Press 9 to continue to the next level\n";
        Next_Level();
        return false;
    }
    isGameColored = stoi(line);
    std::getline(file, line);
    if (line.size() < 1 || !isNumber(line))
    {
        system("CLS"); // Clear the console screen
        std::cout << "\n\n\n\n\n\n\n                      Error in seed number: " << name_file
            << "\n\n                       Press 9 to continue to the next level\n";
        Next_Level();
        return false;
    }
    seed = stoi(line);
    std::getline(file, line);
    int itaration;
    if (line.size() < 1 || !isNumber(line))
    {
        system("CLS"); // Clear the console screen
        std::cout << "\n\n\n\n\n\n\n                      Error in number of iteration: " << name_file
            << "\n\n                       Press 9 to continue to the next level\n";
        Next_Level();
        return false;
    }
    itaration = stoi(line);
    if (itaration < 0)
    {
        system("CLS"); // Clear the console screen
        std::cout << "\n\n\n\n\n\n\n                      Error in number of iteration: " << name_file
            << "\n\n                       Press 9 to continue to the next level\n";
        Next_Level();
        return false;
    }
    int num;
    char c;
    while (std::getline(file, line))
    {
        if (!validateAndParseString(line, num, c))
        {
            system("CLS"); // Clear the console screen
            std::cout << "\n\n\n\n\n\n\n     Error in act - iteration: " << iteration.size() << " not by the rules in file: " << name_file
                << "\n\n                       Press 9 to continue to the next level\n";
            Next_Level();
            return false;
        }
        else
        {
            iteration.push_back(num);
            key_of_iteration.push_back(c);
        }
    }
    if (iteration.size() != itaration)
    {
        system("CLS"); // Clear the console screen
        std::cout << "\n\n\n\n\n\n\n           number of iteration doesnt match in file: " << name_file
            << "\n\n                       Press 9 to continue to the next level\n";
        Next_Level();
        return false;
    }
    file.close(); // Close the file
}
//Checks if the result file is fitting the format
bool LoadMode::If_Result_File_By_Rule(string name_file, vector <int>& iteration_of_Result, vector <int>& status)
{
    ifstream file(name_file); // Open the file for reading
    string line; // Stores each line from the screen file
    std::getline(file, line);
    int itaration;
    if (line.size() < 1 || !isNumber(line))
    {
        system("CLS"); // Clear the console screen
        std::cout << "\n\n\n\n\n\n\n                      Error in number of iteration: " << name_file
            << "\n\n                       Press 9 to continue to the next level\n";
        Next_Level();
        return false;
    }
    itaration = stoi(line);
    if (itaration < 0)
    {
        system("CLS"); // Clear the console screen
        std::cout << "\n\n\n\n\n\n\n                      Error in number of iteration: " << name_file
            << "\n\n                       Press 9 to continue to the next level\n";
        Next_Level();
        return false;
    }
    int iter;
    char stats;
    for(int i=0;i<itaration-1;i++)
    {
        std::getline(file, line);
        if (!validateAndParseString_Result(line, iter, stats))
        {
            system("CLS"); // Clear the console screen
            std::cout << "\n\n\n\n\n\n\n     Error in act - iteration: "<< i << " not by the rules in file: " << name_file
                << "\n\n                       Press 9 to continue to the next level\n";
            Next_Level();
            return false;
        }
        else
        {
            iteration_of_Result.push_back(iter);
            status.push_back(stats - '0');
        }
    }
    if (iteration_of_Result.size() != itaration-1)
    {
        system("CLS"); // Clear the console screen
        std::cout << "\n\n\n\n\n\n\n           number of iteration doesnt match in file: " << name_file
            << "\n\n                       Press 9 to continue to the next level\n";
        Next_Level();
        return false;
    }
    std::getline(file, line);
    if (line.size() < 1 || !isNumber(line))
    {
        system("CLS"); // Clear the console screen
        std::cout << "\n\n\n\n\n\n\n                      Error in Score value: " << name_file
            << "\n\n                       Press 9 to continue to the next level\n";
        Next_Level();
        return false;
    }
    status_score = stoi(line);
    if (status_score < 0)
    {
        system("CLS"); // Clear the console screen
        std::cout << "\n\n\n\n\n\n\n                      Error in Score value: " << name_file
            << "\n\n                       Press 9 to continue to the next level\n";
        Next_Level();
        return false;
    }

        file.close(); // Close the file
}
//Checks if string has only digits
bool LoadMode::isNumber(string& str)
{
    // Check if the string is empty
    if (str.empty())
    {
        return false;
    }

    // Check if the string starts with a minus sign
    size_t startIndex = 0;
    if (str[0] == '-')
    {
        // If the string is only "-" (e.g., no digits after), it's not a valid number
        if (str.size() == 1)
        {
            return false;
        }
        startIndex = 1; // Start checking from the next character
    }

    // Check if all characters starting from startIndex are digits
    return std::all_of(str.begin() + startIndex, str.end(), [](char c)
        {
            return std::isdigit(static_cast<unsigned char>(c));
        });
}
bool LoadMode::validateAndParseString(const string& input, int& num, char& c)
{
    num = 0; // Initialize number output to 0
    c = '0'; // Initialize character output to default '0'

    // Input must be at least 3 characters to be valid (e.g., "1 a")
    if (input.size() < 3)
    {
        return false;
    }

    // Find the position of the first space in the input
    size_t spaceIndex = input.find(' ');
    // Validate space position: it must exist and not be at the beginning or end
    if (spaceIndex == string::npos || spaceIndex == 0 || spaceIndex == input.size() - 1)
    {
        return false;
    }

    // Split the input into number and character parts
    string numberPart = input.substr(0, spaceIndex); // Extract the numeric portion
    string charPart = input.substr(spaceIndex + 1); // Extract the character portion

    // Validate the character part: it must be exactly 1 character and within the allowed set
    if (charPart.size() != 1 || std::string("asdwxp").find(charPart[0]) == std::string::npos)
    {
        return false;
    }

    // Convert the number part to an integer and validate it
    try
    {
        num = std::stoi(numberPart); // Attempt to convert the numeric string to an integer
        if (num < 0) // Number must be non-negative
        {
            return false;
        }
    }
    catch (...) // Catch any exceptions during conversion
    {
        return false;
    }

    c = charPart[0]; // Assign the validated character
    return true; // Input is valid
}
bool LoadMode::validateAndParseString_Result(const string& input, int& num, char& stats)
{
    num = 0;
    stats = '0';

    // Ensure the input has at least 3 characters (e.g., "1 0")
    if (input.size() < 3)
    {
        return false;
    }

    // Find the space separating the number and the character
    size_t spaceIndex = input.find(' ');
    if (spaceIndex == string::npos || spaceIndex == 0 || spaceIndex == input.size() - 1) {
        return false; // No space, or it's at the beginning or end
    }

    // Extract the number part and the character part
    string numberPart = input.substr(0, spaceIndex);
    string charPart = input.substr(spaceIndex + 1);

    // Ensure the character part has exactly one character and is either '0' or '1'
    if (charPart.size() != 1 || (charPart[0] != '0' && charPart[0] != '1')) 
    {
        return false;
    }

    try {
        // Convert the number part to an integer
        num = std::stoi(numberPart);
        if (num < 0)
        {
            return false; // Ensure the number is non-negative
        }
    }
    catch (...) 
    {
        return false; // Conversion failed
    }

    // Update stats with the parsed character
    stats = charPart[0];
    return true; // Input is valid
}
