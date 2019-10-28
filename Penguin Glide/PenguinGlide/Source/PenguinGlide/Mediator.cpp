// Fill out your copyright notice in the Description page of Project Settings.

#include "Mediator.h"

// Sets default values
AMediator::AMediator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMediator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMediator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

