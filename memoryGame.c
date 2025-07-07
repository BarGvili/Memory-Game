#include <cviauto.h>
#include <userint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h> 
#include "memoryGame.h"
#define BOARD_SIZE 4 // We will play on a 4x4 board

typedef struct {
    int board[BOARD_SIZE][BOARD_SIZE];
    int revealed[BOARD_SIZE][BOARD_SIZE];
    int moves;
    int pairsFound;
    int firstCardX;
    int firstCardY;
    int secondCardX;
    int secondCardY;
    int gameState;
	int timeElapsed;
} GameState;

#define MAX_SCORES 10
#define FILENAME "highscores.txt"

typedef struct {
    char name[50];
    int moves;
    int time;
} HighScore;

static HighScore g_highScores[MAX_SCORES];
static int g_numScores = 0; // How many scores are currently loaded

// A global variable to hold the entire state of the current game
static GameState g_game;

// A global array to map button IDs to board positions
static int button_ids[BOARD_SIZE][BOARD_SIZE] = {
    {GAME_PANEL_COMMANDBUTTON_16, GAME_PANEL_COMMANDBUTTON_15, GAME_PANEL_COMMANDBUTTON_14, GAME_PANEL_COMMANDBUTTON_13},
    {GAME_PANEL_COMMANDBUTTON_12, GAME_PANEL_COMMANDBUTTON_11, GAME_PANEL_COMMANDBUTTON_10, GAME_PANEL_COMMANDBUTTON_9},
    {GAME_PANEL_COMMANDBUTTON_8,  GAME_PANEL_COMMANDBUTTON_7,  GAME_PANEL_COMMANDBUTTON_6,  GAME_PANEL_COMMANDBUTTON_5},
    {GAME_PANEL_COMMANDBUTTON_4,  GAME_PANEL_COMMANDBUTTON_3,  GAME_PANEL_COMMANDBUTTON_2,  GAME_PANEL_COMMANDBUTTON_1}
};

// Global variables to hold our panel handles
static int g_mainPanel;
static int g_gamePanel;
static int g_scoresPanel;

// Forward declarations for callbacks
int CVICALLBACK PanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int CVICALLBACK FlipBack_Timer_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);

void SaveScoresToFile(void);
void LoadAndDisplayScores(void);

// --- Game Logic Functions ---
void StartNewGame()
{
    int i, j, k;
    int temp_deck[BOARD_SIZE * BOARD_SIZE];
    
    // Fix for the warning: initialize the array to zeros
    memset(temp_deck, 0, sizeof(temp_deck));

    for (i = 0; i < (BOARD_SIZE * BOARD_SIZE) / 2; i++) {
        temp_deck[i * 2] = i + 1;
        temp_deck[i * 2 + 1] = i + 1;
    }
    for (i = (BOARD_SIZE * BOARD_SIZE) - 1; i > 0; i--) {
        j = rand() % (i + 1);
        k = temp_deck[i];
        temp_deck[i] = temp_deck[j];
        temp_deck[j] = k;
    }
    k = 0;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            g_game.board[i][j] = temp_deck[k];
            g_game.revealed[i][j] = 0;
            k++;
        }
    }
    g_game.moves = 0;
    g_game.pairsFound = 0;
    g_game.gameState = 0;
    g_game.firstCardX = -1;
    g_game.firstCardY = -1;
	g_game.timeElapsed = 0;
    printf("New Game Started. Board layout:\n");
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            printf("%d ", g_game.board[i][j]);
        }
        printf("\n");
    }
	// --- Reset the button labels on the UI ---
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            int control_id = button_ids[i][j];
            SetCtrlAttribute(g_gamePanel, control_id, ATTR_LABEL_TEXT, "?");
        }
    }
    
	
	// Also reset the moves counter display
	SetCtrlAttribute(g_gamePanel, GAME_PANEL_GAME_TXT_MOVES, ATTR_CTRL_VAL, "Moves: 0");

	// --- ADD THIS CODE TO START THE GAME TIMER ---
	// First, reset the time display
	SetCtrlAttribute(g_gamePanel, GAME_PANEL_GAME_TXT_TIME, ATTR_CTRL_VAL, "Time: 00:00");

	// Set the timer to "tick" every 1.0 seconds
	SetCtrlAttribute(g_gamePanel, GAME_PANEL_GAME_TIMER, ATTR_INTERVAL, 1.0);
	// Enable the timer. It will now start ticking.
	SetCtrlAttribute(g_gamePanel, GAME_PANEL_GAME_TIMER, ATTR_ENABLED, 1);
}

