#include "raylib.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h> // Make sure this is included for atoi()

int main(void)
{
    // --- Window Initialization ---
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Number Guessing Game");
    Image icon = LoadImage("resources/logo-1.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    StartGame(); // Seed the random number generator once

    // --- Asset Loading ---
    Texture2D logo = LoadTexture("resources/window logo-2.png");

    // --- Game State Variables ---
    Game_Screen currentScreen = Screen_Welcome;
    int minRange = 1, maxRange = 100, secretNumber = 0, playerGuess = 0;
    guess_result_t lastResult;
    char feedbackText[100] = "Set a range to begin!";

    // --- UI Element Definitions ---
    Rectangle playButton = { screenWidth / 2 - 125, 450, 250, 60 };
    Rectangle minRangeBox = { screenWidth / 2 - 200, 250, 150, 50 };
    Rectangle maxRangeBox = { screenWidth / 2 + 50, 250, 150, 50 };
    Rectangle setRangeButton = { screenWidth / 2 - 75, 320, 150, 50 };
    Rectangle guessBox = { screenWidth / 2 - 150, 420, 300, 50 };
    Rectangle guessButton = { screenWidth / 2 - 75, 490, 150, 50 };

    int activeBox = 0; // 0: none, 1: min, 2: max, 3: guess
    char minRangeText[4] = "1";
    char maxRangeText[4] = "100";
    char guessText[4] = "";
    int letterCountMin = 1, letterCountMax = 3, letterCountGuess = 0;
    int framesCounter = 0;

    SetTargetFPS(60);

    // --- Main Game Loop ---
    while (!WindowShouldClose())
    {
        // --- Update Logic (Runs every frame) ---
        Vector2 mousePoint = GetMousePosition();
        framesCounter++;

        switch (currentScreen)
        {
            case Screen_Welcome:
            {
                if (CheckCollisionPointRec(mousePoint, playButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentScreen = Screen_Game;
                }
            } break;

            case Screen_Game:
            {
                // **FIXED LOGIC**: Set active box only on mouse press
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mousePoint, minRangeBox)) activeBox = 1;
                    else if (CheckCollisionPointRec(mousePoint, maxRangeBox)) activeBox = 2;
                    else if (CheckCollisionPointRec(mousePoint, guessBox)) activeBox = 3;
                    else activeBox = 0; // Deactivate if clicking elsewhere
                }

                // --- Text Input Logic (Only works if a box is active) ---
                int key = GetCharPressed();
                if ((key >= '0' && key <= '9')) {
                    if (activeBox == 1 && letterCountMin < 3) {
                        minRangeText[letterCountMin++] = (char)key;
                        minRangeText[letterCountMin] = '\0';
                    } else if (activeBox == 2 && letterCountMax < 3) {
                        maxRangeText[letterCountMax++] = (char)key;
                        maxRangeText[letterCountMax] = '\0';
                    } else if (activeBox == 3 && letterCountGuess < 3) {
                        guessText[letterCountGuess++] = (char)key;
                        guessText[letterCountGuess] = '\0';
                    }
                }
                // Backspace logic
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (activeBox == 1 && letterCountMin > 0) {
                        minRangeText[--letterCountMin] = '\0';
                    } else if (activeBox == 2 && letterCountMax > 0) {
                        maxRangeText[--letterCountMax] = '\0';
                    } else if (activeBox == 3 && letterCountGuess > 0) {
                        guessText[--letterCountGuess] = '\0';
                    }
                }

                // --- Button Logic ---
                if (CheckCollisionPointRec(mousePoint, setRangeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    minRange = atoi(minRangeText);
                    maxRange = atoi(maxRangeText);
                    secretNumber = GetSecretNumber(minRange, maxRange);
                    sprintf(feedbackText, "Range set! Guess a number from %d to %d.", minRange, maxRange);
                }
                if (CheckCollisionPointRec(mousePoint, guessButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    playerGuess = atoi(guessText);
                    lastResult = CheckGuess(playerGuess, secretNumber);
                    if (lastResult == guess_too_low) sprintf(feedbackText, "'%d' is too low! Try again.", playerGuess);
                    else if (lastResult == guess_too_high) sprintf(feedbackText, "'%d' is too high! Try again.", playerGuess);
                    else if (lastResult == guess_correct) {
                        currentScreen = Screen_Win;
                    }
                    // Clear guess box for next attempt
                    letterCountGuess = 0;
                    guessText[0] = '\0';
                }
            } break;

            case Screen_Win:
            {
                if (CheckCollisionPointRec(mousePoint, playButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    // Reset game variables for a new round
                    minRange = atoi(minRangeText);
                    maxRange = atoi(maxRangeText);
                    secretNumber = GetSecretNumber(minRange, maxRange);
                    sprintf(feedbackText, "New game! Guess a number from %d to %d.", minRange, maxRange);
                    currentScreen = Screen_Game;
                }
            } break;
        }

        // --- Drawing (Runs every frame) ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            switch (currentScreen)
            {
                case Screen_Welcome:
                {
                    // **FIXED LOGIC**: Use DrawTexturePro to scale the logo
                    Rectangle source = { 0, 0, (float)logo.width, (float)logo.height };
                    float destWidth = 400; // Set desired width
                    float destHeight = destWidth * ((float)logo.height / logo.width); // Maintain aspect ratio
                    Rectangle dest = { screenWidth / 2 - destWidth / 2, 80, destWidth, destHeight };
                    DrawTexturePro(logo, source, dest, (Vector2){0,0}, 0, WHITE);

                    DrawRectangleRec(playButton, SKYBLUE);
                    DrawText("Let's Play!", playButton.x + 45, playButton.y + 15, 30, WHITE);
                } break;

                case Screen_Game:
                {
                    // Draw feedback text
                    DrawText(feedbackText, screenWidth/2 - MeasureText(feedbackText, 20)/2, 150, 20, BLACK);

                    // Draw range boxes
                    DrawText("Enter Range", screenWidth/2 - MeasureText("Enter Range", 20)/2, 220, 20, GRAY);
                    DrawRectangleRec(minRangeBox, LIGHTGRAY);
                    DrawRectangleRec(maxRangeBox, LIGHTGRAY);
                    DrawText(minRangeText, minRangeBox.x + 5, minRangeBox.y + 10, 40, MAROON);
                    DrawText(maxRangeText, maxRangeBox.x + 5, maxRangeBox.y + 10, 40, MAROON);
                    // Draw blinking cursor
                    if (activeBox == 1 && (framesCounter/30)%2 == 0) DrawText("_", minRangeBox.x + 8 + MeasureText(minRangeText, 40), minRangeBox.y + 15, 40, MAROON);
                    if (activeBox == 2 && (framesCounter/30)%2 == 0) DrawText("_", maxRangeBox.x + 8 + MeasureText(maxRangeText, 40), maxRangeBox.y + 15, 40, MAROON);

                    DrawRectangleRec(setRangeButton, SKYBLUE);
                    DrawText("Set", setRangeButton.x + 50, setRangeButton.y + 10, 30, WHITE);

                    // Draw guess box
                    DrawText("Your Guess", screenWidth/2 - MeasureText("Your Guess", 20)/2, 390, 20, GRAY);
                    DrawRectangleRec(guessBox, LIGHTGRAY);
                    DrawText(guessText, guessBox.x + 5, guessBox.y + 10, 40, DARKGREEN);
                    if (activeBox == 3 && (framesCounter/30)%2 == 0) DrawText("_", guessBox.x + 8 + MeasureText(guessText, 40), guessBox.y + 15, 40, DARKGREEN);

                    DrawRectangleRec(guessButton, SKYBLUE);
                    DrawText("Guess", guessButton.x + 30, guessButton.y + 10, 30, WHITE);

                } break;

                case Screen_Win:
                {
                    DrawText("Congratulations!", screenWidth/2 - MeasureText("Congratulations!", 40)/2, 150, 40, GOLD);
                    DrawText("You guessed the number!", screenWidth/2 - MeasureText("You guessed the number!", 30)/2, 220, 30, GRAY);
                    DrawRectangleRec(playButton, SKYBLUE);
                    DrawText("Play Again?", playButton.x + 30, playButton.y + 15, 30, WHITE);
                } break;
            }

        EndDrawing();
    }

    // --- De-Initialization ---
    UnloadTexture(logo);
    CloseWindow();

    return 0;
}