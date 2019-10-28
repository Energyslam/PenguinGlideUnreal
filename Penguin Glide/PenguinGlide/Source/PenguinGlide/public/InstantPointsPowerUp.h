// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterfaceManager.h"
#include "GameFramework/Actor.h"
#include "SoundManager.h"
#include "Interactable.h"
#include "InstantPointsPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class PENGUINGLIDE_API AInstantPointsPowerUp : public AInteractable
{
	GENERATED_BODY()
	AInstantPointsPowerUp();

	AUserInterfaceManager* userInterfaceManager;
	UPROPERTY(EditAnywhere)
		class ASoundManager* soundManager; //!< pointer to the soundmanager so it can play sound on collision
	void DestroySelf();

protected:
	// Called when the game starts or when spawned
	void ResolveCollision() override;

	UFUNCTION()
		void OnCompOverlap(class UPrimitiveComponent *ThisComp, AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
