// Fill out your copyright notice in the Description page of Project Settings.

#include "InstantPointsPowerUp.h"
#include "Core.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "UserInterfaceManager.h"

// Sets default values
AInstantPointsPowerUp::AInstantPointsPowerUp()
{
	warning = "YAY, power up :)";
	warningColor = FColor::Green;
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AInstantPointsPowerUp::OnCompOverlap); //!< tells the engine that this object has an overlap event
}

void AInstantPointsPowerUp::OnCompOverlap(class UPrimitiveComponent *ThisComp, AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->GetName() == "Player") {
		StaticMesh->SetVisibility(false);
		MyConstantParticleSystem->DeactivateSystem();
		MyParticleSystem->ActivateSystem();
		for (TActorIterator<AUserInterfaceManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			float randomNr = FMath::RandRange(500, 1500);
			ActorItr->AddPointsToScore(randomNr);
			ActorItr->SetLastInstantScore(randomNr);
		}
		/*
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoundManager::StaticClass(), FoundActors);
		for (AActor* TActor : FoundActors)
		{
			soundManager = Cast<ASoundManager>(TActor);

			if (soundManager != nullptr) {
				return;
			}
		}
		soundManager->PlayInstantPoints();*/
		ResolveCollision(); //!< Resolve collision is implemented in the class powerup. It is simply a function that starts a timer to destroy itself in 1 second.
	}
}

void AInstantPointsPowerUp::ResolveCollision()
{
	//userInterfaceManager->SetSpeedTo(userInterfaceManager->GetCurrentSpeed() + 1);

	//SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	FTimerHandle timehandle;
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlaySound"), ar, NULL, true);
	GetWorldTimerManager().SetTimer(timehandle, this, &AInstantPointsPowerUp::DestroySelf, 2.0f, true);
}

void AInstantPointsPowerUp::DestroySelf() {
	Destroy();
}

