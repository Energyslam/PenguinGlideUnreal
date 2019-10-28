#pragma once

#include "CoreMinimal.h"
#include <string>
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParticleDefinitions.h"
#include "ConstructorHelpers.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

using namespace std;
UCLASS()
/*!
 *  \brief     This class is used for the interactables in the game, namely the power-ups.
 *  \details   The interactables in the game right now is the power-up. The power-up has two seperate particle system, one to indicate
 *					to the player that there is something special about it, and one that plays once the player has picked it up to
 *					give some juice to the game. This makes it feel more impactful than simply picking it up. This class is derived from
 *					a blueprint. This blueprint holds the static mesh that the interactable has, which is a standard sphere with a material.
 *					It creates and attaches the particle system during runtime.
 *
 *  \author    Max Groen
 *  \version   1
 *  \date      15/05/2019
 *  \copyright Energyslam™
 */
class PENGUINGLIDE_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();
	class UParticleSystemComponent* MyParticleSystem; //!< This particle system is always playing until it is picked up
	class UParticleSystemComponent* MyConstantParticleSystem; //!< this particle system will only play once it has been picked up
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UParticleSystem* particles;
	FString warning;
	FColor warningColor;
	
	virtual void ResolveCollision();
};