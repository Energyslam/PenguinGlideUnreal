// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallingObjectNotifier.h"
#include "FallingObject.h"
#include "FallingObjectSpawner.generated.h"

UCLASS()
class PENGUINGLIDE_API AFallingObjectSpawner : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Blueprints)
		TSubclassOf<class AFallingObjectNotifier> fallingObjectNotifierBP;

	UPROPERTY(EditAnywhere, Category = Blueprints)
		TSubclassOf<class AFallingObject> fallingObjectBP;

	UPROPERTY(EditAnywhere)
		float zOffset = 300;

	// Sets default values for this actor's properties
	AFallingObjectSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AFallingObjectNotifier* fallingObjectNotifier;
	AFallingObject* fallingObject;
	void Spawn(FVector Location, FVector Normals);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
