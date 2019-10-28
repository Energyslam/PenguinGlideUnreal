// Fill out your copyright notice in the Description page of Project Settings.

#include "FallingObjectNotifier.h"


// Sets default values
AFallingObjectNotifier::AFallingObjectNotifier()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cyl = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cyl"));

	//RootComponent = cyl;
}

void AFallingObjectNotifier::Blink()
{
/*	if (blinkCounter % 2 == 0) {
		cyl->SetVisibility(false, true);
	}
	else {
		cyl->SetVisibility(true, true);
	}

	blinkCounter++;*/
}

// Called when the game starts or when spawned
void AFallingObjectNotifier::BeginPlay()
{
	Super::BeginPlay();



	FVector newLocation = GetActorLocation();
	newLocation.Z += 1.0f;
	SetActorLocation(newLocation);

	GetWorldTimerManager().SetTimer(timehandle, this, &AFallingObjectNotifier::Blink, 0.1f, true);

}

// Called every frame
void AFallingObjectNotifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


