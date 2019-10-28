// Fill out your copyright notice in the Description page of Project Settings.

#include "PenguinSlideGameInstance.h"

int UPenguinSlideGameInstance::GetLatestRunPoints()
{
	return latestPoints;
}

void UPenguinSlideGameInstance::SetLatestRunPoints(int points)
{
	latestPoints = points;
}
