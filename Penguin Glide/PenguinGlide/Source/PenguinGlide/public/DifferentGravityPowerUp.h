// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterfaceManager.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "DifferentGravityPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class PENGUINGLIDE_API ADifferentGravityPowerUp : public AInteractable
{
	GENERATED_BODY()
	ADifferentGravityPowerUp();

	void DestroySelf();

protected:
	// Called when the game starts or when spawned
	void ResolveCollision() override;

	UFUNCTION()
		void OnCompOverlap(class UPrimitiveComponent *ThisComp, AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};