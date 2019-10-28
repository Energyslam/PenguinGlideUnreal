// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"

#include "PenguinSlideGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PENGUINGLIDE_API UPenguinSlideGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
		/** \brief Returns the points from the last run
		*/
		int GetLatestRunPoints();
		/** \brief Sets the points from the last run
		*/
		void SetLatestRunPoints(int points);
	
private:
	UPROPERTY(EditAnywhere)
		int latestPoints = 1;
};
