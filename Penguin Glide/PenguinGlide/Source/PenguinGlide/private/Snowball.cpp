// Fill out your copyright notice in the Description page of Project Settings.

#include "Snowball.h"
#include "Components/SphereComponent.h"
#include "ConstructorHelpers.h"
#include "TempPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/MovementComponent.h"

int playSound;
ASnowball::ASnowball()
{
	amazingTimer = 0;
	lerpValue = 0.0f;
	speedValue = FMath::RandRange(0.008f, 0.015f);
	countingDown = false;
	playSound = 0;
 	
	PrimaryActorTick.bCanEverTick = true;
	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = sphereComponent;
	sphereComponent->InitSphereRadius(30.0f);
	sphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	//!< Creates a basic static mesh to which we assign a sphere
	sphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	sphereVisual->SetupAttachment(RootComponent);

	/** \brief Creates the mesh by using the UE4 sphere and assign it several collision parameters to be used
	 *
	 *
	 */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereVisualAsset.Succeeded())
	{
		sphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		sphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		sphereVisual->SetWorldScale3D(FVector(1.5f));
		sphereVisual->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		sphereVisual->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		sphereVisual->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		sphereVisual->SetGenerateOverlapEvents(true);
		sphereVisual->SetNotifyRigidBodyCollision(true);

		SetActorEnableCollision(true);
	}

	//!< here we assign a material by finding it in the content browser.
	static ConstructorHelpers::FObjectFinder<UMaterial> mat(TEXT("Material'/Game/Materials/PowerUpMat.PowerUpMat'"));
	if (mat.Succeeded()) {
		StoredMaterial = mat.Object;
	}
	sphereVisual->SetMaterial(0, StoredMaterial);
}

// Called when the game starts or when spawned
void ASnowball::BeginPlay()
{
	Super::BeginPlay();
	sphereVisual->OnComponentBeginOverlap.AddDynamic(this, &ASnowball::OnCompOverlap); //!< tells the engine to check for overlaps with this object
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoundManager::StaticClass(), FoundActors);
	for (AActor* TActor : FoundActors)
	{
		soundManager = Cast<ASoundManager>(TActor);

		if (soundManager != nullptr) {
			return;
		}
	}
}

// Called every frame
void ASnowball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	playSound++;
	amazingTimer += 1;
	if (amazingTimer >= 100) 
	{ //!< has a semi-timer to prevent the object from calling the player position each frame
		for (TActorIterator<ATempPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			FVector playerloc = ActorItr->GetActorLocation();
			if (playerloc.X > GetActorLocation().X + 2000) {
				Destroy();
			}
		}
		amazingTimer = 0;
	}
	if (lerpPositions.Num() == 0) 
	{
		return;
	}
	if (!countingDown) 
	{ //!< if it is not counter down, add the movement/speed value.
		lerpValue += speedValue;
	}
	else 
	{
		lerpValue -= speedValue;
	}
	if (lerpValue >= 1.0f)
	{
		countingDown = true;
	}

	if (lerpValue <= 0.0f) 
	{
		countingDown = false;
	}

	/** \brief This is the bezier curve function. It is the same way we create the track and its curve. It does a linear interpolation
	 *			  between the linear interpolation of the linear interpolation between the nodes, and then sets the snowball location to
	 *			  the distance between this interpolation * the lerpValue. This way we can simulate a moving snowball.
	 *
	 */
	this->SetActorLocation(FMath::Lerp(
									FMath::Lerp(
										FMath::Lerp(lerpPositions[0], lerpPositions[1], lerpValue),
										FMath::Lerp(lerpPositions[1], lerpPositions[2], lerpValue),
										lerpValue),
									FMath::Lerp(
										FMath::Lerp(lerpPositions[1], lerpPositions[2], lerpValue),
										FMath::Lerp(lerpPositions[2], lerpPositions[3], lerpValue),
										lerpValue),
									lerpValue));
}

/** Sets the positions of the nodes that the snowball will create a bezier curve from and lerp between.
 *
 */
void ASnowball::SetPositions(TArray<FVector> positions, float zOffset) {
	lerpPositions = positions;
	lerpPositions[1].Z -= zOffset;
	lerpPositions[2].Z -= zOffset;
}

/** \brief handles the collision events of the snowball by using the unreal function OnCompOverlap that is a part of AActor.
 *
 */
void ASnowball::OnCompOverlap(class UPrimitiveComponent *ThisComp, AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->GetName() == "Player" && playSound > 100) {
		playSound = 0;
		soundManager->StartFadeOut();
		soundManager->PlayPickupSound();
		soundManager->PlayShake();
		OnDeath();
	}
}

/** \brief calls the player ondeath function
 *
 */
void ASnowball::OnDeath()
{
	speedValue = 0;
	for (TActorIterator<ATempPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->OnDeath();
	}
}
