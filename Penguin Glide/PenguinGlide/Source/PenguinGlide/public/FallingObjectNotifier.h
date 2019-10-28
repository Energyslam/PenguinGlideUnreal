// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Public/TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "FallingObjectNotifier.generated.h"

UCLASS()
class PENGUINGLIDE_API AFallingObjectNotifier : public AActor
{
	GENERATED_BODY()
	
public:	
	AFallingObjectNotifier();

	UPROPERTY(EditAnywhere)
		UMaterialInterface* material;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* cyl; //!< is set as our root component

private:
	FTimerHandle timehandle; //!< \brief Unreal Engines timerhandle to handle the timer
	void Blink();
	int blinkCounter = 2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
