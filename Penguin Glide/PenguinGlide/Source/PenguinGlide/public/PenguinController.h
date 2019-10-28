// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PenguinController.generated.h"

UCLASS()
class PENGUINGLIDE_API APenguinController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APenguinController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* OurCameraSpringArm;
	class UCameraComponent* OurCamera;

public:
	FVector2D MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	bool bZoomingIn;

	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);    
	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Movement_LeftRight(float amount);

	void Movement_UpDown(float amount);


};
