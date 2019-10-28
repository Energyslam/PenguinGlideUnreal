#include "TempPlayer.h"
#include "UserInterfaceManager.h"
#include "TimerManager.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/Visual.h"
#include "Kismet/GameplayStatics.h"
#include "BloodscreenClass.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Core.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsSettings.h"
#include "Runtime/Engine/Public/EngineUtils.h"

bool hasWahhPlayed;
ATempPlayer::ATempPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	/** Uses an empty component that the camera can attach to.
	 This component will have an absolute world rotation in the editor so it doesn't inherit the sphere rotationns */
	emptyComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EmptySceneComponent"));
	sphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	hasWahhPlayed = false;
	RootComponent = sphereVisual; //!< Sets the sphereVisual as the rootcomponent of the player
	emptyComponent->SetupAttachment(sphereVisual);
	sphereVisual->SetSimulatePhysics(true);

	// Use a spring arm to give the camera a distance and angle. Can also be used to follow the player around with some lag in between.
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	springArm->SetupAttachment(emptyComponent);
	springArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	springArm->TargetArmLength = 400.0f;

	// Create a camera and attach to our spring arm
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

/**  Stops player movement after hitting an obstacle
 *
 */
void ATempPlayer::StopMoving(bool stop) 
{
	if (stop == true)
	{
		startPosition = GetActorLocation();
		playerActive = false;
		stop = false;
	}
}

/**  When the player dies, he stops moving, the camera shakes and the game fades out. After 1.5 seconds, Die() is called and the game is moved to the game over screen
 *
 */
void ATempPlayer::OnDeath()
{
	if (!snowballCollision)
	{
		blood->createWidget();
		snowballCollision = true;
		CameraShake();
		StopMoving(true);
		GetWorld()->GetTimerManager().SetTimer(deathDelayHandler, this, &ATempPlayer::Die, 1.5f, false);
	}
}

/**  Stops the current game and moves the player to the game over screen
 *
 */
void ATempPlayer::Die()
{
	GetWorld()->GetTimerManager().ClearTimer(deathDelayHandler);
	for (TActorIterator<AUserInterfaceManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->GameOver();
	}
}

/**  Shakes the camera when there is a collision with a snowball
 *
 *
 */
void ATempPlayer::CameraShake()
{
	if (shake != NULL)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(shake, 5.0f);
	}
}

/**  This functions changes the field of field for a set amount of time, then calls the function ResetFieldOfView
 *
 *
 *
 */
void ATempPlayer::SetFieldOfViewTo(int newValue, int seconds)
{
	GetWorld()->GetTimerManager().ClearTimer(fieldOfViewHandler);
	newFOV = newValue;
	increasingFOV = true;
	GetWorld()->GetTimerManager().SetTimer(fieldOfViewHandler, this, &ATempPlayer::ResetFieldOfView, seconds, false);
}

void ATempPlayer::MultiplySpeedBy(float newValue, int seconds)
{
	normalSpeed = constantForwardSpeedPerFrame;
	constantForwardSpeedPerFrame *= newValue;
	GetWorld()->GetTimerManager().SetTimer(slowDownHandler, this, &ATempPlayer::ResetSpeed, seconds, false);
}

/**  Resets the field of view to the original values
 *
 *
 */
void ATempPlayer::ResetFieldOfView()
{
	resettingFOV = true;
	GetWorld()->GetTimerManager().ClearTimer(fieldOfViewHandler);
	if (currentFOV < 100)
	{
		camera->SetFieldOfView(100);
	}
}

void ATempPlayer::ResetSpeed()
{
	constantForwardSpeedPerFrame = normalSpeed;
	GetWorld()->GetTimerManager().ClearTimer(slowDownHandler);
	UPhysicsSettings* settings = GetMutableDefault<UPhysicsSettings>();;
	settings->DefaultGravityZ = -980;
}

/**  This function is used to set the initial position of the player to the start of the track
 *
 *
 */
void ATempPlayer::SetSpawnPosition()
{
	for (TActorIterator<ATrackGeneratorBezier> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		this->SetActorLocation(ActorItr->spawnPosition);
	}
}

// Called when the game starts or when spawned
void ATempPlayer::BeginPlay()
{
	Super::BeginPlay();
	originalSpeedPerFrame = speedPerFrame;
	startPosition = GetActorLocation();
	HasToActivatePlayer(false);
	oldPositionLength = GetActorLocation(); //!< Get the current location of the player
	normalSpeed = constantForwardSpeedPerFrame;

	UPhysicsSettings* settings = GetMutableDefault<UPhysicsSettings>();;
	settings->DefaultGravityZ = -980;
}

