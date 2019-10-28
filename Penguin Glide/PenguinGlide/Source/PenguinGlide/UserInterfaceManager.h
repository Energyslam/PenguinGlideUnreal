/*!
 *  \brief     This header manages the user interface
 *  \details   This class serves as a mediator. It communicates between several classes and is a central point for everything user interface related. 
 *			   Classes can access the mediator and ask for information from other classes. The userinterfacemanager class is the one having the references to other classes.
 *			   This releases classes from dependencies with one another, which results no circular dependencies. The user interface itself only has access to this class.
 *			   That is why all functionality related to the UI can be found here. The class mostly constits of methods returning values, because this is required to cast it to text in the UI.
 *			   Extra information regarding specific methods can be found with the methods.
 *  \author    Tycho Zaal
 *  \version   1
 *  \date      12-06-2019
 *  \copyright TZ_productions
 */

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "PenguinSlideGameInstance.h"
#include "LocalHighscoreAccesor.h"
#include "UserInterfaceManager.generated.h"

UCLASS()
class PENGUINGLIDE_API AUserInterfaceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	/** \brief Constructor
	*/
	AUserInterfaceManager();

private:	
		float currentScore = 0;
		float lastScore;
	UPROPERTY(VisibleAnywhere)
	float scoreMultiplier = 1;
	UPROPERTY(VisibleAnywhere)
	float speedScoreMultiplier = 1;
	UPROPERTY(EditAnywhere)
		float averageSpeed = 12;

	UPROPERTY(EditAnywhere)
		int timeToWait = 5;
	UPROPERTY(VisibleAnywhere)
		int secondsLeft = 5;

		ULocalHighscoreAccesor* highScoreAccessor;
	UPROPERTY(EditAnywhere)
		UPenguinSlideGameInstance* penguinSlideGameInstance;
	
		TArray <int> highScoresIntArray;
		TArray <FString> highScoresStringArray;
		TArray<FString> topXHighScores;	

		int lastAddedInstantScore;
		bool showInstantText = false;
	
	UPROPERTY(EditAnywhere)
		FString TextFileString;

	FTimerHandle timerHandler;
	FTimerHandle startSequenceTimer;
	FTimerHandle countdownTimer;
	FTimerHandle instantPointsTimer;

	UPROPERTY(EditAnywhere)
		TArray<FString> quotes;

	UPROPERTY(EditAnywhere)
		FString finalQuote;

protected:
	/** \brief Called at the start of the game or when spawned
	*/
	virtual void BeginPlay() override;

public:	
	/** \brief Called every frame.
	*/
	virtual void Tick(float DeltaTime) override;

	/** \brief Returns the current score
	*/
	UFUNCTION(BlueprintCallable)
		float GetCurrentScore();

	/** \brief Returns the last score
	*/
	UFUNCTION(BlueprintCallable)
		float GetLastScore();

	/** \brief Returns the current score multiplier
	*/
	UFUNCTION(BlueprintCallable)
		float GetScoreMultiplier();

	/** \brief Returns the current speed
	*/
	UFUNCTION(BlueprintCallable)
		float GetCurrentSpeed();

	UFUNCTION(BlueprintCallable)
		int GetLastInstantScore();

	UFUNCTION(BlueprintCallable)
		void SetLastInstantScore(int score);

	UFUNCTION(BlueprintCallable)
		int ShowText();

	/** \brief Change score multiplier
	*/
	void SetScoreMultiplierTo(float newMultiplier, int duration);

	/** \brief Resets score multiplier
	*/
	void ResetScoreMultiplier();

	void ResetLastInstantScore();

	/** \brief Adds X to the current score
	*/
	void AddPointsToScore(float pointsToAdd);

	/** \brief Save the score in the Singleton
	*/
	void SavePoints();

	/** \brief Collects all highscores from text file
	*/
	void GetHighscores();

	/** \brief Applies changes in highscore array to text file
	*/
	void SetHighscores();

	/** \brief Adds a new highscore to the array
	*/
	void AddIntToHighScoresStringArray(int numberToAdd);

	/** \brief Sorts the highscore array, using the newly added data 
	*/
	void ConvertAndSortHighScoreArray();

	/** \brief Returns top X highscores
	*/
	UFUNCTION(BlueprintCallable)
		TArray<FString> GetTopXHighScores(int amountToGet);

	/** \brief Loads the Game Over menu map
	*/
	void GameOver();

	/** \brief Counts down for the start sequence
	*/
	void Countdown();

	/** \brief Starts playing the game
	*/
	void StartPlaying();

	/** \brief Returns the amount of seconds left before the game starts
	*/
	UFUNCTION(BlueprintCallable)
		int GetSecondsLeft();

	UFUNCTION(BlueprintCallable)
		FString GetQuote();
};
