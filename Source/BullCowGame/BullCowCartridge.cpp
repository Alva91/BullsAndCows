// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (!bGameOver) // Checking the player guess
    {
        ProcessGuess(PlayerInput);
    }
    else
    {
        ClearScreen();
        SetupGame();
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    // Welcoming the player
    //PrintLine(TEXT("HiddenWord is %s"), *HiddenWord); // FOR DEBUGGING 
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

void UBullCowCartridge::ProcessGuess(const FString& Guess)
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
    FBullCowCount Score = GetBullsCows (Guess);

    PrintLine (TEXT("Your guess has %i Bulls and %i Cows."), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
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

TArray<FString> UBullCowCartridge::GetValidWords (const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullsCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
        
    }
    return Count;
    
}