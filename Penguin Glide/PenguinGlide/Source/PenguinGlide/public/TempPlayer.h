#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrackGeneratorBezier.h"
#include "SoundManager.h"
#include "CollisionCameraShake.h"

#include "TempPlayer.generated.h"

class ATrackGeneratorBezier;
UCLASS()
/*!
 *  \brief     This class is used to create our player. Originally this was not planned to be our main player class hence the name.
 *
 *
 *  \details  Our player manually created in C++. It uses a static mesh that is assigned in the editor for our penguin model.
 *					The player utilizes a scene component with absolute rotation to attach our springarm and camera to. This is done because
 *					the player rotates heavily during camera which causes the camera to spin if it is not set to absolute rotation. A sphere component
 *					is used to draw a snowball over the penguin, as our penguin is stuck in a snowball and rolling down the track.
 *					The player needs a pointer to the class TrackGeneratorBezier to retrieve the position of the last vertice of the track to determine
 *					whether the player has died or not, and to set the initial position of the player. It also needs an include of the CollisionCameraShake
 *					class in order to create a shaky effects once the player has collision with a snowball.
 *					This class handles the movement of the player and the camera, as well as calling the function in the UI to move the player from game screen
 *					to the game over screen.
 *
 *  \author    Max Groen
 *  \version   1
 *  \date      09/06/2019
 *  \copyright Energyslam™
 */
class PENGUINGLIDE_API ATempPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATempPlayer();
	float speedPerFrame = 10.0f;
	float originalSpeedPerFrame;

	UPROPERTY(EditAnywhere)
		float constantForwardSpeedPerFrame = 50.0f; //!< Used to give a constant movement forward

	/** Components have been made a UPROPERTY(EditAnywhere) and public to force them to be visible in the UE editor */
	UPROPERTY(EditAnywhere)
		class ASoundManager* soundManager;
	UPROPERTY(EditAnywhere)
		class ABloodscreenClass* blood; //!< pointer to the soundmanager so it can play sound on collision

	UPROPERTY(EditAnywhere)
		class UCameraComponent* camera; //!< the camera is attached as a child to the springarm so it follows the player at a distance

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* springArm; //!< A springarm is attached to the player so it follows the player at a static range

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* sphereVisual; //!< is set as our root component

	UPROPERTY(EditAnywhere)
		class USphereComponent* sphereComponent; //!< used to display a snowball over the penguin

	UPROPERTY(EditAnywhere)
		class USceneComponent* emptyComponent; //!< used to prevent the rotation of the player affecting the attached children that don't need to rotate

	UPROPERTY(EditAnywhere)
		ATrackGeneratorBezier* trackGeneratorBezier; //!< a pointer to the track so the player knows when it fell off and where it has to start

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCollisionCameraShake> shake; //!< A template class of UCollisionCameraShake to handle the shake on collision

	UPROPERTY(VisibleAnywhere)
		float deltaPositionLength;

	int frames = 0;

	FVector deltaPosition;

	FVector MovementInput;
	FVector2D CameraInput;
	FVector startPosition;

	bool playerActive = false;
	UPROPERTY(EditAnywhere)
		bool resettingFOV = false;
	UPROPERTY(EditAnywhere)
		bool increasingFOV = false;

	float lowestVerticeZ;
	float zPosition;
	float currentFOV = 100;
		UPROPERTY(EditAnywhere)
	float newFOV;

public:	
	float GetCurrentSpeed();
	void CalculateSpeed();

	void SetSpawnPosition();

	/** handles input functions for UE4
	 *
	 */
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

	void HasToActivatePlayer(bool playerState);
	bool GetGameState();

	void StopMoving(bool stop);
	void CameraShake();

	UFUNCTION()
			void OnDeath();

	void Die();
	void SetFieldOfViewTo(int newValue, int seconds);
	void MultiplySpeedBy(float muliplier, int seconds);
	void ResetFieldOfView();
	void ResetSpeed();

	float normalSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FVector oldPositionLength;
	FVector newPositionLength;
	FVector newVelocity;

	UWorld* World;

	FTimerHandle timerHandler;
	FTimerHandle fieldOfViewHandler;
	FTimerHandle deathDelayHandler;
	FTimerHandle slowDownHandler;

	FVector currentPivot;
	TArray<FVector> trackPivots;
	int pivotIndex = 5;
	bool snowballCollision = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void AddTrackPivots(FVector pivot);

};