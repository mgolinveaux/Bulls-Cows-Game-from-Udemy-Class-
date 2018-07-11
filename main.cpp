/* This is the console executable, that makes use of the BullCow class. 
This acts as the view in a MVC pattern, and is responsible for all
user interation. For game logic see the FBullCowGame class. 
*/

#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"
 
//to make syntax Unreal friendly 
using FText = std::string;
using int32 = int;

// function prototypes as outside a class 
void PrintIntro();
void PlayGame();
FText GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGAME; //instantiate a new game, which we re-use across plays

//the entry point for our application 
int main()
{
	std::cout << BCGAME.GetCurrentTry();

	bool bPlayAgain = false;
	do 
	{
		PrintIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	}
	while (bPlayAgain);

	std::cout << std::endl;
	return 0; //exit application
}


// introduce the game
void PrintIntro()
{
	std::cout << "\nWelcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "        }     {                               " << std::endl;
	std::cout << "        }_____{         ()________()                   " << std::endl;
	std::cout << "        ( 0 0 )          (  0 0   )                " << std::endl;
	std::cout << "        \\     /           \\      /              " << std::endl;
	std::cout << "         \\o o/             \\o  o/              " << std::endl;
	std::cout << "       -----------------------------------          " << std::endl;
	std::cout << "    |   BULLS        AND          COWS      |    " << std::endl;
	std::cout << "       -----------------------------------            " << std::endl;
	std::cout << std::endl;

	std::cout << "Can you guess the " << BCGAME.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";
	std::cout << std::endl;
	return;
}

//plays a single game to completion 
void PlayGame()
{
	BCGAME.Reset();
	int32 MaxTries = BCGAME.GetMaxTries();
	std::cout << MaxTries << std::endl;

	//loop asking for guesses while the game
	//is NOT won and there are still tries remaining 
	while (!BCGAME.IsGameWon() && BCGAME.GetCurrentTry() <= MaxTries)
	{
		FText Guess = GetValidGuess();  

		//submit vald guess of the game, and receive counts
		FBullCowCount BullCowCount = BCGAME.SubmitValidGuess(Guess);
		
		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << "\n\n";
	}

	PrintGameSummary();
	return;
}

// loop continuely until the user gives a valid guess 
FText GetValidGuess() 
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		// get a guess from player 
		int32 CurrentTry = BCGAME.GetCurrentTry();
		std::cout << "\nTry " << CurrentTry << " of " << BCGAME.GetMaxTries();
		std::cout << ". Enter your guess: ";
	    std::getline(std::cin, Guess);

		//check status and give feedback
		Status = BCGAME.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGAME.GetHiddenWordLength() << "letter word.\n\n";
			break;

		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n\n";
			break;

		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letters.\n\n";
			break;

		default:
			// assuming the guess is valid 
			break;
		}
	} while (Status != EGuessStatus::OK); // keep looping untill we get no errors 
	return Guess;
}

//ask to play again 
bool AskToPlayAgain()
{
	std::cout << "Do you want to play again with the same hidden word? (y/n) ";
	FText Response = "";
	std::getline(std::cin, Response);

	return (Response[0] == 'y') || (Response[0] == 'Y');
}

void PrintGameSummary() 
{
	if (BCGAME.IsGameWon())
	{
		std::cout << "WELL DONE - YOU WIN!\n";
	}
	else
	{
		std::cout << "Better Luck next time!\n";
	}
}
