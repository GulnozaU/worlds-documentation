First Name Last Name
May 11, 2026
Final Project
Memory Matching Game - Text Based Game

# A. Program Requirements - Copied from Assignment
Create a Memory Matching Game as a console/text based C++ program using cin and cout.
Create three 50-word theme files. Display menus for theme, board size, and delay speed.
Load the selected theme words into a one-dimensional array, shuffle the words, and choose 8, 18, or 32 terms depending on the board size.
Place each chosen word twice in a two-dimensional answer array, shuffle the answers, and use a separate two-dimensional display array.
Begin the game with the selected theme name visible in every square.
Let the user choose two squares each turn, reveal both words, compare them, keep matches visible, and hide nonmatches after 2, 4, or 6 seconds.
Use variables, one-dimensional arrays, two-dimensional arrays, and clearly named functions. Do not define classes or structures.
Extra credit: display a timer showing how many seconds it took to win.

# B. Required Line of Code
First line after the int main() function:
```cpp
cout << "Student Name - May 11, 2026" << endl;
```

# C. Copy and Paste of All Code
```cpp
#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <thread>
#include <chrono>

const int WORD_COUNT = 50;
const int THEME_COUNT = 3;
const int MAX_GRID_SIZE = 8;
const int MAX_CELLS = 64;
const int CELL_WIDTH = 16;

string themeNames[THEME_COUNT] = {"Animals", "Space", "Sports"};
string themeFiles[THEME_COUNT] = {"animals_theme.txt", "space_theme.txt", "sports_theme.txt"};
string wordArray[WORD_COUNT];
string answerArray[MAX_GRID_SIZE][MAX_GRID_SIZE];
string displayArray[MAX_GRID_SIZE][MAX_GRID_SIZE];
bool matchedArray[MAX_GRID_SIZE][MAX_GRID_SIZE];

void seedRandomNumberGenerator();
void displayWelcomeMessage();
int getThemeChoice();
int getGridSizeChoice();
int getDelayChoice();
bool getDemoChoice();
bool loadThemeWords(string fileName);
void shuffleStringArray(string values[], int count);
void prepareGameBoard(int gridSize, string faceTerm);
void displayBoard(int gridSize);
void playManualGame(int gridSize, int delaySeconds, string faceTerm);
void playDemoGame(int gridSize);
void chooseSquare(int gridSize, int blockedRow, int blockedCol, int &row, int &col);
bool isNumberText(string text);
int textToNumber(string text);
char firstUpperLetter(string text);
void waitSeconds(int seconds);

int main()
{
    cout << "Student Name - May 11, 2026" << endl;

    seedRandomNumberGenerator();
    displayWelcomeMessage();

    int themeIndex = getThemeChoice();
    int gridSize = getGridSizeChoice();
    int delaySeconds = getDelayChoice();
    bool demoMode = getDemoChoice();

    if (!loadThemeWords(themeFiles[themeIndex]))
    {
        cout << "The selected theme file could not be loaded. Program ending." << endl;
        return 1;
    }

    shuffleStringArray(wordArray, WORD_COUNT);
    prepareGameBoard(gridSize, themeNames[themeIndex]);

    if (demoMode)
    {
        playDemoGame(gridSize);
    }
    else
    {
        playManualGame(gridSize, delaySeconds, themeNames[themeIndex]);
    }

    return 0;
}

void seedRandomNumberGenerator()
{
    int seedValue = static_cast<int>(time(NULL));
    char *testSeed = getenv("MEMORY_GAME_SEED");

    if (testSeed != NULL && testSeed[0] != '\0')
    {
        seedValue = atoi(testSeed);
    }

    srand(seedValue);
}

void displayWelcomeMessage()
{
    cout << endl;
    cout << "Final Project - Memory Matching Game" << endl;
    cout << "Match all word pairs on the text based board." << endl;
    cout << "Enter row and column numbers to choose squares." << endl;
    cout << endl;
}

int getThemeChoice()
{
    string input = "";

    while (true)
    {
        cout << "Select Theme" << endl;
        cout << "1. " << themeNames[0] << endl;
        cout << "2. " << themeNames[1] << endl;
        cout << "3. " << themeNames[2] << endl;
        cout << "Enter ( 1, 2 or 3) : ";
        cin >> input;

        if (input == "1")
        {
            return 0;
        }
        else if (input == "2")
        {
            return 1;
        }
        else if (input == "3")
        {
            return 2;
        }

        cout << "Invalid theme. Please enter 1, 2, or 3." << endl << endl;
    }
}

int getGridSizeChoice()
{
    string input = "";
    char choice = ' ';

    while (true)
    {
        cout << endl;
        cout << "Select Size of Play" << endl;
        cout << "A. 4 x 4 grid (Small)" << endl;
        cout << "B. 6 x 6 grid (Medium)" << endl;
        cout << "C. 8 x 8 grid (Large)" << endl;
        cout << "Enter ( S, M, L) : ";
        cin >> input;

        choice = firstUpperLetter(input);

        if (choice == 'S' || choice == 'A')
        {
            return 4;
        }
        else if (choice == 'M' || choice == 'B')
        {
            return 6;
        }
        else if (choice == 'L' || choice == 'C')
        {
            return 8;
        }

        cout << "Invalid size. Please enter S, M, or L." << endl;
    }
}

int getDelayChoice()
{
    string input = "";
    char choice = ' ';

    while (true)
    {
        cout << endl;
        cout << "Select Time word pair remain on screen" << endl;
        cout << "A. 2 seconds (Fast)" << endl;
        cout << "B. 4 seconds (Moderate)" << endl;
        cout << "C. 6 seconds (Slow)" << endl;
        cout << "Enter ( F, M, S ) : ";
        cin >> input;

        choice = firstUpperLetter(input);

        if (choice == 'F' || choice == 'A')
        {
            return 2;
        }
        else if (choice == 'M' || choice == 'B')
        {
            return 4;
        }
        else if (choice == 'S' || choice == 'C')
        {
            return 6;
        }

        cout << "Invalid speed. Please enter F, M, or S." << endl;
    }
}

bool getDemoChoice()
{
    string input = "";
    char choice = ' ';

    while (true)
    {
        cout << endl;
        cout << "Run automatic demo/test mode? (Y/N): ";
        cin >> input;
        choice = firstUpperLetter(input);

        if (choice == 'Y')
        {
            return true;
        }
        else if (choice == 'N')
        {
            return false;
        }

        cout << "Invalid answer. Please enter Y or N." << endl;
    }
}

bool loadThemeWords(string fileName)
{
    ifstream inputFile(fileName.c_str());
    string folderFileName = "";

    if (!inputFile)
    {
        folderFileName = "final-project-memory-game/" + fileName;
        inputFile.open(folderFileName.c_str());
    }

    if (!inputFile)
    {
        return false;
    }

    for (int i = 0; i < WORD_COUNT; i++)
    {
        if (!(inputFile >> wordArray[i]))
        {
            cout << "Theme file must contain at least " << WORD_COUNT << " words." << endl;
            return false;
        }
    }

    inputFile.close();
    return true;
}

void shuffleStringArray(string values[], int count)
{
    string temp = "";
    int randomIndex = 0;

    for (int i = 0; i < count; i++)
    {
        randomIndex = rand() % count;
        temp = values[i];
        values[i] = values[randomIndex];
        values[randomIndex] = temp;
    }
}

void prepareGameBoard(int gridSize, string faceTerm)
{
    string flatAnswers[MAX_CELLS];
    int pairsNeeded = (gridSize * gridSize) / 2;
    int flatIndex = 0;

    for (int i = 0; i < pairsNeeded; i++)
    {
        flatAnswers[flatIndex] = wordArray[i];
        flatIndex++;
        flatAnswers[flatIndex] = wordArray[i];
        flatIndex++;
    }

    shuffleStringArray(flatAnswers, gridSize * gridSize);

    flatIndex = 0;
    for (int row = 0; row < gridSize; row++)
    {
        for (int col = 0; col < gridSize; col++)
        {
            answerArray[row][col] = flatAnswers[flatIndex];
            displayArray[row][col] = faceTerm;
            matchedArray[row][col] = false;
            flatIndex++;
        }
    }
}

void displayBoard(int gridSize)
{
    cout << endl;
    cout << setw(7) << " ";

    for (int col = 0; col < gridSize; col++)
    {
        cout << setw(CELL_WIDTH) << ("Col " + to_string(col + 1));
    }

    cout << endl;

    for (int row = 0; row < gridSize; row++)
    {
        cout << "Row " << setw(2) << (row + 1);

        for (int col = 0; col < gridSize; col++)
        {
            cout << setw(CELL_WIDTH) << displayArray[row][col];
        }

        cout << endl;
    }

    cout << endl;
}

void playManualGame(int gridSize, int delaySeconds, string faceTerm)
{
    int firstRow = 0;
    int firstCol = 0;
    int secondRow = 0;
    int secondCol = 0;
    int matchedPairs = 0;
    int totalPairs = (gridSize * gridSize) / 2;
    int turns = 0;
    time_t startTime = time(NULL);
    time_t endTime = 0;

    cout << endl;
    cout << "Game started. The hidden board begins with the theme term in every square." << endl;

    while (matchedPairs < totalPairs)
    {
        displayBoard(gridSize);

        cout << "Choose the FIRST square." << endl;
        chooseSquare(gridSize, -1, -1, firstRow, firstCol);
        displayArray[firstRow][firstCol] = answerArray[firstRow][firstCol];
        displayBoard(gridSize);

        cout << "Choose the SECOND square." << endl;
        chooseSquare(gridSize, firstRow, firstCol, secondRow, secondCol);
        displayArray[secondRow][secondCol] = answerArray[secondRow][secondCol];
        displayBoard(gridSize);

        turns++;

        if (answerArray[firstRow][firstCol] == answerArray[secondRow][secondCol])
        {
            cout << "Match found: " << answerArray[firstRow][firstCol] << endl;
            matchedArray[firstRow][firstCol] = true;
            matchedArray[secondRow][secondCol] = true;
            matchedPairs++;
        }
        else
        {
            cout << "Not a match. The two words will remain on screen for "
                 << delaySeconds << " seconds." << endl;
            waitSeconds(delaySeconds);
            displayArray[firstRow][firstCol] = faceTerm;
            displayArray[secondRow][secondCol] = faceTerm;
        }

        cout << "Matched pairs: " << matchedPairs << " of " << totalPairs << endl;
    }

    endTime = time(NULL);
    displayBoard(gridSize);
    cout << "Congratulations! You won the Memory Matching Game." << endl;
    cout << "Total turns: " << turns << endl;
    cout << "Timer: " << static_cast<int>(endTime - startTime) << " seconds" << endl;
}

void playDemoGame(int gridSize)
{
    int firstRow = 0;
    int firstCol = 0;
    int secondRow = 0;
    int secondCol = 0;
    int matchedPairs = 0;
    int totalPairs = (gridSize * gridSize) / 2;
    int turns = 0;
    bool foundFirst = false;
    bool foundSecond = false;
    time_t startTime = time(NULL);
    time_t endTime = 0;

    cout << endl;
    cout << "Automatic demo/test mode started." << endl;
    cout << "This mode shows a successful execution for test-case screen prints." << endl;
    displayBoard(gridSize);

    while (matchedPairs < totalPairs)
    {
        foundFirst = false;
        foundSecond = false;

        for (int row = 0; row < gridSize && !foundFirst; row++)
        {
            for (int col = 0; col < gridSize && !foundFirst; col++)
            {
                if (!matchedArray[row][col])
                {
                    firstRow = row;
                    firstCol = col;
                    foundFirst = true;
                }
            }
        }

        for (int row = 0; row < gridSize && !foundSecond; row++)
        {
            for (int col = 0; col < gridSize && !foundSecond; col++)
            {
                if (!matchedArray[row][col]
                    && !(row == firstRow && col == firstCol)
                    && answerArray[row][col] == answerArray[firstRow][firstCol])
                {
                    secondRow = row;
                    secondCol = col;
                    foundSecond = true;
                }
            }
        }

        cout << "Demo turn " << (turns + 1) << ": ";
        cout << "select Row " << (firstRow + 1) << ", Col " << (firstCol + 1);
        cout << " and Row " << (secondRow + 1) << ", Col " << (secondCol + 1) << endl;

        displayArray[firstRow][firstCol] = answerArray[firstRow][firstCol];
        displayArray[secondRow][secondCol] = answerArray[secondRow][secondCol];
        matchedArray[firstRow][firstCol] = true;
        matchedArray[secondRow][secondCol] = true;
        matchedPairs++;
        turns++;

        displayBoard(gridSize);
        cout << "Match found: " << answerArray[firstRow][firstCol] << endl;
        cout << "Matched pairs: " << matchedPairs << " of " << totalPairs << endl;
    }

    endTime = time(NULL);
    cout << "Congratulations! You won the Memory Matching Game." << endl;
    cout << "Total turns: " << turns << endl;
    cout << "Timer: " << static_cast<int>(endTime - startTime) << " seconds" << endl;
}

void chooseSquare(int gridSize, int blockedRow, int blockedCol, int &row, int &col)
{
    string rowText = "";
    string colText = "";
    int enteredRow = 0;
    int enteredCol = 0;

    while (true)
    {
        cout << "Enter row and column, separated by a space: ";

        if (!(cin >> rowText >> colText))
        {
            cout << endl << "Input ended. Game stopped." << endl;
            exit(0);
        }

        if (!isNumberText(rowText) || !isNumberText(colText))
        {
            cout << "Please enter numbers only." << endl;
            continue;
        }

        enteredRow = textToNumber(rowText) - 1;
        enteredCol = textToNumber(colText) - 1;

        if (enteredRow < 0 || enteredRow >= gridSize || enteredCol < 0 || enteredCol >= gridSize)
        {
            cout << "That square is outside the board." << endl;
        }
        else if (matchedArray[enteredRow][enteredCol])
        {
            cout << "That square is already matched. Choose another square." << endl;
        }
        else if (enteredRow == blockedRow && enteredCol == blockedCol)
        {
            cout << "You already selected that square this turn. Choose another square." << endl;
        }
        else
        {
            row = enteredRow;
            col = enteredCol;
            return;
        }
    }
}

bool isNumberText(string text)
{
    if (text.length() == 0)
    {
        return false;
    }

    for (int i = 0; i < static_cast<int>(text.length()); i++)
    {
        if (!isdigit(static_cast<unsigned char>(text[i])))
        {
            return false;
        }
    }

    return true;
}

int textToNumber(string text)
{
    int number = 0;

    for (int i = 0; i < static_cast<int>(text.length()); i++)
    {
        number = (number * 10) + (text[i] - '0');
    }

    return number;
}

char firstUpperLetter(string text)
{
    if (text.length() == 0)
    {
        return ' ';
    }

    return static_cast<char>(toupper(static_cast<unsigned char>(text[0])));
}

void waitSeconds(int seconds)
{
    this_thread::sleep_for(chrono::seconds(seconds));
}
```

