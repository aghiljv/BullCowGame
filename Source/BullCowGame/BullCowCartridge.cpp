// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
	Super::BeginPlay();
	//FBullCowCount Count;
	Isograms = GetValidWords(Words);
	InitGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
	//if game is over do ClearScreen() and InitGame()
	if (bGameOver) {
		ClearScreen();
		InitGame();
	}
	//else check player guess
	else {	//Checking player guess
		ProcessGuess(Input);
	}
}

void UBullCowCartridge::InitGame() {
	//Welcome the player
	PrintLine(TEXT("Welcome to Bulls and Cows!"));

	HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];	//Set the HiddenWord
	Lives = HiddenWord.Len();	//Set lives
	bGameOver = false;

	PrintLine(TEXT("Guess the %i letter word."), HiddenWord.Len());
	PrintLine(TEXT("The HiddenWord is : %s."), *HiddenWord);
	PrintLine(TEXT("You have %i lives left."), Lives);
	PrintLine(TEXT("Type in your guess and press Enter to continue"));	//Prompt player for guess
}

void UBullCowCartridge::EndGame() {
	bGameOver = true;
	PrintLine(TEXT("\nPress Enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
	if (Guess == HiddenWord) {
		PrintLine(TEXT("You have won!!!"));
		EndGame();
		return;
	}

	//Check is isogram
	if (!IsIsogram(Guess)) {
		PrintLine(TEXT("No repeating letters. Guess Again!"));
		return;
	}

	if (Guess.Len() != HiddenWord.Len())
	{
		PrintLine(TEXT("The Hidden Word is %i letters long."), HiddenWord.Len());
		PrintLine(TEXT("Sorry, try again. You have %i lives left"), Lives);
		return;
	}

	//Remove life
	PrintLine(TEXT("You have lost one life."));
	--Lives;

	if (Lives <= 0) {
		ClearScreen();
		PrintLine(TEXT("You have no lives left!\nYou have lost!"));
		PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
		EndGame();
		return;
	}
	//Show Player the bulls and cows
	FBullCowCount Count = GetBullCows(Guess);
	PrintLine(TEXT("You have %i Bulls & %i Cows!"), Count.Bulls, Count.Cows);
	PrintLine(TEXT("Sorry, try again. You have %i lives left"), Lives);
	//Check if lives are > 0
	//If yes continue game
	//Show lives left
	//If not, Show gameover and show HiddenWord
	//Prompt to play again, Press enter to play again
	//PlayAgain or quit
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const {
	for (int32 Index = 0; Index < Word.Len(); Index++) {
		for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++) {
			if (Word[Index] == Word[Comparison]) {
				return false;
			}
		}
	}
	return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordsList) const {
	TArray<FString> ValidWords;
	for (FString Word : WordsList) {
		if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word)) {
			ValidWords.Emplace(Word);
		}
	}
	return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const {
	FBullCowCount Count;

	for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
	{
		if (Guess[GuessIndex] == HiddenWord[GuessIndex]) {
			Count.Bulls++;
			continue;
		}
		for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
		{
			if (Guess[GuessIndex] == HiddenWord[HiddenIndex]) {
				Count.Cows++;
				break;
			}
		}
	}
	return Count;
}