#include "UserInterfaceManager.h"
#include "TempPlayer.h"

#include "Algo/Reverse.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

// Constructor
AUserInterfaceManager::AUserInterfaceManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Returns the current score
float AUserInterfaceManager::GetCurrentScore()
{
	return currentScore;
}

// Returns the score of the latest run
float AUserInterfaceManager::GetLastScore()
{
	return penguinSlideGameInstance->GetLatestRunPoints();
}

// Returns the score multiplier
float AUserInterfaceManager::GetScoreMultiplier()
{
	return scoreMultiplier;
}

// Returns current speed
float AUserInterfaceManager::GetCurrentSpeed()
{
	for (TActorIterator<ATempPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		return ActorItr->GetCurrentSpeed();
	}

	return 25;
}

int AUserInterfaceManager::GetLastInstantScore()
{
	return lastAddedInstantScore;
}

void AUserInterfaceManager::SetLastInstantScore(int score)
{
	showInstantText = true;
	GetWorld()->GetTimerManager().ClearTimer(instantPointsTimer);
	lastAddedInstantScore = score;
	GetWorld()->GetTimerManager().SetTimer(instantPointsTimer, this, &AUserInterfaceManager::ResetLastInstantScore, 5, false);
}

int AUserInterfaceManager::ShowText()
{
	if (showInstantText)
		return 0;

	else
	{
		return 1;
	}
}

// Changes score multiplier to parameter value
void AUserInterfaceManager::SetScoreMultiplierTo(float newMultiplier, int duration)
{
	scoreMultiplier = newMultiplier;
	GetWorld()->GetTimerManager().SetTimer(timerHandler, this, &AUserInterfaceManager::ResetScoreMultiplier, duration, false);
}

// Resets multiplier to 1
void AUserInterfaceManager::ResetScoreMultiplier()
{
	scoreMultiplier = 1;
	GetWorld()->GetTimerManager().ClearTimer(timerHandler);
}

void AUserInterfaceManager::ResetLastInstantScore()
{
	lastAddedInstantScore = NULL;
	showInstantText = false;
}

// Adds points to the score
void AUserInterfaceManager::AddPointsToScore(float pointsToAdd)
{
	currentScore += pointsToAdd * scoreMultiplier * speedScoreMultiplier;
}

// Sets latest run points to final score of the run
void AUserInterfaceManager::SavePoints()
{
	penguinSlideGameInstance->SetLatestRunPoints(currentScore);
}

// Gets all data from text file, converts them to ints and places then in an int array
void AUserInterfaceManager::GetHighscores()
{
	highScoreAccessor->LoadTxt(TextFileString, highScoresStringArray);
	ConvertAndSortHighScoreArray();
}

// Replaces text in the textfile with the highscores
void AUserInterfaceManager::SetHighscores()
{
	highScoreAccessor->SaveTxt(highScoresStringArray, TextFileString);
}

// Add new int to string array.
void AUserInterfaceManager::AddIntToHighScoresStringArray(int numberToAdd)
{
	highScoresStringArray.Add(FString::FromInt(numberToAdd));
	ConvertAndSortHighScoreArray();
	SetHighscores();
}

// Converts data from text file string array, sorts it by number and converts it back to a string array
void AUserInterfaceManager::ConvertAndSortHighScoreArray()
{
	for (size_t i = 0; i < highScoresStringArray.Num(); i++)
	{
		int32 highScoreFromStringToInt = FCString::Atoi(*highScoresStringArray[i]);
		highScoresIntArray.Add(highScoreFromStringToInt);
	}

	highScoresIntArray.Sort();
	Algo::Reverse(highScoresIntArray);

	highScoresStringArray.Empty();

	for (size_t i = 0; i < highScoresIntArray.Num(); i++)
	{
		highScoresStringArray.Add(FString::FromInt(highScoresIntArray[i]));
	}

	highScoresIntArray.Empty();
}

// Gets the topX amount of high scores and returns them
TArray<FString> AUserInterfaceManager::GetTopXHighScores(int amountToGet)
{
	topXHighScores.Empty();

	for (size_t i = 0; i < amountToGet; i++)
	{
		topXHighScores.Add(highScoresStringArray[i]);
	}

	return topXHighScores;
}

// Player dies
void AUserInterfaceManager::GameOver()
{
	if (currentScore > 0)
	{
		AddIntToHighScoresStringArray(currentScore);
	}
	penguinSlideGameInstance->SetLatestRunPoints(currentScore);
	UGameplayStatics::OpenLevel(this, FName(TEXT("GameOverMenu")));
}

// Counts down from X to zero at the start of the game
void AUserInterfaceManager::Countdown()
{
	GetWorld()->GetTimerManager().ClearTimer(countdownTimer);
	secondsLeft--;
	if (secondsLeft > 1)
	{
		GetWorld()->GetTimerManager().SetTimer(countdownTimer, this, &AUserInterfaceManager::Countdown, 1, false);
	}
}

// Activates player after start sequence
void AUserInterfaceManager::StartPlaying()
{
	GetWorld()->GetTimerManager().ClearTimer(startSequenceTimer);
	GetWorld()->GetTimerManager().ClearTimer(countdownTimer);

	for (TActorIterator<ATempPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->HasToActivatePlayer(true);
	}
	secondsLeft = 0;
}

// Returns seconds left of countdown
int AUserInterfaceManager::GetSecondsLeft()
{
	return secondsLeft;
}

FString AUserInterfaceManager::GetQuote()
{
	return finalQuote;
}

// Called upon start
void AUserInterfaceManager::BeginPlay()
{
	Super::BeginPlay();
	finalQuote = nullptr;
	penguinSlideGameInstance = Cast<UPenguinSlideGameInstance>(GetGameInstance());
	GetHighscores();
	GetWorld()->GetTimerManager().SetTimer(startSequenceTimer, this, &AUserInterfaceManager::StartPlaying, timeToWait, false);
	GetWorld()->GetTimerManager().SetTimer(countdownTimer, this, &AUserInterfaceManager::Countdown, 1, false);
	secondsLeft = timeToWait;
	if (quotes.Num() > 0)
		finalQuote = quotes[FMath::RandRange(0, quotes.Num() - 1)];
}


// Called every frame
void AUserInterfaceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (TActorIterator<ATempPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetGameState() == true)
		{
			AddPointsToScore(1);
		}
	}

	if (speedScoreMultiplier < 0.5)
	{
		speedScoreMultiplier = 0.5;
	}
}

