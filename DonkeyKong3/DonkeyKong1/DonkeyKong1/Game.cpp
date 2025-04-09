#include "Game.h"

// Displays the main menu and handles user input
void Game::Menu() 
{
    ShowConsoleCursor(false); // Hide the console cursor for a cleaner display
    bool Continue_After_Task; // Flag to track if the menu loop should continue
    // Loop until the user chooses to exit
    while (Menu_Choice != Exit_Menu)
    {
        Is_Win = Not_Won_Yet;   // Reset win state
        Lives = Default_Lives;  // Reset lives to the default value
        Score = Default_Score;
        Continue_After_Task = true;
        system("CLS");          // Clear the console screen

        printLogo();
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
                    if (Check_If_No_Screens())
                    {
                        Continue_After_Task = false;
                        break;
                    }
                    Start_Index_Screen = Menu_Screens();
                    auto Place_In_List_Screens = Screens.begin();
                    std::advance(Place_In_List_Screens, Start_Index_Screen);

                    system("CLS"); // Clear the screen
                    printLogo();
                    // Display color selection options
                    std::cout << "\n\n\n                         (1) Play without color\n"
                        << "                         (2) Play with color\n";
                    while (true)
                    {

                        if (_kbhit()) // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
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
                    Location mario_location,pauline_location,donkeykong_location,hammer_location,Legend_location;
                    VectorOfLocationofGhost ghosts_location(Max_Y_Board);
                    TypeOfGhosts type_of_ghost(Max_Y_Board);
                    char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1];
                        for (int i = Start_Index_Screen; i < Screens.size(); i++)
                        {
                            if (Lives == Death_Lives)
                            {
                                break;
                            }
                            if (Read_The_Screen(*Place_In_List_Screens, Board_Read_From_File, mario_location, pauline_location, donkeykong_location, hammer_location, Legend_location, ghosts_location, type_of_ghost))
                            {
                                    Is_Win = Not_Won_Yet;
                                    while (Lives != Death_Lives && !Is_Win)
                                    {
                                        this->Run(Board_Read_From_File, mario_location, pauline_location, donkeykong_location, hammer_location, Legend_location, ghosts_location, type_of_ghost);
                                        if (Is_Win) // Display win message if the player wins the level
                                        {
                                            Score += Plus_Score;
                                            system("CLS");
                                            std::cout << "\n\n\n\n\n\n\n\n\n\n                  You Are Amazing, You Won Level:" << *Place_In_List_Screens << "\n"
                                                << "                      Press 9 to continue to the next level\n";
                                            Next_Level();
                                            break;
                                        }
                                    }
                            }
                            std::advance(Place_In_List_Screens, next_level);
                        }
                        if (Is_Win)
                        {
                            system("CLS");
                            std::cout << "\n\n\n\n\n\n\n\n\n\n                  You Are Amazing, You Won All Levels with Score of:" << Score << "\n"
                                << "                           Press ESC to return the menu\n";
                            Return_After_ESC();
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

// Displays game instructions
void Game::Print_Instructions() const
{
    system("CLS"); // Clear the console screen //this function i found in interntet search in https://www.geeksforgeeks.org/how-to-clear-console-in-cpp/
    std::cout << "Guide Mario (@) through the construction site\n"
        << "climbing floors (<, >, = ) and ladders (H) to reach Pauline ($)\n"
        << "at the top while avoiding deadly barrels (O) thrown by Donkey Kong (&).\n"
        << "Barrels roll along floors, fall off edges, and explode after long falls.\n"
        << "Touching a barrel, being near an explosion (within 2-pixel range),\n"
        << "or falling too far will cost Mario one of his three lives.\n"
        << "Be careful of ghosts (x) which move unpredictably and are deadly if touched without a hammer.\n"
        << "Be aware of Smart ghosts (X) which target mario and follow him wherever he goes (yes, they can even climb ladders).\n"
        << "Use hammers (p) to kill ghosts and barrels scattered on floors.\n"
        << "If all lives are lost, the game ends.\n"
        << "Rescue Pauline to win the stage.\n\n"
        << "Keys:\n"
        << "LEFT: a or A\n"
        << "RIGHT: d or D\n"
        << "UP/JUMP: w or W\n"
        << "DOWN: x or X\n"
        << "STAY: s or S\n"
        << "Kill enemys: p or P (only if you collected p from board)\n\n"
        << "Stay alert and time your movements carefully\n"
        << "to outwit Donkey Kong, and save Pauline!\n\n\n"
        << "                        Press ESC to return to the menu.";

    Return_After_ESC(); // Wait for ESC to return
}

// Main game loop
void Game::Run(char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_locatio, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost)
{
    bool isColored = this->isGameColored;
    int pace = Regular_Pace;
    system("CLS"); // Clear the screen //this function i found in interntet search in https://www.geeksforgeeks.org/how-to-clear-console-in-cpp/
    ShowConsoleCursor(false); // Hide the console cursor // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
    Board board;
    board.reset(Lives,Score, Legend_location, Board_Read_From_File); // Initialize the board
    bool Not_Print_Hammer = false;
    board.print(isColored,Lives,Score, Legend_location, Not_Print_Hammer, hammer_location); // Print the board

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
            if (mario.Get_Is_Holding_Hammer() && std::tolower(key) == Hammer_symbol)
            {
                Hammer_Destroyed_Enemies(mario, bar, Barrel_Index, ghosts, board,Legend_location);
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
        for (int j=0;j<25;j++)
        {
            for (int i=0; i < ghosts[j].size(); i++)
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
    }
}

// Pauses the game and waits for ESC to resume
void Game::Pause_Game() const
{
    system("CLS"); //this function i found in interntet search in https://www.geeksforgeeks.org/how-to-clear-console-in-cpp/
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n                          To return the Game Press ESC\n";
    Return_After_ESC();
}

// Checks if Mario is hit by a barrel or other obstacles
bool Game::Is_Hit(Player& Mario, Barrel barim[], int Size_Barrel, const VectorOfGhosts& ghosts)
{
    if (Mario.Is_mario_Fall_To_Death()) // Check if Mario falls to his death
    {
        return true;
    }

    for (int i = 0; i <= Size_Barrel; i++) // Check collisions with barrels
    {
        if ((Mario.getX() == barim[i].getX()) && (Mario.getY() == barim[i].getY()))
        {
            return true;
        }
    }
    for (int j = 0; j < 25; j++)
    {
        for (int i = 0; i < ghosts[j].size(); i++)
        {
            if ((Mario.getX() == ghosts[j][i]->getX()) && (Mario.getY() == ghosts[j][i]->getY()))
            {
                return true;
            }
        }
    }
    return false;
}

// Checks if Mario is within the explosion radius of a barrel
bool Game::Is_Mario_In_Exp_Radius(int mario_x, int mario_y, int barrel_x, int barrel_y)
{
    int distance = std::abs(mario_x - barrel_x) + std::abs(mario_y - barrel_y); // Manhattan distance //this function of abs in std i learnrd in https://en.cppreference.com/w/cpp/numeric/math/abs
    return distance <= 2; // Explosion radius is 2 units 
}

// Waits for the ESC key to resume
void Game::Return_After_ESC() const
{
    char Back = Default_Choice_Menu;
    while (true)
    {
        if (_kbhit()) // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
        {
            Back = _getch();
            if (Back == ESC)
                break;
        }
    }
}

// Waits for the 9 key to continue
void Game::Next_Level() const
{
    char next = Default_Choice_Menu;
    while (true)
    {
        if (_kbhit()) // this function learned and copied from the lesson in class with amrir kirsh in https://mama.mta.ac.il/mod/page/view.php?id=33165
        {
            next = _getch();
            if (next == Next)
                break;
        }
    }
}

//Function to print the Donkey Kong logo in the menu. Color functions from windows.h taken from ChatGPT
void Game::printLogo() const 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); // Red
    for (int i = 0; i < Lines_Of_Logo; i++)
    {
        std::cout << logo[i] << '\n';
    }
    SetConsoleTextAttribute(hConsole, 7);  //Set to defualt after printing the logo
}

//Function to check and handle mario hitting enemies
void Game::Hammer_Destroyed_Enemies(Player& Mario, Barrel barim[], int Size_Barrel, VectorOfGhosts& ghosts, Board& board, Location& Legend_location)
{
    // Check all barrels for collisions with Mario while holding a hammer
    for (int i = 0; i <= Size_Barrel; i++)
    {
        // If Mario and a barrel are on the same row
        if (Mario.getY() == barim[i].getY())
        {
            // Check if the barrel is directly to the left or right of Mario
            if ((((Mario.getX() + Right == barim[i].getX()) || (Mario.getX() + Right + Right == barim[i].getX())) && Mario.getDirection().x == Right) ||
                (((Mario.getX() + Left == barim[i].getX()) || (Mario.getX() + Left + Left == barim[i].getX())) && Mario.getDirection().x == Left))
            {
                Score += Plus_Score; // Award points for destroying the barrel
                barim[i].Reset();   // Reset the barrel's position
            }
        }
    }

    // Check all ghosts for collisions with Mario while holding a hammer
    for (int j = 0; j < 25; j++) // Iterate through each row of ghosts
    {
        for (int i = ghosts[j].size() - 1; i >= 0; i--) // Iterate through ghosts in the row
        {
            // If Mario and the ghost are on the same row
            if (Mario.getY() == ghosts[j][i]->getY())
            {
                // Check if the ghost is directly to the left or right of Mario
                if (((((Mario.getX() + Right == ghosts[j][i]->getX()) || (Mario.getX() + Right + Right == ghosts[j][i]->getX())) && Mario.getDirection().x == Right) ||
                    (((Mario.getX() + Left == ghosts[j][i]->getX()) || (Mario.getX() + Left + Left == ghosts[j][i]->getX())) && Mario.getDirection().x == Left)))
                {
                    Score += Plus_Score;        // Award points for destroying the ghost
                    delete ghosts[j][i];       // Free memory for the destroyed ghost
                    ghosts[j].erase(ghosts[j].begin() + i); // Remove the ghost from the vector
                }
            }
        }
    }
    if (!is_silent)
    {
        // Update the score display on the board
        board.Update_Score(Score, Legend_location);
    }
}

// Checks if the 
//  list is empty and, if so, displays an error to the user.
// Ensures the Screens list is populated if it has not been read already.
bool Game::Check_If_No_Screens()
{
    // Populate the screen list if it hasn't been read already
    if (!Read_Screen)
    {
        Number_Of_File();
        Read_Screen = true;
    }

    // If no screens are available, display an error and return to the menu
    if (Screens.empty())
    {
        system("CLS"); // Clear the console screen
        std::cout << "\n\n\n\n\n\n\n\n\n                     You Have a problem, There are no screens\n"
            << "         check your text file names in Screens folder and run the game again\n\n"
            << "                        Press ESC to return to the menu\n";

        Return_After_ESC(); // Wait for the user to press ESC
        return true;        // Indicate that there are no screens
    }
    return false; // Screens are available
}

// Iterates through the "Screens" directory to locate valid screen files.
// Adds all files with valid naming conventions to the Screens list.
// Handles filesystem errors.
void Game::Number_Of_File()
{
    string name_file;               // Temporary variable to hold the name of each file
    string directory = ".";   // Directory where the screen files are stored

    try
    {
        // Iterate over all entries in the specified directory (from ChatGpt)
        for (const auto& entry : fs::directory_iterator(directory))
        {
            if (entry.is_regular_file()) // Check if the current entry is a regular file
            {
                name_file = entry.path().filename().string(); // Extract the file name

                // Validate the file name format before adding it to the list of screens
                if (Is_File_Name_In_Format(name_file, Suffix, Min_Length_Board_Name))
                {
                    Screens.push_back(name_file); // Add the valid file name to the Screens list
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
}

// Validates the format of a given file name based on prefix, suffix, and numeric content.
// Ensures the file name meets the convention.
bool Game::Is_File_Name_In_Format(const string& name_file,const char* sufix,int min_length)
{
    // Check minimal length
    if (name_file.size() < min_length)
    {
        return false;
    }

    // Check prefix
    if (name_file.compare(First_Index_On_File_Name, strlen(Prefix), Prefix) != 0)
    {
        return false;
    }

    // Check sufix
    if (name_file.compare(name_file.size() - strlen(sufix),strlen(sufix), sufix) != 0)
    {
        return false;
    }

    // Check digits in the middle
    int digit_start = strlen(Prefix);
    int digit_end = name_file.size() - strlen(sufix);
    if (digit_end - digit_start < 2)
    { // Ensure at least 2 digits
        return false;
    }
    for (int i = digit_start; i < digit_end; i++)
    {
        if (name_file[i] < '0' || name_file[i] > '9')
        {
            return false;
        }
    }
    return true;
}

// Displays a page-like menu of available screen files to the user.
// Allows navigation through screens and returns the index of the selected screen.
int Game::Menu_Screens()
{
    Screens.sort(); // Sort the list of screen names alphabetically

    int counter = 0;           // Counter for the current screen index
    int size_screen = Screens.size(); // Total number of screens
    int outloop, inloop;       // Variables to handle screen batches
    char choice = Default_Choice_Menu;
    auto it = Screens.begin(); // Iterator for the screen list

    while (counter < size_screen) // Loop through all available screens
    {
        system("CLS"); // Clear the console screen
        printLogo();   // Print the game logo
        std::cout << "\n\n\n";

        inloop = counter + 5; // Display 5 screens at a time
        for (outloop = counter; outloop < size_screen && outloop < inloop; outloop++)
        {
            std::cout << "                            (" << ((outloop % 5) + 1) << ") " << *it << std::endl;
            ++it; // Move to the next screen
        }

        for (int i = 0; i < inloop - outloop; i++) // Fill remaining lines for alignment
        {
            std::cout << "\n";
        }

        // Display navigation options
        if (counter >= 5)
        {
            if (size_screen - outloop > 0)
            {
                std::cout << "                  (8) Back                           (9) Next";
            }
            else
            {
                std::cout << "                  (8) Back                          ";
            }
        }
        else
        {
            if (size_screen > 5)
            {
                std::cout << "                                                     (9) Next";
            }
        }

        // Handle user input for menu navigation
        while (true)
        {
            if (_kbhit()) // Check if a key is pressed
            {
                choice = _getch(); // Get the pressed key

                if (choice == Back && counter >= 5) // Navigate back
                {
                    for (int i = 0; i < outloop - counter + 5; i++)
                    {
                        --it; // Move iterator backwards
                    }
                    inloop -= 10;
                    break;
                }
                else if (choice >= '1' && choice <= '5' && choice <= char(outloop - counter + '0')) // Select a screen
                {
                    return int(choice - '1') + counter;
                }
                if (choice == Next && outloop != size_screen) // Navigate forward
                {
                    break;
                }
            }
        }
        counter = inloop; // Update the counter for the next batch
    }
    return 0; // Default return if no screen is selected
}

// Reads a screen file and validates its content.
// Extracts key objects such as Mario, Pauline, Donkey Kong, Hammer, and ghosts from the file.
// Updates the game board and ensures the screen file follows expected rules.
bool Game::Read_The_Screen(string& name_board, char Board_Read_From_File[Max_Y_Board][Max_X_Board + 1], Location& mario_location, Location& pauline_location, Location& donkeykong_location, Location& hammer_location, Location& Legend_location, VectorOfLocationofGhost& ghosts_location, TypeOfGhosts& type_of_ghost)
{
    string file_path = name_board; // Build the full path to the screen file
    ifstream file(file_path); // Open the file for reading
    if (!file.is_open()) {
        std::cout << "\n\n\n\n\n\n\n Error: Unable to open file: " << name_board << "\n"
            << " Ensure the file exists in the 'Screens' directory and has the correct name.\n\n"
            << " Press 9 to continue to the next level\n";
        Next_Level();
        return false;
    }


    // Initialize the board with empty spaces and terminate each row with a null character
    for (int i = 0; i < Max_Y_Board; ++i)
    {
        fill(Board_Read_From_File[i], Board_Read_From_File[i] + Max_X_Board, Empty_Space);
        Board_Read_From_File[i][Max_X_Board] = '\0';
    }

    // Clear any existing ghost locations for the new screen
    for (auto& row : ghosts_location)
    {
        row.clear();
    }
    for (auto& row : type_of_ghost)
    {
        row.clear();
    }

    char Ghost_Location[Max_Y_Board][Max_X_Board + 1] = { 0 }; // Temporary storage for ghost positions
    string line; // Stores each line from the screen file
    int y = 0;   // Row counter for the board
    int counter_symbol[Size_Of_Symbol] = { 0 }; // Tracks occurrences of key symbols
    const string symbol[] = { "mario", "pauline", "donkey kong", "hammer", "Legend" }; // List of symbols to validate

    // Read the file line by line until reaching the maximum board height
    while (std::getline(file, line) && y < Max_Y_Board)
    {
        // Ensure the line has at least the required width
        if (line.size() < Max_X_Board)
        {
            system("CLS"); // Clear the console screen
            std::cout << "\n\n\n\n\n\n\n                      Error in Screen: " << name_board <<
                "\n             you have: " << line.size() << " chars in row number: " << y << ", which is less than 80\n\n"
                << "                      Press 9 to continue to the next level\n";
            Next_Level();
            return false;
        }

        line = line.substr(0, Max_X_Board); // Trim the line to the board width

        // Process each character in the line
        for (int x = 0; x < line.size(); ++x)
        {
            char symbol = line[x];
            Board_Read_From_File[y][x] = symbol; // Place the symbol on the board

            // Identify and process special symbols
            if (symbol == Mario_symbol)
            {
                mario_location = { x, y }; // Store Mario's location
                Board_Read_From_File[y][x] = Empty_Space; // Clear Mario's position on the board
                counter_symbol[mario_index]++;
            }
            else if (symbol == Pauline_symbol)
            {
                pauline_location = { x, y }; // Store Pauline's location
                counter_symbol[pauline_index]++;
            }
            else if (symbol == DonkeyKong_symbol)
            {
                donkeykong_location = { x, y }; // Store Donkey Kong's location
                counter_symbol[donkey_kong_index]++;
            }
            else if (symbol == Hammer_symbol)
            {
                hammer_location = { x, y }; // Store Hammer's location
                counter_symbol[hammer_index]++;
            }
            else if (symbol == Ghost_symbol || symbol == Smart_Ghost_symbol)
            {
                ghosts_location[y].push_back(new Location{ x, y }); // Add ghost location
                if (symbol == Ghost_symbol)
                {
                    Ghost_Location[y][x] = Ghost_symbol;
                    type_of_ghost[y].push_back(new int(Regular_Ghost)); // Add regular ghost
                }
                else
                {
                    type_of_ghost[y].push_back(new int(Smart_Ghost)); // Add Smart ghost
                    Ghost_Location[y][x] = Smart_Ghost_symbol;
                }
                Board_Read_From_File[y][x] = Empty_Space; // Clear Ghost's position on the board
            }
            else if (symbol == Legend_Symbol)
            {
                Legend_location = { x, y }; // Store Legend's location
                Board_Read_From_File[y][x] = Empty_Space; // Clear Legend's position on the board
                counter_symbol[legend_index]++;
            }
            else if (!Is_Platform(symbol) && (symbol != Empty_Space && symbol != Wall_Symbol && symbol != Legend_Symbol && symbol != Ladder))
            {
                // Error for invalid characters
                system("CLS");
                std::cout << "\n\n\n\n\n\n\n                      Error in Screen: " << name_board <<
                    "\n                you have invalid char: " << symbol << " in line: " << x << ", row: " << y << "\n\n"
                    << "                      Press 9 to continue to the next level\n";
                Next_Level();
                return false;
            }
        }
        ++y; // Move to the next row
    }
    file.close(); // Close the file

    // Check if the file has fewer rows than the maximum board height
    if (y < Max_Y_Board)
    {
        system("CLS");
        std::cout << "\n\n\n\n\n\n\n                      Error in Screen: " << name_board <<
            "\n                     you have only: " << y << " rows in the screen \n\n"
            << "                      Press 9 to continue to the next level\n";
        Next_Level();
        return false;
    }

    // Update walls and validate ghost positions
    char symbol_wall = Board_Read_From_File[0][Max_X_Board - 1];
    for (int y = 0; y < Max_Y_Board; y++)
    {
        Board_Read_From_File[y][0] = symbol_wall; // Set left wall
        Board_Read_From_File[y][Max_X_Board - 1] = symbol_wall; // Set right wall
        for (int x = 0; x < Max_X_Board; x++)
        {
            // Check if a ghost is not above a valid platform
            if ((Ghost_Location[y][x] == Ghost_symbol || Ghost_Location[y][x] == Smart_Ghost_symbol) && y != Max_Y_Board + Up && !Is_Platform(Board_Read_From_File[y + Down][x]))
            {
                system("CLS");
                std::cout << "\n\n\n\n\n\n\n                      Error in Screen: " << name_board <<
                    "\n                the ghost is not above a platform in line: " << y << "\n\n"
                    << "                      Press 9 to continue to the next level\n";
                Next_Level();
                return false;
            }
        }
    }

    // Process the floor layout at the bottom of the board
    char symbol_floor = Board_Read_From_File[Max_Y_Board + Up][1];
    for (int x = 0; x < Max_X_Board; x++)
    {
        Board_Read_From_File[0][x] = symbol_wall; // Set top wall
        if (symbol_floor == Empty_Space && Board_Read_From_File[Max_Y_Board + Up + Up][x] == Ladder)
        {
            Board_Read_From_File[Max_Y_Board + Up][x] = Ladder; // Adjust floor ladders if necessary
        }
    }

    // Ensure walls are consistent
    if (symbol_wall != Empty_Space)
    {
        Board_Read_From_File[Max_Y_Board + Up][0] = symbol_wall; // Left corner
        Board_Read_From_File[Max_Y_Board + Up][Max_X_Board - 1] = symbol_wall; // Right corner
    }

    // Validate the presence and count of all required symbols
    int y_Error[] = { mario_location.y, pauline_location.y, donkeykong_location.y, hammer_location.y, Legend_location.y };
    for (int i = 0; i < Size_Of_Symbol; i++)
    {
        if (counter_symbol[i] == 0) // Missing symbol
        {
            system("CLS");
            std::cout << "\n\n\n\n\n\n\n                        Error in Screen: " << name_board <<
                "\n                      You don't have any " << symbol[i] << "'s in screen\n\n"
                << "                      Press 9 to continue to the next level\n";
            Next_Level();
            return false;
        }
        if (counter_symbol[i] > 1) // Duplicate symbol
        {
            system("CLS");
            std::cout << "\n\n\n\n\n\n\n                      Error in Screen: " << name_board <<
                "\n        You have more than 1 " << symbol[i] << ", the last " << symbol[i] << " is in line: " << y_Error[i] << "\n\n"
                << "                      Press 9 to continue to the next level\n";
            Next_Level();
            return false;
        }
    }

    // Add the Legend text structure to the board
    string Legend_Str = "Lives:  ~ Score:   ";
    for (int i = 0; i < Legend_Str.length(); i++)
    {
        Board_Read_From_File[Legend_location.y][Legend_location.x + i] = Legend_Str[i];
    }
    return true; // Screen successfully read and validated
}



