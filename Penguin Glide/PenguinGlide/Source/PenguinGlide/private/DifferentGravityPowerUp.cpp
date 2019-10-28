// Fill out your copyright notice in the Description page of Project Settings.

#include "DifferentGravityPowerUp.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsSettings.h"
#include "Core.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "TempPlayer.h"

// Sets default values
ADifferentGravityPowerUp::ADifferentGravityPowerUp()
{
	warning = "YAY, power up :)";
	warningColor = FColor::Green;
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ADifferentGravityPowerUp::OnCompOverlap); //!< tells the engine that this object has an overlap event
}

void ADifferentGravityPowerUp::OnCompOverlap(class UPrimitiveComponent *ThisComp, AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->GetName() == "Player") {
		StaticMesh->SetVisibility(false);
		MyConstantParticleSystem->DeactivateSystem();
		MyParticleSystem->ActivateSystem();

		float gravityMultiplier = 0.4f;

		UPhysicsSettings* settings = GetMutableDefault<UPhysicsSettings>();

		settings->DefaultGravityZ = -980;
		for (TActorIterator<ATempPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			ActorItr->constantForwardSpeedPerFrame = ActorItr->normalSpeed;
		}

		settings->DefaultGravityZ *= gravityMultiplier;

		for (TActorIterator<ATempPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			ActorItr->MultiplySpeedBy(gravityMultiplier * 2, 8);
		}

		FOutputDeviceNull ar;
		this->CallFunctionByNameWithArguments(TEXT("MakeDark"), ar, NULL, true);

		ResolveCollision(); //!< Resolve collision is implemented in the class powerup. It is simply a function that starts a timer to destroy itself in 1 second.
	}
}

void ADifferentGravityPowerUp::ResolveCollision()
{
	//userInterfaceManager->SetSpeedTo(userInterfaceManager->GetCurrentSpeed() + 1);

	//SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	FTimerHandle timehandle;
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlaySound"), ar, NULL, true);
	GetWorldTimerManager().SetTimer(timehandle, this, &ADifferentGravityPowerUp::DestroySelf, 10.0f, true);
}

void ADifferentGravityPowerUp::DestroySelf() {
	Destroy();
}