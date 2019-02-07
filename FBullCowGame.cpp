/*
Implementation of all the game logic required to run the game instance. 
*/

#pragma once

#include <unordered_map>
#include <unordered_set>
#include "FBullCowGame.h"

// unreal friendly typenames
using int32 = int;
using FString = std::string;
template <typename ValueType>
using TSet = std::unordered_set<ValueType>;
template <typename KeyType, typename ValueType>
using TMap = std::unordered_map<KeyType, ValueType>;

// constructor
FBullCowGame::FBullCowGame(int32 HiddenWordLength) : 
	WordLengthToWord({
		{3,"ant"},{4,"quiz"},{5,"plant"},{6,"friend"},{7,"academy"} 
		}),
	WordLengthToMaxTurn({
		{3,4},{4,6},{5,8},{6,11},{7,16}
		})
{
	ResetWithNewWord(HiddenWordLength);
}

int32 FBullCowGame::GetCurrentTurn() const { return MyCurrentTurn; }

int32 FBullCowGame::GetMaxTurn() const { return WordLengthToMaxTurn.at((int32)MyCurrentHiddenWord.size()); }

int32 FBullCowGame::GetHiddenWordLength() const { return (int32)MyCurrentHiddenWord.size(); }

EGameStatus FBullCowGame::GetGameStatus() const { return GameStatus; }

// returns the guess status to the user interface corresponding
// to the user given word containing specific error if any
EGuessStatus FBullCowGame::GetGuessStatus(const FString &GuessWord) const
{
	EGuessStatus GuessStatus = EGuessStatus::Uninitiated;
	
	if (!IsAlphabetic(GuessWord))
	{
		GuessStatus = EGuessStatus::Not_alphabetic;
	}
	else if (!IsLowercase(GuessWord))
	{
		GuessStatus = EGuessStatus::Not_Lowercase;
	}
	else if (GuessWord.size() != MyCurrentHiddenWord.size())
	{
		GuessStatus = EGuessStatus::Unequal_Length;
	}
	else if (!IsIsogram(GuessWord))
	{
		GuessStatus = EGuessStatus::Not_Isogram;
	}
	else {
		GuessStatus = EGuessStatus::OK;
	}

	return GuessStatus;
}

// plays a turn using a valif guess and returns the
// number of bulls and cows corresponding to the user given input
FBullCowCount FBullCowGame::SubmitValidGuess(const FString &GuessWord)
{
	// the object that holds the results to return, see FBullCowGame.h for declaration
	FBullCowCount BullCowCount;

	// container to store pairs of (letter, the index where it occured) to find number of bulls and cows
	TMap<char, int32> LetterToPosition; 

	// loop through the guess word
	for (int32 LetterPos = 0; LetterPos < GuessWord.size(); LetterPos++)
	{
		// insert pairs of (letter, it's index) into the container for each character
		LetterToPosition[GuessWord[LetterPos]] = LetterPos;
	}

	// loop through the hidden word
	for (int32 LetterPos = 0; LetterPos < MyCurrentHiddenWord.size(); LetterPos++)
	{
		// if the current letter of the hidden word exists in the guess word, check further
		if (LetterToPosition.count(MyCurrentHiddenWord[LetterPos]))
		{
			// if their positions match also, then it's a bull
			if (LetterToPosition[MyCurrentHiddenWord[LetterPos]] == LetterPos) { BullCowCount.Bulls++; }
			// if their positions don't match, then it's a cow
			else { BullCowCount.Cows++; }
		}
	}

	MyCurrentTurn++;

	// condition that the guess is correct
	if (BullCowCount.Bulls == MyCurrentHiddenWord.size())
	{
		GameStatus = EGameStatus::Won;
	}
	// the game is lost if more than the maximum tries is about to be performed
	else if (GetCurrentTurn() > GetMaxTurn())
	{
		GameStatus = EGameStatus::Lost;
	}

	return BullCowCount;
}

// resets the game instance with the new word length given by the user
void FBullCowGame::ResetWithNewWord(int32 HiddenWordLength)
{
	// get corresponding hidden word from the word length using the map container 
	MyCurrentHiddenWord = WordLengthToWord.at(HiddenWordLength);
	MyCurrentTurn = 1;
	GameStatus = EGameStatus::In_Progress;
}

// checks if the given word is an isogram
bool FBullCowGame::IsIsogram(const FString &GuessWord) const
{
	// container to store already visited letters
	TSet<char> LettersFound;

	// loop through all the letters in the word
	for (const auto& Letter : GuessWord)
	{
		LettersFound.insert(Letter);
	}

	// if all the letters are unique it's an isogram
	return GuessWord.size() == LettersFound.size(); 
}

// checks if all the letters in the given word are english lowercase letters
bool FBullCowGame::IsLowercase(const FString &GuessWord) const
{
	bool bLowercase = true;

	// loop through all the letters in the word
	for (const auto& Letter : GuessWord)
	{
		// condition to check if a letter is not lowercase
		if (Letter < 'a' || Letter > 'z') 
		{
			bLowercase = false;
		}
	}

	return bLowercase;
}

// checks if the given word contains only english alphabets
bool FBullCowGame::IsAlphabetic(const FString &GuessWord) const
{
	bool bAlphabetic = true;

	// loop through all the letters in the word
	for (const auto& Letter : GuessWord)
	{
		// condition to check if a letter is not an alphabet
		if ((Letter < 'a' && Letter > 'z') || (Letter < 'A' && Letter > 'Z')) 
		{
			bAlphabetic = false;
		}
	}

	return bAlphabetic;
}
