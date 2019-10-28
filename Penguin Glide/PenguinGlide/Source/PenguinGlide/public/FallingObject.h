// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TempPlayer.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "FallingObject.generated.h"

UCLASS()
class PENGUINGLIDE_API AFallingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallingObject();

	UStaticMeshComponent* StaticMeshComponent = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetMovementDirection(FVector Normal);

private:
	UPROPERTY(EditAnywhere)
		int fallingSpeed = 10;

	UFUNCTION()
		void OnHit(class UPrimitiveComponent *ThisComp, AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	void OnDeath();

	FVector movementDir = FVector(0, 0, 0);

};
