// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnableObject.h"
#include "Engine/World.h"
#include <vector>
#include "TEMPpowerUp.h"
#include "PowerUp.h"
#include <iostream>
#include "Engine/Classes/Components/ShapeComponent.h"

bool canSpawn = false;
FVector Location;
FVector offset;

// Sets default values
ASpawnableObject::ASpawnableObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpawnableObject::SpawnObject()
{
	AActor* powerUp;
	UWorld* World = GetWorld();

	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	int randomNr = FMath::RandRange(0, powerUpsToSpawn.Num() - 1);

	powerUp = World->SpawnActor<AActor>(
		powerUpsToSpawn[randomNr],
		Location,
		Rotation,
		SpawnInfo
		);
}

FVector ASpawnableObject::GetVertices(TArray<FVector> vertices, FVector trackScale, TArray<FVector> normals)
{
	//TArray<FVector> trackVertices;
	//trackVertices.Append(vertex.data(), vertex.size());
	//UE_LOG(LogTemp, Error, TEXT("Vertex index0: %s"), *trackVertices[0].ToString());
	int vertexIndex = FMath::RandRange(0, vertices.Num() - 1);
	vertexIndex -= vertexIndex % 2; //To Always get an even number
	//UE_LOG(LogTemp, Error, TEXT("Random vertex: %s"), *trackVertices[vertexIndex].ToString());
	//FVector width = vertices[vertexIndex + 1] - trackVertices[vertexIndex];
	Location = vertices[vertexIndex] * trackScale;// +(normals[vertexIndex] * 10);
	//Location.Z -= 350;

	offset = normals[vertexIndex] * 70;
	Location += offset;
	//UE_LOG(LogTemp, Error, TEXT("location: %s"), *vertices[vertexIndex].ToString());

	canSpawn = true;

	return Location;
}

// Called every frame
void ASpawnableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canSpawn) {
		SpawnObject();
		canSpawn = false;
	}

}

