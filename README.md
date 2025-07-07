# CVI Memory Game - Final Project Report
## 1. Introduction

This project implements the classic memory matching game in the LabWindows/CVI development environment. The goal of this project is to demonstrate proficiency in a wide range of CVI tools and concepts, from building a multi-panel graphical user interface (GUI) to managing events, game logic, and working with files for data persistence.

The application consists of three separate panels for easy navigation: a main menu, the game board itself, and a high-score screen. The game board is fully interactive, featuring a timer that tracks the game duration and a counter for the number of moves made. Upon successful completion of a game, the application allows the player to save their result to an external file.

The project was built with an emphasis on separating logic from presentation, efficient use of graphical controls, and managing different game states using state variables.

## 2. Graphical Windows

### 2.1 Main Menu (Main Panel)
This is the first window the user encounters. It provides navigation to the application's main features: starting a new game, viewing the high scores table, or exiting the program.

![Main Menu](main_menu.png)

### 2.2 Game Panel
The heart of the application. This panel contains a 4x4 game board, a move counter, a timer, and a button to return to the main menu.

![Game Panel](game_board.png)

### 2.3 Scores Panel
This panel displays the best scores achieved in the game. The data is loaded from an external file, allowing for progress tracking. The panel includes a table with columns for the player's name, number of moves, and time, along with a button to close and return to the menu.

![Scores Panel](scores_panel.png)

---

## 3. User Guide

1.  **Start the Program:** Upon launching, the main menu will appear.
2.  **Start a Game:** Click the "New Game" button to navigate to the game board.
3.  **Gameplay:**
    *   Click on two cards to reveal their numbers.
    *   If the numbers match, a pair is found, and the cards remain face up.
    *   If the numbers do not match, the cards will flip back over after a short delay.
    *   The move and time counters update automatically.
4.  **End of Game:** The game ends when all 8 pairs are found. A victory message will pop up, prompting you to enter your name to save your score.
    ![Victory Prompt](victory_prompt.png)
5.  **View Scores:** From the main menu, click "Record Table" to view the high scores.
6.  **Exit:** You can exit the program by clicking the "EXIT" button on the main menu.

---

## 4. Algorithms and Key Functions

### 4.1 `StartNewGame()`
This function is responsible for all preparations for a new game. It performs the following actions:
- Creates a temporary array containing 8 pairs of numbers (from 1 to 8).
- Shuffles the array using the **Fisher-Yates shuffle** algorithm to ensure a random order in every game.
- "Deals" the shuffled cards into a 4x4 logical matrix that represents the game board.
- Resets all game state variables (move counter, pairs found, time, etc.).
- Resets the graphical display of the board (turning all cards to "?") and starts the game timer.

### 4.2 `Board_Callback()`
This is the central callback function that handles all clicks on the game board cards. It manages the turn-based logic of the game:
- **Card Identification:** The function uses a mapping array (`button_ids`) to identify the row and column of the clicked button.
- **State Management:** Using the `g_game.gameState` variable, the function knows whether the player is selecting the first or second card of a turn.
- **Match Checking:** After the second card is selected, the function compares the values of the two cards. If a match is found, it updates the pair counter and checks if the game is over. If there's no match, it changes the game state to allow the timer to flip the cards back.

### 4.3 `FlipBack_Timer_Callback()`
This callback function is triggered by a timer that runs continuously every second. It is responsible for two tasks:
1.  **Time Update:** On every "tick" of the timer, the function updates the elapsed time variable and displays it on the screen in MM:SS format.
2.  **Flipping Cards:** The function checks the game state. If the state is "error" (`gameState == 2`), it identifies the last two revealed cards and flips them back to their hidden state.

### 4.4 `SaveScoresToFile()` & `LoadAndDisplayScores()`
This pair of functions implements the file I/O:
- **`SaveScoresToFile`**: After a win, this function opens a text file (`highscores.txt`) in write mode (`w`) and saves the entire list of high scores.
- **`LoadAndDisplayScores`**: When entering the high scores screen, this function opens the same file in read mode (`r`), reads all the saved scores into an array in memory, and then populates the graphical table with this data.

---
