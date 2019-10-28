// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "UserInterfacemanager.h"

// Sets default values
AInteractable::AInteractable()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent"); //!< creates a staticmeshcomponent and assigns it to the root later
	/** tells the mesh to adjust some of the default settings. A actor with collision needs to simulate physics but these physics have been
	 *  set to non-functional in the blueprint as we don't actually want these to move.
	 */
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic"); //!< the name of one of the default settings for UE physics
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap); //!< the type of overlap response

	RootComponent = StaticMesh;

	/** \brief uses the constructorhelpers to find the particles system needed in the content browser
	 *
	 */
	ConstructorHelpers::FObjectFinder<UParticleSystem> ArbitraryParticleName(TEXT("ParticleSystem'/Game/Particles/Part_Powerup.Part_Powerup'"));
	MyParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ArbitraryParticleName"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> ConstantParticleSystem(TEXT("ParticleSystem'/Game/Particles/Part_TrailedParticles.Part_TrailedParticles'"));
	MyConstantParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ConstantParticleSystem"));
	if (ConstantParticleSystem.Succeeded()) { //!< only if it has succesfully found the particle system, will it try to use the object attached to it
		MyConstantParticleSystem->Template = ConstantParticleSystem.Object;
	}
	MyParticleSystem->bAutoActivate = true;

	if (ArbitraryParticleName.Succeeded()) {
		MyParticleSystem->Template = ArbitraryParticleName.Object;
	}
	MyParticleSystem->bAutoActivate = false;
	MyParticleSystem->SetHiddenInGame(false);
}

void AInteractable::ResolveCollision()
{
}

void AInteractable::BeginPlay() {
	Super::BeginPlay();
	MyParticleSystem->AttachTo(Cast<USceneComponent>(FindComponentByClass(UStaticMeshComponent::StaticClass())));
	MyConstantParticleSystem->AttachTo(Cast<USceneComponent>(FindComponentByClass(UStaticMeshComponent::StaticClass())));
}
