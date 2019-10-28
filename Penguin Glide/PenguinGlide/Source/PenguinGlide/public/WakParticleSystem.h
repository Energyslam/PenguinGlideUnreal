#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "ParticleDefinitions.h"
#include "GameFramework/Actor.h"
#include "WakParticleSystem.generated.h"

UCLASS()
/*!
 *  \brief     This class has the particle system attached to it to indicate where a wak is
 *  \details   This class is used to instantiate an actor in the level on the location of where a wak is
 *					to indicate that there is one. It automatically creates a particle system once it is instantiated
 *					and attaches it to itself. The wak is not an actual actor but a vertice in the track that has been
 *					moved downwards to simulate a pitfall. Due to this, we can't actually attach a particle system to the wak.
 *
 *
 *  \author    Max Groen
 *  \version   1
 *  \date      29/05/2019
 *  \copyright Energyslam™
 */
class PENGUINGLIDE_API AWakParticleSystem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWakParticleSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int despawnTimer = 0;
	class UParticleSystemComponent* MyParticleSystem;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