// Called every frame
void ATempPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!playerActive) //!< Waits until the player is set to active before he moves
	{
		speedPerFrame = 0;
		SetActorLocation(startPosition); //!< sets the initial position of the player
	}
	else
	{
		speedPerFrame = originalSpeedPerFrame; //!< once active, the player receives a value per frame to move
	}

	currentFOV = camera->FieldOfView;
	frames++;

	FRotator newRotation = camera->GetComponentRotation();
	newRotation.Yaw += CameraInput.X;
	newRotation.Pitch = FMath::Clamp(newRotation.Pitch + CameraInput.Y, -15.f, -15.f); //!< Camera is set to a -15.0 but can be adjust to have vertical rotation
	springArm->SetWorldRotation(newRotation);

	FVector camVect = camera->GetForwardVector(); //!< Gets the forward vector of the camera to determine direction
	sphereVisual->AddForce(camVect * constantForwardSpeedPerFrame); //!< This adds the movement of the player. It adds force each frame in the direction of where the camera is looking

	FVector currentLocation = sphereVisual->GetComponentLocation(); //!< Get the current location of the player

	zPosition = GetActorLocation().Z;

	for (TActorIterator<ATrackGeneratorBezier> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		lowestVerticeZ = ActorItr->GetEndingVerts(); //!< Gets the position of the middle ending vertice of the track
	}

	if (zPosition < lowestVerticeZ) //!< If the player is lower than the lowest vertice of the track, this means that the player fell off and the game is over
	{
		if (hasWahhPlayed == false) {
			soundManager->PlayWahh();
			hasWahhPlayed = true;
		}
		OnDeath();
	}

	if (camera->FieldOfView <= 100) //!< turns off the resetting of the field of view once a value has been reached
	{
		resettingFOV = false;
	}
	
	if (newFOV - camera->FieldOfView < 1 && newFOV - camera->FieldOfView > -1) //!< stops icnreasing the field of view when a value has been reached
	{
		increasingFOV = false;
	}

	if (resettingFOV)
	{
		currentFOV = FMath::Lerp((int)currentFOV, 100, 0.001f);
		camera->SetFieldOfView(currentFOV);
		increasingFOV = false;
	}

	if (increasingFOV)
	{
		currentFOV = FMath::Lerp((int)newFOV, 100, 0.001f);
		camera->SetFieldOfView(currentFOV);
	}

	if (frames % 2 == 0)
	{
		CalculateSpeed();
	}
}

// Called to bind functionality to input
void ATempPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATempPlayer::MoveRight);
	PlayerInputComponent->BindAxis("CameraPitch", this, &ATempPlayer::PitchCamera);
	PlayerInputComponent->BindAxis("CameraYaw", this, &ATempPlayer::YawCamera);
}

/**  Returns the current speed based on the change in position. This is used in the UI to display the speed of the player
 *
 */
float ATempPlayer::GetCurrentSpeed()
{
	return deltaPositionLength;
}

void ATempPlayer::CalculateSpeed()
{
	newPositionLength = GetActorLocation(); //!< return the actor location
	deltaPosition = newPositionLength - oldPositionLength;
	oldPositionLength = GetActorLocation(); //!< Get the current location of the player

	deltaPositionLength = FGenericPlatformMath::Sqrt(deltaPosition.X * deltaPosition.X + deltaPosition.Y * deltaPosition.Y + deltaPosition.Z * deltaPosition.Z);

	deltaPositionLength = round(deltaPositionLength / 2) * 2;

	deltaPositionLength /= 1.2f;
}

/** This region handles the UE4 input for the player
 *
 */
#pragma region Camera and player movement
void ATempPlayer::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}
void ATempPlayer::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}
void ATempPlayer::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}
#pragma endregion

/**  says whether the player is active or not
 *
 */
void ATempPlayer::HasToActivatePlayer(bool playerState)
{
	playerActive = playerState;
	SetActorTickEnabled(playerState);
}

/**  return the state of the player
 *
 */
bool ATempPlayer::GetGameState()
{
	return playerActive;
}

/**  return the trackpivots that can be used for the rotation of the player
 *
 */
void ATempPlayer::AddTrackPivots(FVector pivot)
{
	trackPivots.Add(pivot);
}