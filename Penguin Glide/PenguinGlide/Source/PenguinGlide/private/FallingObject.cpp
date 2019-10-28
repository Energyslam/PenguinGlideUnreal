// Fill out your copyright notice in the Description page of Project Settings.

#include "FallingObject.h"
#include "Engine/Classes/Engine/World.h"
#include "Core.h"
#include "EngineUtils.h"

// Sets default values
AFallingObject::AFallingObject()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFallingObject::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> Components;
	this->GetComponents<UStaticMeshComponent>(Components);

	RootComponent = StaticMeshComponent;


	for (int32 i = 0; i < Components.Num(); i++)
	{
		StaticMeshComponent = Components[i];
	}

	if (StaticMeshComponent != nullptr) {

		StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AFallingObject::OnHit);

		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		StaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		StaticMeshComponent->SetGenerateOverlapEvents(true);
		StaticMeshComponent->SetNotifyRigidBodyCollision(true);

		SetActorEnableCollision(true);


		FVector kakkie = StaticMeshComponent->GetComponentScale();

		UE_LOG(LogTemp, Error, TEXT("poep %s"), *kakkie.ToString());

	}

}

// Called every frame
void AFallingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector currentLoc = StaticMeshComponent->GetComponentLocation();
	float dist = FMath::Sqrt(FMath::Pow((playerLoc.X - currentLoc.X), 2) + FMath::Pow((playerLoc.Y - currentLoc.Y), 2) + FMath::Pow((playerLoc.Z - currentLoc.Z), 2));

	fallingSpeed = dist;

	if (fallingSpeed > 2000) {
		fallingSpeed = 0;
	}
	else if (fallingSpeed < 7000 && fallingSpeed > 4000) {
		fallingSpeed = 1;
	}
	else if (fallingSpeed < 4000 && fallingSpeed > 2000) {
		fallingSpeed = 3;
	}
	else if (fallingSpeed < 2000 && fallingSpeed > 750) {
		fallingSpeed = 5;
	}
	else if (fallingSpeed < 750 && fallingSpeed > 0) {
		fallingSpeed = 7;
	}
	else {
		fallingSpeed = 0;
	}


	FVector newLocation = StaticMeshComponent->GetComponentLocation() - (movementDir * fallingSpeed);
	StaticMeshComponent->SetWorldLocation(newLocation);
	UE_LOG(LogTemp, Error, TEXT("nwloc: %s"), *newLocation.ToString());

}

void AFallingObject::SetMovementDirection(FVector Normal)
{
	movementDir = Normal;
	UE_LOG(LogTemp, Error, TEXT("mvmntdr: %s"), *movementDir.ToString());
}

void AFallingObject::OnHit(class UPrimitiveComponent *ThisComp, AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->GetName() == "Player") {
		FOutputDeviceNull ar;
		this->CallFunctionByNameWithArguments(TEXT("PlayIcecrack"), ar, NULL, true);
		UE_LOG(LogTemp, Error, TEXT("poepie"));
		OnDeath();
	}

	UE_LOG(LogTemp, Error, TEXT("joepie"));

}

void AFallingObject::OnDeath()
{
	fallingSpeed = 0;
	for (TActorIterator<ATempPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->OnDeath();
	}
}
