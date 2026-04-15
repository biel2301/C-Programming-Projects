#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 10 //Board size
#define WATER 'X' //Water tile
#define HIT 'H' //hit marker
#define MISS 'M' //Miss marker
#define MAX_SHIPS 5 //Max amount of ships
#define SCORE_FILE "topTenScores.txt" //Score tracker
#define STATE_FILE "gamestate.txt" //Game board state tracker

typedef struct { //Ship struct
    char name[30]; //Name
    int size; //How many tiles it takes up
    int hits; //How many hits it has taken
    int sunk; //Whether it's sunk or not
    int positions[SIZE][2]; // Store positions of the ship
} Ship;

// Function to initialize the board with water
void initializeBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = WATER;
        }
    }
}

// Function to place ships randomly on the board
void placeShips(char board[SIZE][SIZE], Ship ships[], int numShips) {
    srand(time(NULL));
    for (int i = 0; i < numShips; i++) {
        int placed = 0; //Check if the ship has been placed
        while (!placed) {
            int orientation = rand() % 2; // 0 = horizontal, 1 = vertical
            int row = rand() % SIZE;
            int col = rand() % SIZE;
            int size = ships[i].size;

            // Check if the ship can be placed
            int canPlace = 1;
            for (int j = 0; j < size; j++) {
                int r = row + (orientation == 1 ? j : 0); //Check the orientation and make sure the row
                int c = col + (orientation == 0 ? j : 0); //Or column match up
                if (r >= SIZE || c >= SIZE || board[r][c] != WATER) { //Check to make sure it's not out of bounds
                    canPlace = 0;
                    break;
                }
            }

            // Place the ship
            if (canPlace) {
                for (int j = 0; j < size; j++) {
                    int r = row + (orientation == 1 ? j : 0);
                    int c = col + (orientation == 0 ? j : 0);
                    board[r][c] = i + '1'; // Use ship index to identify ship on board
                    ships[i].positions[j][0] = r; // Store row position
                    ships[i].positions[j][1] = c; // Store column position
                }
                placed = 1;
            }
        }
    }
}

// Function to display the board with ship names if sunk
void displayBoard(char board[SIZE][SIZE], Ship ships[], int numShips) {
    printf("  ");
    for (int i = 0; i < SIZE; i++) { //Prints the column numbers
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i); //Prints the row numbers
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == HIT || board[i][j] == MISS) {
                printf("%c ", board[i][j]); //Places the hit or miss markers with a space
            } else if (board[i][j] >= '1' && board[i][j] <= '0' + numShips) {
                int shipIndex = board[i][j] - '1'; //Check if a ship is present in a space
                if (ships[shipIndex].sunk) { //Check whether a ship has been sunk
                    printf("%c ", ships[shipIndex].name[0]); // Display ship initial if sunk
                } else {
                    printf("%c ", WATER);
                }
            } else {
                printf("%c ", board[i][j]);
            }
        }
        printf("\n");
    }
}

// Function to take player input and check hits and misses
void takePlayerTurn(char board[SIZE][SIZE], Ship ships[], int numShips, int *shots) {
    int row, col;
    printf("Enter row and column (0-9): ");
    scanf("%d %d", &row, &col);

    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) { //Check if the coordinates are not out of bounds
        printf("Invalid coordinates. Try again.\n");
        return;
    }

    char cell = board[row][col]; //Gets a single cell on the board
    if (cell >= '1' && cell <= '0' + numShips) { //Check if a cell contains a ship
        int shipIndex = cell - '1'; //If yes, add a hit to the ship and print hit and mark the space
        ships[shipIndex].hits++;
        printf("Hit!\n");
        board[row][col] = HIT;

        // Check if the ship is sunk
        if (ships[shipIndex].hits == ships[shipIndex].size) {
            printf("You've sunk the %s!\n", ships[shipIndex].name);
            ships[shipIndex].sunk = 1; // Mark the ship as sunk
            // Update all parts of the ship on the board to show it's sunk
            for (int j = 0; j < ships[shipIndex].size; j++) {
                int r = ships[shipIndex].positions[j][0];
                int c = ships[shipIndex].positions[j][1];
                board[r][c] = ships[shipIndex].name[0];
            }
        }
    } else if (board[row][col] == WATER) { //Check if you missed
        printf("Miss!\n");
        board[row][col] = MISS;
    } else { //Making sure you don't double guess
        printf("You already guessed this spot. Try again.\n");
    }
    (*shots)++;//Increases the shot pointer by one
}

