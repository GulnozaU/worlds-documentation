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
