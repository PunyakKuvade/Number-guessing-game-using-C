#include"game.h"//game header file
#include<stdlib.h>// for rand() and srand()
#include<time.h>//for time() required for seeding
guess_result_t CheckGuess(int guess, int secretNumber){
    if(guess<secretNumber)// if guess is less than secret number
    return guess_too_low;
    else if(guess>secretNumber)// if guess is greater than secret number
    return guess_too_high;
    else// if guess is equal to secret number
    return guess_correct;
}//function to check the guess against the secret number
void StartGame(void) {
    // This only needs to be called once in the entire program
    srand(time(NULL));// Seed the random number generator
}

int GetSecretNumber(int min, int max) {
    // This generates the number within the range
    return (rand() % (max - min + 1)) + min;
}