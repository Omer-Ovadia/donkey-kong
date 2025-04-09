#include "SaveMode.h"

// Reset The steps & result files
void SaveMode::Reset_File()
{
    string directoryPath = fs::current_path().string();
    try {
        for (const auto& entry : fs::directory_iterator(directoryPath))
        {
            if (entry.path().extension() == Suffix_Steps || entry.path().extension() == Suffix_Result)
            {
                fs::remove(entry.path());
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
}

// Main game loop
void SaveMode::Run(int& counter_iter, int& counter_act, string& mario_movement,char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_locatio, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost)
{
    bool isColored = this->isGameColored;
    int pace = Regular_Pace;
    system("CLS"); // Clear the screen //this function i found in interntet search in https://www.geeksforgeeks.org/how-to-clear-console-in-cpp/
    ShowConsoleCursor(false); // Hide the console cursor // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
    Board board;
    board.reset(Lives, Score, Legend_location, Board_Read_From_File); // Initialize the board
    bool Not_Print_Hammer = false;
    board.print(isColored, Lives, Score, Legend_location, Not_Print_Hammer, hammer_location); // Print the board

    bool Mario_Is_Died = false; // Track if Mario has died
    Player mario(mario_location);
    VectorOfGhosts ghosts(Max_Y_Board);

    //Handles all types of ghosts
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
    mario.draw(); // Draw Mario on the screen

    char key = Stay; // Default key for staying still

    // Main game logic loop
    while (!Is_Win)
    {
        if (!Mario_Is_Died)
        {
            Mario_Is_Died = Is_Hit(mario, bar, Barrel_Index, ghosts); // Check if Mario is hit
        }

        if (Mario_Is_Died)
        {
            Lives--; // Decrease lives if Mario dies
            Score += Down_Score;
            if (Score < Default_Score)
            {
                Score = Default_Score;
            }
            break;
        }

        if (_kbhit()) // Check for keyboard input // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
        {
            key = _getch();
            if (key == ESC)
            {
                this->Pause_Game(); // Pause the game on ESC
                system("CLS");
                board.print(isColored, Lives, Score, Legend_location, mario.Get_Is_Holding_Hammer(), hammer_location);
                mario.draw();
            }
            mario.keyPressed(key); // Update Mario's movement direction
            if (key != ESC)
            {
                for (size_t i = 0; i < numKeys; i++)
                {
                    if (std::tolower(key) == keys[i]) // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
                    {
                            counter_act++;
                            mario_movement += to_string(counter_iter) + " " + key + "\n";
                    }
                }
            }
            if (mario.Get_Is_Holding_Hammer() && std::tolower(key) == Hammer_symbol)
            {
                Hammer_Destroyed_Enemies(mario, bar, Barrel_Index, ghosts, board, Legend_location);
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
                ghosts[j][i]->Objects_movement(mario.getY(),seed, is_silent);
            }
        }
        Is_Win = mario.Is_mario_Win(); // Check if Mario has won
        counter_iter++;
    }
}

// just for base - empty
void SaveMode::Run(char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_location, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost)
{

}

// Displays the main menu and handles user input
void SaveMode::Menu()
{
    ShowConsoleCursor(false); // Hide the console cursor for a cleaner display
    bool Continue_After_Task; // Flag to track if the menu loop should continue
    // Loop until the user chooses to exit
    while (Menu_Choice != Exit_Menu)
    {
        Is_Win = Not_Won_Yet;   // Reset win state
        Lives = Default_Lives;  // Reset lives to the default value
        Score = Default_Score;  // Reset score to default
        Continue_After_Task = true;
        system("CLS");          // Clear the console screen

        printLogo();            // Display the game logo
        // Display menu options
        std::cout << "\n\n\n                         (1) Start a new game\n"
            << "                         (8) Present instructions and keys\n"
            << "                         (9) EXIT";

        Menu_Choice = Default_Choice_Menu; // Reset menu choice

        // Wait for user input and process it
        while (Continue_After_Task && Menu_Choice != Exit_Menu)
        {
            if (_kbhit()) // Check if a key has been pressed
            {
                Menu_Choice = _getch(); // Get the pressed key

                switch (Menu_Choice)
                {
                case (Show_Instructions): // Display instructions
                    this->Print_Instructions();
                    Continue_After_Task = false;
                    break;

                case (Start_Game): // Start a new game
                    if (Check_If_No_Screens()) // Check if there are no levels available
                    {
                        Continue_After_Task = false;
                        break;
                    }
                    Start_Index_Screen = Menu_Screens(); // Get the starting index for screens
                    auto Place_In_List_Screens = Screens.begin();
                    std::advance(Place_In_List_Screens, Start_Index_Screen); // Move to the selected screen

                    system("CLS"); // Clear the screen
                    printLogo();   // Display the logo again
                    // Display color selection options
                    std::cout << "\n\n\n                         (1) Play without color\n"
                        << "                         (2) Play with color\n";
                    while (true)
                    {
                        if (_kbhit()) // Wait for color selection input
                        {
                            colorChoice = _getch(); // Get the pressed key
                            if (colorChoice == Without_Color) // Play without color
                            {
                                isGameColored = false;
                                break;
                            }
                            else if (colorChoice == With_Color) // Play with color
                            {
                                isGameColored = true;
                                break;
                            }
                        }
                    }
                    Reset_File(); // Reset the file before starting the game
                    // Initialize locations and variables for gameplay
                    Location mario_location, pauline_location, donkeykong_location, hammer_location, Legend_location;
                    VectorOfLocationofGhost ghosts_location(Max_Y_Board);
                    TypeOfGhosts type_of_ghost(Max_Y_Board);
                    char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1];
                    int counter_iter = 0, counter_act = 0, counter_result = 1;
                    string mario_movement = "\n", mario_results;

                    // Iterate through the game screens
                    for (int i = Start_Index_Screen; i < Screens.size(); i++)
                    {
                        if (Lives == Death_Lives) // Exit if the player runs out of lives
                        {
                            Make_Steps_File(*(--Place_In_List_Screens), counter_act, mario_movement);
                            break;
                        }

                        // Load the screen and initialize gameplay
                        if (Place_In_List_Screens != Screens.end() &&Read_The_Screen(*Place_In_List_Screens, Board_Read_From_File, mario_location, pauline_location, donkeykong_location, hammer_location, Legend_location, ghosts_location, type_of_ghost))
                        {
                            mario_results = "";
                            counter_result = 1;
                            Is_Win = Not_Won_Yet;
                            while (Lives != Death_Lives && !Is_Win) // Run the game loop for the current screen
                            {
                                this->Run(counter_iter, counter_act, mario_movement, Board_Read_From_File, mario_location, pauline_location, donkeykong_location, hammer_location, Legend_location, ghosts_location, type_of_ghost);
                                mario_results += to_string(counter_iter) + " " + to_string(Is_Win) + "\n";
                                counter_result++;
                                if (Is_Win) // Display win message if the player wins the level
                                {
                                    Make_Steps_File(*Place_In_List_Screens, counter_act, mario_movement);
                                    counter_iter = counter_act = 0;
                                    mario_movement = "\n";
                                    Score += Plus_Score; // Increase the score
                                    mario_results += to_string(Score);
                                    string fileName = *Place_In_List_Screens;
                                    fileName.erase(fileName.length() - suffix_length);
                                    string fileName_result = fileName + Suffix_Result;
                                    ofstream outFile(fileName_result);
                                    if (outFile.is_open())
                                    {
                                        outFile << to_string(counter_result) << "\n" << mario_results;
                                        outFile.close();
                                    }
                                    system("CLS");
                                    std::cout << "\n\n\n\n\n\n\n\n\n\n                  You Are Amazing, You Won Level:" << *Place_In_List_Screens << "\n"
                                        << "                      Press 9 to continue to the next level\n";
                                    Next_Level(); // Wait for the user to start the next level
                                    break;
                                }
                            }
                        }
                        std::advance(Place_In_List_Screens, 1); // Move to the next screen

                    }

                    if (Is_Win) // Display final message if all levels are won
                    {
                       // Make_Steps_File(*(--Place_In_List_Screens), counter_act, mario_movement);
                        system("CLS");
                        std::cout << "\n\n\n\n\n\n\n\n\n\n                  You Are Amazing, You Won All Levels with Score of:" << Score << "\n"
                            << "                           Press ESC to return the menu\n";
                        Return_After_ESC(); // Wait for ESC key to return to menu
                    }
                    else
                    {
                        //In case you lost the last level available
                        if (Place_In_List_Screens == Screens.end() && Lives == Death_Lives)
                        {
                            Make_Steps_File(*(--Place_In_List_Screens), counter_act, mario_movement);
                        }
                    }
                    Continue_After_Task = false;
                    break;
                }
            }
        }
    }

    // Exit message
    system("CLS");
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n                               Thank you, Bye Bye...\n\n\n\n\n\n\n\n\n" << std::endl;
}

// make the loadin to steps file
void SaveMode::Make_Steps_File(string fileName, int& counter_act, string& mario_movement) const
{
    //Handles the name of the file and writes into it
    fileName.erase(fileName.length() - suffix_length);
    string fileName_steps = fileName + Suffix_Steps;
    ofstream outFile(fileName_steps);
    //in the ordered format
    if (outFile.is_open())
    {
        outFile << isGameColored << "\n" << seed << "\n" << counter_act << mario_movement;
        outFile.close();
    }
}


