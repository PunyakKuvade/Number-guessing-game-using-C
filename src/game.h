#ifndef GAME_H
#define GAME_H// to prevent multiple calling of this header file
typedef enum { guess_too_low, guess_too_high, guess_correct } guess_result_t;// defining an enum for the result of the guess
typedef enum{Screen_Welcome, Screen_Game, Screen_Win} Game_Screen;// defining an enum for the different screens in the game
void StartGame(void);
int GetSecretNumber(int min, int max);// function prototype for getting the secret number
guess_result_t CheckGuess(int guess, int secretNumber);// function prototype for checking the guess
#endif // GAME_H