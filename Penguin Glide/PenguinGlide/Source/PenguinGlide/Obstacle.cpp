// Fill out your copyright notice in the Description page of Project Settings.

#include "Obstacle.h"
#include <Engine.h>

// Sets default values
AObstacle::AObstacle()
{
	warning = "YOU HIT AN OBSTACLE";
	warningColor = FColor::Red;
}

void AObstacle::ResolveCollision()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}
