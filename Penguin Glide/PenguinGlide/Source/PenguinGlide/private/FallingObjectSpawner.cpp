// Fill out your copyright notice in the Description page of Project Settings.

#include "FallingObjectSpawner.h"
#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"

// Sets default values
AFallingObjectSpawner::AFallingObjectSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFallingObjectSpawner::BeginPlay()
{
	Super::BeginPlay();
	//Spawn();
}

void AFallingObjectSpawner::Spawn(FVector Location, FVector Normals)
{
	UWorld* World = GetWorld();
	//FVector Location(0, 0, 0);
	//FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	Location.Z += 5.f;

	FVector rotationPoint = Location + (Normals * 100);

	//DrawDebugPoint(GetWorld(), rotationPoint, 20.0f, FColor(0, 0, 1), false, 10.0f, 0.03f);

	FRotator Rotation = FRotationMatrix::MakeFromZ(Location - rotationPoint).Rotator();

	Rotation.Roll += 180.f;

	fallingObjectNotifier = World->SpawnActor<AFallingObjectNotifier>(
		fallingObjectNotifierBP,
		Location,
		Rotation,
		SpawnInfo
		);

	//FVector foLocation = Location + (Normals * zOffset);
	Location += (Normals * zOffset);

	fallingObject = World->SpawnActor<AFallingObject>(
		fallingObjectBP,
		Location,
		Rotation,
		SpawnInfo
		);

	fallingObject->SetMovementDirection(Normals);

	
}

// Called every frame
void AFallingObjectSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