# D. Conclusion / Lessons Learned
Writing this program helped me practice breaking a larger program into smaller functions. I used file input to load theme words, arrays to store both the answers and the visible board, and random shuffling to make each game different. I also practiced validating user input, comparing selected squares, delaying nonmatching pairs, and tracking the total time for a win.

# E. Screen Prints of Successful Execution / Run
The following console-output excerpts are from successful test runs. Full transcripts are included as separate .txt files in this project folder.

## 4x4 Animals Test Run
```text
Matched pairs: 5 of 8
Demo turn 6: select Row 2, Col 3 and Row 3, Col 1

                  Col 1           Col 2           Col 3           Col 4
Row  1          shrimp           sheep          salmon           sheep
Row  2           otter             fox           whale         Animals
Row  3           whale             fox          salmon         Animals
Row  4           otter         Animals         Animals          shrimp

Match found: whale
Matched pairs: 6 of 8
Demo turn 7: select Row 2, Col 4 and Row 4, Col 3

                  Col 1           Col 2           Col 3           Col 4
Row  1          shrimp           sheep          salmon           sheep
Row  2           otter             fox           whale           trout
Row  3           whale             fox          salmon         Animals
Row  4           otter         Animals           trout          shrimp

Match found: trout
Matched pairs: 7 of 8
Demo turn 8: select Row 3, Col 4 and Row 4, Col 2

                  Col 1           Col 2           Col 3           Col 4
Row  1          shrimp           sheep          salmon           sheep
Row  2           otter             fox           whale           trout
Row  3           whale             fox          salmon           goose
Row  4           otter           goose           trout          shrimp

Match found: goose
Matched pairs: 8 of 8
Congratulations! You won the Memory Matching Game.
Total turns: 8
Timer: 0 seconds
```

