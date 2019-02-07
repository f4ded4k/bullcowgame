/* 
This file contains all the user interfacing code which helps the user communicate with the game instance 
through the console. For game logic see FBullCowGame.h
*/

#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"

// unreal friendly typenames
using FText = std::string;
using int32 = int;


// function prototypes
void PrintIntro();
void PlaySingleRound(FBullCowGame&);
bool AskIfPlayAgain();
bool AskIfPlaySameWord();
FText GetValidGuess(const FBullCowGame&);
void PrintInterRoundMessage();
void PrintGameResults(const FBullCowGame&);
int32 AskNewWordLength();

// entry point to the program
int main() 
{
	
	PrintIntro();
	int32 NewHiddenWordLength = AskNewWordLength();
	std::cout << std::endl;

	// create a game instance to play with
	FBullCowGame BCGame = FBullCowGame(NewHiddenWordLength);

	// the endline character is flushed before getline() can be used correctly
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	bool bPlayAgain = false;

	// loop as long as the user wants to keep playing the game
	do 
	{
		PlaySingleRound(BCGame);

		bPlayAgain = AskIfPlayAgain();

		// if the user wants to play again, get response to more details
		if (bPlayAgain)
		{
			// ask if the user wants to play with the same word
			bool bSameWord = AskIfPlaySameWord();
			if (bSameWord)
			{
				// reset the game instance with the same word
				BCGame.ResetWithNewWord(BCGame.GetHiddenWordLength());
			}
			else
			{
				// ask and reset the game instance with the new word
				int32 NewWordLength = AskNewWordLength();
				BCGame.ResetWithNewWord(NewWordLength);

				// the endline character is flushed before getline() can be used correctly
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			std::cout << std::endl;
			PrintInterRoundMessage();
		}
	} 
	while (bPlayAgain);

	return 0; // exit the program
}

void PrintIntro() 
{
	std::cout << "===============================================" << std::endl;
	std::cout << "Welcome to Bulls and Cows, a logical word game!" << std::endl;
	std::cout << "===============================================" << std::endl << std::endl;
}

void PrintInterRoundMessage()
{
	std::cout << "=================" << std::endl;
	std::cout << "Let's play again!" << std::endl;
	std::cout << "=================" << std::endl << std::endl;
}

// plays one round of the game till completion
void PlaySingleRound(FBullCowGame& BCGame)
{
	// loop until the game is either won or lost
	while (BCGame.GetGameStatus() == EGameStatus::In_Progress)
	{
		std::cout << "Current turn " << BCGame.GetCurrentTurn() << " out of " << BCGame.GetMaxTurn() << " turns :  ";

		// get a valid guess word from the user
		FText ValidGuessWord = GetValidGuess(BCGame);

		// submit a valid guess word
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(ValidGuessWord);

		// give the results to the user in the form of number of bulls and cows
		std::cout << "Bulls : " << BullCowCount.Bulls << ", Cows : " << BullCowCount.Cows << std::endl << std::endl;
	}

	PrintGameResults(BCGame);
}

// ask if the user wants to play again
bool AskIfPlayAgain()
{
	std::cout << "Do you want to play again? (Y/N) : ";

	FText Response = "";
	std::getline(std::cin, Response);

	return (Response[0] == 'y') || (Response[0] == 'Y'); // consider answer as YES if first character is 'y' or 'Y'
}

// ask if the user wants to play with the same word as previous round
bool AskIfPlaySameWord()
{
	std::cout << "Do you want to play with the same word? (Y/N) : ";

	FText Response = "";
	std::getline(std::cin, Response);

	return (Response[0] == 'y') || (Response[0] == 'Y'); // consider answer as YES if first character is 'y' or 'Y'
}

// get a valid guess word from the user
FText GetValidGuess(const FBullCowGame &BCGame)
{
	FText Guess = "";
	EGuessStatus GuessStatus = EGuessStatus::Uninitiated;

	// loop indefinitely until a valid guess is given
	do
	{
		std::getline(std::cin, Guess);
		GuessStatus = BCGame.GetGuessStatus(Guess);

		// check all possible errors the user may input and give feedback how to correct them
		switch (GuessStatus)
		{
		case EGuessStatus::Not_alphabetic:
			std::cout << "Enter a word containing only lowercase english letters :  ";
			break;

		case EGuessStatus::Not_Isogram:
			std::cout << "Enter a word containing no repeating letters :  ";
			break;

		case EGuessStatus::Not_Lowercase:
			std::cout << "Enter a word containing only lowercase english letters :  ";
			break;

		case EGuessStatus::Unequal_Length:
			std::cout << "Enter a word containing " << BCGame.GetHiddenWordLength() << " letters :  ";
			break;

		default:
			// the guess is valid
			break;
		}
	} 
	while (GuessStatus != EGuessStatus::OK);

	return Guess;
}

// ask the user to input the new word length to play with
int32 AskNewWordLength()
{
	std::cout << "Enter the length of the hidden word you want to play with (within 3 and 7) : ";

	int32 NewLength = 0;
	std::cin >> NewLength;

	// loop until the user enters a valid word length
	while (NewLength < 3 || NewLength > 7)
	{
		std::cout << "Please enter a valid length between 3 and 7 : ";
		std::cin >> NewLength;
	}

	return NewLength;
}

void PrintGameResults(const FBullCowGame &BCGame)
{
	if (BCGame.GetGameStatus() == EGameStatus::Won)
	{
		std::cout << "=================================" << std::endl;
		std::cout << "Congratulations! that's the word!" << std::endl;
		std::cout << "=================================" << std::endl;
	}
	else
	{
		std::cout << "==============================" << std::endl;
		std::cout << "Oh no! You are out of guesses." << std::endl;
		std::cout << "==============================" << std::endl;
	}

	std::cout << std::endl;
}
