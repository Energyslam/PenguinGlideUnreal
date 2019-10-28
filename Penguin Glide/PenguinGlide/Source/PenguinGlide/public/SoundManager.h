#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundManager.generated.h"

UCLASS()
/*!
 *  \brief     This class is used to store sounds and has methods that can be called to play them.
 *  \details   The class has an attached blueprint based on this class. The blueprint has the information about the sounds
 *					and on what channel to play them. The methods in this class can be called to play a function in the blueprint.
 *					It also contains a pointer to the level sequence attached to the instanced SoundManager to play once a player
 *					moves to the game over screen. This is purely because we only need to call this sequence once per playthrough
 *					and this class is already instantiated. It has timer to prevent from playing continuously in a row.
 *
 *  \author    Max Groen
 *  \version   1
 *  \date      02/06/2019
 *  \copyright Energyslam™
 */
class PENGUINGLIDE_API ASoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoundManager();
	void PlayPickupSound();
	void StartFadeOut();
	void PlaySpeedUpSound();
	void PlayIcecrack();
	void PlayGravityDown();
	void PlayGravityUp();
	void PlayInstantPoints();
	void PlayMultiplier();
	void PlayShake();
	void PlayWahh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
