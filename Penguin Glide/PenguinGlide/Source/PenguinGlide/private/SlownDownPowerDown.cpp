// Fill out your copyright notice in the Description page of Project Settings.

#include "SlownDownPowerDown.h"
#include "Core.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "UserInterfaceManager.h"

// Sets default values
ASlownDownPowerDown::ASlownDownPowerDown()
{
	warning = "YAY, power up :)";
	warningColor = FColor::Green;
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ASlownDownPowerDown::OnCompOverlap); //!< tells the engine that this object has an overlap event
}

void ASlownDownPowerDown::OnCompOverlap(class UPrimitiveComponent *ThisComp, AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->GetName() == "Player") {
		StaticMesh->SetVisibility(false);
		MyConstantParticleSystem->DeactivateSystem();
		MyParticleSystem->ActivateSystem();
		for (TActorIterator<ATempPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			float randomNr = FMath::RandRange(0.6f, 1.0f);
			ActorItr->SetFieldOfViewTo(80, 2.0f);
			ActorItr->MultiplySpeedBy(randomNr, 2.0f);
		}
		ResolveCollision(); //!< Resolve collision is implemented in the class powerup. It is simply a function that starts a timer to destroy itself in 1 second.
	}
}

void ASlownDownPowerDown::ResolveCollision()
{
	//userInterfaceManager->SetSpeedTo(userInterfaceManager->GetCurrentSpeed() + 1);

	//SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	FTimerHandle timehandle;
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlaySound"), ar, NULL, true);
	GetWorldTimerManager().SetTimer(timehandle, this, &ASlownDownPowerDown::DestroySelf, 2.0f, true);
}

void ASlownDownPowerDown::DestroySelf() {
	Destroy();
}

