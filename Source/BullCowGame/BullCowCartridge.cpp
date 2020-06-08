// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    PrintLine(TEXT("HiddenWord is %s\n"), *HiddenWord); // FOR DEBUGGING 
    PrintLine(TEXT("The number of possibble words are: %i"), Words.Num());
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (!bGameOver) // Checking the player guess
    {
        ProcessGuess(Input);
    }
    else
    {
        ClearScreen();
        SetupGame();
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = TEXT("cakes");
    Lives = HiddenWord.Len();
    bGameOver = false;

    // Welcoming the player
    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Guess the %i letter word!\n"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type in your guess and \npress Enter to continue.\n"));

    const TCHAR HW[] = TEXT("cakes");
}

void UBullCowCartridge::EndGame() 
{
    bGameOver = true;
    
    PrintLine(TEXT("\nPlease press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(FString Guess)
{
    if (Guess == HiddenWord) // If the guess is correct
    {
        PrintLine(TEXT("That is correct!"));
        EndGame();
        return;
    }

    if (HiddenWord.Len() != Guess.Len()) // The number of characters don't match
    {
        PrintLine(TEXT("Not enough characters.\nThe hidden word is %i characters long.\nYou have %i lives remaining"), HiddenWord.Len(), Lives);
        return;
    }

    if (!IsIsogram(Guess)) // Check that it is an isogram
    {
        PrintLine(TEXT("Too many repeating letters, guess again."));
        return;
    }

    // Losing a life
    PrintLine(TEXT("Lost a life."));
    --Lives;

    // Once lives run out
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
        EndGame();
        return;
    }

    // Show the player Bulls and Cows
    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(FString Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}