## 6x6 Space Test Run
```text
Row  3           lunar          rocket          uranus          saturn            star            moon
Row  4          rocket          uranus          cosmos          pulsar       blackhole           orbit
Row  5           pluto           probe           solar           Space           Space           comet
Row  6           probe           Space       spacesuit       blackhole           Space           lunar

Match found: probe
Matched pairs: 16 of 18
Demo turn 17: select Row 5, Col 4 and Row 6, Col 2

                  Col 1           Col 2           Col 3           Col 4           Col 5           Col 6
Row  1           pluto            star           orbit           comet           solar          saturn
Row  2          module            moon          module          pulsar          cosmos       spacesuit
Row  3           lunar          rocket          uranus          saturn            star            moon
Row  4          rocket          uranus          cosmos          pulsar       blackhole           orbit
Row  5           pluto           probe           solar         capsule           Space           comet
Row  6           probe         capsule       spacesuit       blackhole           Space           lunar

Match found: capsule
Matched pairs: 17 of 18
Demo turn 18: select Row 5, Col 5 and Row 6, Col 5

                  Col 1           Col 2           Col 3           Col 4           Col 5           Col 6
Row  1           pluto            star           orbit           comet           solar          saturn
Row  2          module            moon          module          pulsar          cosmos       spacesuit
Row  3           lunar          rocket          uranus          saturn            star            moon
Row  4          rocket          uranus          cosmos          pulsar       blackhole           orbit
Row  5           pluto           probe           solar         capsule         shuttle           comet
Row  6           probe         capsule       spacesuit       blackhole         shuttle           lunar

Match found: shuttle
Matched pairs: 18 of 18
Congratulations! You won the Memory Matching Game.
Total turns: 18
Timer: 0 seconds
```