// Function to check if all ships are sunk
int checkGameOver(Ship ships[], int numShips) { 
    for (int i = 0; i < numShips; i++) {
        if (ships[i].hits < ships[i].size) {
            return 0; // Not all ships are sunk
        }
    }
    return 1; // All ships are sunk
}

// Function to save the game state to a file
void saveGameState(char board[SIZE][SIZE], Ship ships[], int numShips, int shots) {
    FILE *file = fopen(STATE_FILE, "w"); //Makes a game state file
    if (file == NULL) {
        perror("Error opening state file");
        return;
    }
    fwrite(board, sizeof(char), SIZE * SIZE, file); //Writes to the file the board state
    fwrite(ships, sizeof(Ship), numShips, file); //The ships
    fwrite(&shots, sizeof(int), 1, file); //And the number of shots taken
    fclose(file);
}

// Function to load the game state from a file
int loadGameState(char board[SIZE][SIZE], Ship ships[], int *numShips, int *shots) {
    FILE *file = fopen(STATE_FILE, "r");
    if (file == NULL) {
        return 0; // No saved game state
    }
    fread(board, sizeof(char), SIZE * SIZE, file);
    fread(ships, sizeof(Ship), MAX_SHIPS, file);
    fread(shots, sizeof(int), 1, file);
    *numShips = MAX_SHIPS;
    fclose(file);
    return 1; // Loaded saved game state
}

// Function to save the score to a file
void saveScore(int shots) {
    FILE *file = fopen(SCORE_FILE, "a");
    if (file == NULL) {
        perror("Error opening score file");
        return;
    }
    fprintf(file, "%d\n", shots);
    fclose(file);
}

// Function to display the top 10 scores
void displayTopScores() {
    FILE *file = fopen(SCORE_FILE, "r");
    if (file == NULL) {
        printf("No scores available.\n");
        return;
    }

    int scores[100];
    int count = 0;

    while (fscanf(file, "%d", &scores[count]) != EOF && count < 100) {
        count++;
    }
    fclose(file);

    // Sort the scores in ascending order
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (scores[i] > scores[j]) {
                int temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    printf("Top 10 Scores:\n");
    for (int i = 0; i < count && i < 10; i++) {
        printf("%d\n", scores[i]);
    }
}

// Function to display the game menu
int displayMenu() {
    int choice;
    printf("1. Fire Again\n");
    printf("2. Display Scores\n");
    printf("3. Quit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

int main() {
    char board[SIZE][SIZE]; //board itself
    Ship ships[MAX_SHIPS] = { //The ships
        {"Air Force Academy", 5, 0, 0, {{0,0}}},
        {"Valencia Destroyer", 4, 0, 0, {{0,0}}},
        {"Seminole State Ship", 3, 0, 0, {{0,0}}},
        {"Eskimo University", 3, 0, 0, {{0,0}}},
        {"Deland High School", 2, 0, 0,{{0,0}}}
    };
    int numShips = MAX_SHIPS; //How many ships there are
    int shots = 0; //Shots taken

    if (!loadGameState(board, ships, &numShips, &shots)) { //Loads the game state if there is one
        initializeBoard(board);
        placeShips(board, ships, numShips);
    }

    while (1) { //Infinite loop to keep running until the user chooses to quit 
        displayBoard(board, ships, numShips); //Shows the board
        int choice = displayMenu(); //Gets the user choice after running the display menu
        
        if (choice == 1) { //Case if the user wants to shoot
            takePlayerTurn(board, ships, numShips, &shots); //Make a shot
            saveGameState(board, ships, numShips, shots); //Save the board
            if (checkGameOver(ships, numShips)) { //Check if all ships were sunk
                printf("All ships sunk! You win!\n");
                printf("Total shots: %d\n", shots);
                saveScore(shots);
                remove(STATE_FILE); // Delete the saved game state
                break;
            }
        } else if (choice == 2) { //Shows the scores
            displayTopScores();
        } else if (choice == 3) { //Quits game
            printf("Exiting game...\n");
            saveGameState(board, ships, numShips, shots); //Saves the board one more time just to be safe
            break;
        } else { //Check for invalid choices
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
