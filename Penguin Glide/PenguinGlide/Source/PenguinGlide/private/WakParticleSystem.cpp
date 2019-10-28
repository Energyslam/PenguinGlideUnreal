// Fill out your copyright notice in the Description page of Project Settings.

#include "WakParticleSystem.h"

// Sets default values
AWakParticleSystem::AWakParticleSystem()
{
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* baseComp = CreateDefaultSubobject<USceneComponent>("UScenecomponent"); //!< Creates a default USceneComponent so we can see our root in the editor
	RootComponent = baseComp;
	ConstructorHelpers::FObjectFinder<UParticleSystem> ArbitraryParticleName(TEXT("ParticleSystem'/Game/Particles/Part_Wak.Part_Wak'")); //!< Finds the system in the content browser
	MyParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ArbitraryParticleName"); //!< Creates a particle system component
	if (ArbitraryParticleName.Succeeded()) { //!< Only use the object of the found system if it is succesful
		MyParticleSystem->Template = ArbitraryParticleName.Object;
	}
	MyParticleSystem->AttachTo(baseComp); //!< Makes it a child of the root
}

// Called when the game starts or when spawned
void AWakParticleSystem::BeginPlay()
{
	Super::BeginPlay();
	MyParticleSystem->ActivateSystem(); //!< starts the system once the actor begins
}

// Called every frame
void AWakParticleSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (despawnTimer > 200) {
		FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		UE_LOG(LogTemp, Error, TEXT("PlayerPos: %f, poweruploc: %f"), playerPos.X, GetActorLocation().X);
		if (playerPos.X > GetActorLocation().X + 2000) {
			this->Destroy();
		}
		despawnTimer = 0;
	}



	despawnTimer++;
}