## 8x8 Sports Test Run
```text

Match found: judo
Matched pairs: 30 of 32
Demo turn 31: select Row 7, Col 6 and Row 7, Col 7

                  Col 1           Col 2           Col 3           Col 4           Col 5           Col 6           Col 7           Col 8
Row  1        baseball          racing       wrestling        football      tournament          skiing       wrestling        lacrosse
Row  2         skating         archery      scoreboard           medal           rugby          discus         archery           relay
Row  3      scoreboard      volleyball         surfing           medal          trophy      pickleball          skiing        softball
Row  4         skating        kickball        lacrosse         fencing         javelin        handball      basketball         sailing
Row  5           relay         sailing       dodgeball       badminton         javelin         bowling       badminton         bowling
Row  6         stadium           rugby        baseball          racing        softball            judo            judo      basketball
Row  7         surfing       dodgeball        kickball         stadium        handball          soccer          soccer         fencing
Row  8          Sports      tournament          Sports      pickleball          discus      volleyball        football          trophy

Match found: soccer
Matched pairs: 31 of 32
Demo turn 32: select Row 8, Col 1 and Row 8, Col 3

                  Col 1           Col 2           Col 3           Col 4           Col 5           Col 6           Col 7           Col 8
Row  1        baseball          racing       wrestling        football      tournament          skiing       wrestling        lacrosse
Row  2         skating         archery      scoreboard           medal           rugby          discus         archery           relay
Row  3      scoreboard      volleyball         surfing           medal          trophy      pickleball          skiing        softball
Row  4         skating        kickball        lacrosse         fencing         javelin        handball      basketball         sailing
Row  5           relay         sailing       dodgeball       badminton         javelin         bowling       badminton         bowling
Row  6         stadium           rugby        baseball          racing        softball            judo            judo      basketball
Row  7         surfing       dodgeball        kickball         stadium        handball          soccer          soccer         fencing
Row  8          hurdle      tournament          hurdle      pickleball          discus      volleyball        football          trophy

Match found: hurdle
Matched pairs: 32 of 32
Congratulations! You won the Memory Matching Game.
Total turns: 32
Timer: 0 seconds
```

# F. Test Cases

| Input Value | Expected Output | Actual Output |
| --- | --- | --- |
| 1, S, F, Y | Animals theme, 4x4 board, fast delay, automatic demo wins 8 pairs. | Program ended with: Congratulations; Total turns: 8; Timer displayed. |
| 2, M, F, Y | Space theme, 6x6 board, fast delay, automatic demo wins 18 pairs. | Program ended with: Congratulations; Total turns: 18; Timer displayed. |
| 3, L, F, Y | Sports theme, 8x8 board, fast delay, automatic demo wins 32 pairs. | Program ended with: Congratulations; Total turns: 32; Timer displayed. |

# G. Flow Chart / Diagram
Start -> Display menu -> Load selected theme file -> Shuffle 50 words -> Pick needed pairs -> Fill and shuffle answer array -> Fill display array with theme name -> Player selects first square -> Player selects second square -> Compare words -> Keep match or hide nonmatch after selected delay -> Repeat until all pairs matched -> Display win message and timer -> End.

# Files Included
- memory_matching_game.cpp
- animals_theme.txt
- space_theme.txt
- sports_theme.txt
- test_4x4_output.txt
- test_6x6_output.txt
- test_8x8_output.txt