// Main entry point of the program
int main(int argc, char *argv[])
{
    if (InitCVIRTE(0, argv, 0) == 0)
        return -1;
    srand(time(NULL));
    g_mainPanel = LoadPanel(0, "memoryGame.uir", MAIN_PANEL);
    g_gamePanel = LoadPanel(0, "memoryGame.uir", GAME_PANEL);
    g_scoresPanel = LoadPanel(0, "memoryGame.uir", SCORE_PNL);
    InstallPanelCallback(g_mainPanel, PanelCallback, 0);
    DisplayPanel(g_mainPanel);
    RunUserInterface();
    DiscardPanel(g_mainPanel);
    return 0;
}

// General callback for the main panel
int CVICALLBACK PanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE) {
        QuitUserInterface(0);
    }
    return 0;
}

// --- Main Panel Button Callbacks ---
int CVICALLBACK MainPanel_NewGame_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT) {
        StartNewGame();
        SetPanelAttribute(g_mainPanel, ATTR_VISIBLE, 0);
        SetPanelAttribute(g_gamePanel, ATTR_VISIBLE, 1);
    }
    return 0;
}
int CVICALLBACK MainPanel_Scores_Callback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT)
    {
        // First, load and show the scores
        LoadAndDisplayScores();
        
        // Then, show the panel
        SetPanelAttribute(g_mainPanel, ATTR_VISIBLE, 0);
        SetPanelAttribute(g_scoresPanel, ATTR_VISIBLE, 1);
    }
    return 0;
}

int CVICALLBACK MainPanel_Exit_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT) {
        QuitUserInterface(0);
    }
    return 0;
}
int CVICALLBACK BackToMenu_Callback(int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT) 
    {
        // --- ADD THIS LINE ---
        // Stop the game timer to prevent it from running in the background.
        // We do this just in case the user exits mid-game.
        SetCtrlAttribute(g_gamePanel, GAME_PANEL_GAME_TIMER, ATTR_ENABLED, 0);
        // ---------------------

        // Hide the current panel (either game or scores)
        SetPanelAttribute(panel, ATTR_VISIBLE, 0);
        
        // Show the main menu
        SetPanelAttribute(g_mainPanel, ATTR_VISIBLE, 1);
    }
    return 0;
}

// --- Game Panel Callbacks ---
int CVICALLBACK Board_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int row = -1, col = -1;
    char card_text[4];
    int card_value;

    if (g_game.gameState == 2) { // Corrected brace placement
        return 0;
    }

    if (event == EVENT_COMMIT) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (control == button_ids[i][j]) {
                    row = i;
                    col = j;
                    break;
                }
            }
            if (row != -1) break;
        }

        if (row != -1) {
            if (g_game.revealed[row][col] == 2) {
                return 0;
            }
            if (g_game.gameState == 0) {
                card_value = g_game.board[row][col];
                sprintf(card_text, "%d", card_value);
                SetCtrlAttribute(panel, control, ATTR_LABEL_TEXT, card_text);
                g_game.firstCardX = row;
                g_game.firstCardY = col;
                g_game.revealed[row][col] = 1;
				g_game.moves++;
                char moves_text[32];
                sprintf(moves_text, "Moves: %d", g_game.moves);
                // Use the correct constant name from your .h file
                SetCtrlAttribute(g_gamePanel, GAME_PANEL_GAME_TXT_MOVES, ATTR_CTRL_VAL, moves_text);
                g_game.gameState = 1;
            } else if (g_game.gameState == 1) {
                if (row == g_game.firstCardX && col == g_game.firstCardY) {
                    return 0;
                }
                card_value = g_game.board[row][col];
                sprintf(card_text, "%d", card_value);
                SetCtrlAttribute(panel, control, ATTR_LABEL_TEXT, card_text);
                g_game.revealed[row][col] = 1;

                int first_card_value = g_game.board[g_game.firstCardX][g_game.firstCardY];
                int second_card_value = g_game.board[row][col];

                if (first_card_value == second_card_value) {
                    printf("Match found!\n");
                    g_game.revealed[g_game.firstCardX][g_game.firstCardY] = 2;
                    g_game.revealed[row][col] = 2;
                    g_game.gameState = 0;
                    g_game.pairsFound++;
					if (g_game.pairsFound == (BOARD_SIZE * BOARD_SIZE) / 2)
					{
					    SetCtrlAttribute(g_gamePanel, GAME_PANEL_GAME_TIMER, ATTR_ENABLED, 0);

					    // --- NEW: Handle new high score ---
					    char playerName[51];
					    // Prompt for player's name. If they press Cancel, it returns 1.
					    if (PromptPopup("Victory!", "Congratulations, you won!\nEnter your name:", playerName, 50) == 0)
					    {
					        if (g_numScores < MAX_SCORES) {
					            strcpy(g_highScores[g_numScores].name, playerName);
					            g_highScores[g_numScores].moves = g_game.moves;
					            g_highScores[g_numScores].time = g_game.timeElapsed;
					            g_numScores++;
            
					            // TODO: Add sorting logic later
            
					            SaveScoresToFile();
					        }
					    }
					    // ------------------------------------
    
					    // Go back to the main menu
					    SetPanelAttribute(g_gamePanel, ATTR_VISIBLE, 0);
					    SetPanelAttribute(g_mainPanel, ATTR_VISIBLE, 1);
					}
                } else {
                    printf("No match.\n");
				    // Store the position of the second card
				    g_game.secondCardX = row;
				    g_game.secondCardY = col;

				    // Change game state to "busy" (2).
				    // The timer will see this state in its next tick and flip the cards.
				    g_game.gameState = 2; 
                }
            }
        }
    }
    return 0;
}

