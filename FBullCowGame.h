/*
Contains declaration of FBullCowGame, FBullCowCount and helper status enumerations EGuessStatus and EGameStatus.
For implementation of the game logic and explainations, see FBullCowGame.cpp
*/

#pragma once

#include <string>
#include <unordered_map>

// unreal friendly typenames
using int32 = int;
using FString = std::string;
template <typename KeyType, typename ValueType>
using TMap = std::unordered_map<KeyType, ValueType>;

enum class EGuessStatus
{
	Uninitiated,
	OK,
	Not_Isogram,
	Unequal_Length,
	Not_Lowercase,
	Not_alphabetic
};

enum class EGameStatus
{
	In_Progress,
	Won,
	Lost
};

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

class FBullCowGame
{
public:
	FBullCowGame(int32); //constructor

	// getter methods
	int GetCurrentTurn() const;
	int GetMaxTurn() const;
	int GetHiddenWordLength() const;
	EGameStatus GetGameStatus() const;

	// game play methods
	EGuessStatus GetGuessStatus(const FString&) const;
	FBullCowCount SubmitValidGuess(const FString&);

	// reset game method
	void ResetWithNewWord(int32 HiddenWordLength);
	
private:
	// containers to store words corresponding to their lengths
	const std::unordered_map<int32, FString> WordLengthToWord;
	const std::unordered_map<int32, int32> WordLengthToMaxTurn;

	// current round information and status
	FString MyCurrentHiddenWord;
	int32 MyCurrentTurn;
	EGameStatus GameStatus;

	// helper methods
	bool IsIsogram(const FString&) const;
	bool IsLowercase(const FString&) const;
	bool IsAlphabetic(const FString&) const;
};