int CVICALLBACK FlipBack_Timer_Callback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_TIMER_TICK)
    {
        // --- Part A: Always update the game time ---
        g_game.timeElapsed++;
        
        // Format the time into MM:SS format
        int minutes = g_game.timeElapsed / 60;
        int seconds = g_game.timeElapsed % 60;
        char time_text[32];
        sprintf(time_text, "Time: %02d:%02d", minutes, seconds);

        // Update the text on the screen
        SetCtrlAttribute(g_gamePanel, GAME_PANEL_GAME_TXT_TIME, ATTR_CTRL_VAL, time_text);


        // --- Part B: Check if we need to flip cards back ---
        // We flip cards ONLY if the game state is "busy" (2)
        if (g_game.gameState == 2)
        {
            // Find the UI controls for the two cards
            int first_card_control_id = button_ids[g_game.firstCardX][g_game.firstCardY];
            int second_card_control_id = button_ids[g_game.secondCardX][g_game.secondCardY];

            // Flip them back
            SetCtrlAttribute(g_gamePanel, first_card_control_id, ATTR_LABEL_TEXT, "?");
            SetCtrlAttribute(g_gamePanel, second_card_control_id, ATTR_LABEL_TEXT, "?");
            
            // Mark them as hidden again
            g_game.revealed[g_game.firstCardX][g_game.firstCardY] = 0;
            g_game.revealed[g_game.secondCardX][g_game.secondCardY] = 0;
            
            // Set the game state back to normal
            g_game.gameState = 0; 
        }
    }
    return 0;
}


void SaveScoresToFile()
{
    FILE *file = fopen(FILENAME, "w"); // "w" for write, creates/overwrites the file
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }
    
    for (int i = 0; i < g_numScores; i++) {
        fprintf(file, "%s %d %d\n", g_highScores[i].name, g_highScores[i].moves, g_highScores[i].time);
    }
    
    fclose(file);
}

void LoadAndDisplayScores()
{
    FILE *file = fopen(FILENAME, "r"); // "r" for read
    g_numScores = 0;
    
    if (file != NULL) {
        while (g_numScores < MAX_SCORES && 
               fscanf(file, "%s %d %d", g_highScores[g_numScores].name, &g_highScores[g_numScores].moves, &g_highScores[g_numScores].time) != EOF)
        {
            g_numScores++;
        }
        fclose(file);
    }

    // --- Display scores in the table ---
    // First, clear the table
    DeleteTableRows(g_scoresPanel, SCORE_PNL_TABLE, 1, -1);
    
    // Insert new rows and fill them
    InsertTableRows(g_scoresPanel, SCORE_PNL_TABLE, 1, g_numScores, VAL_CELL_STRING);
    for (int i = 0; i < g_numScores; i++)
    {
        char tempStr[100];
        SetTableCellVal(g_scoresPanel, SCORE_PNL_TABLE, MakePoint(1, i + 1), g_highScores[i].name);
        sprintf(tempStr, "%d", g_highScores[i].moves);
        SetTableCellVal(g_scoresPanel, SCORE_PNL_TABLE, MakePoint(2, i + 1), tempStr);
        sprintf(tempStr, "%d", g_highScores[i].time);
        SetTableCellVal(g_scoresPanel, SCORE_PNL_TABLE, MakePoint(3, i + 1), tempStr);
    }
}
