// Fill out your copyright notice in the Description page of Project Settings.

#include "PenguinController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
// Sets default values
APenguinController::APenguinController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;/*
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->SetupAttachment(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 400.f;
	OurCameraSpringArm->bEnableCameraLag = true;
	//OurCameraSprintArm->CameraLagSpeed = 3.0f;
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->SetupAttachment(OurCameraSpringArm, USpringArmComponent::SocketName);*/

}

// Called when the game starts or when spawned
void APenguinController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APenguinController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*OurCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
	OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);

	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}
	{
		FRotator NewRotation = OurCameraSpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
		OurCameraSpringArm->SetWorldRotation(NewRotation);
	}
	    {
        if (!MovementInput.IsZero())
        {
            //Scale our movement input axis values by 100 units per second
            MovementInput = MovementInput.GetSafeNormal() * 100.0f;
            FVector NewLocation = GetActorLocation();
            NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
            NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
            SetActorLocation(NewLocation);
        }
    }*/
}

// Called to bind functionality to input
void APenguinController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("Movement_UpDown", this, &APenguinController::Movement_UpDown);
	InputComponent->BindAxis("Movement_LeftRight", this, &APenguinController::Movement_LeftRight);
	InputComponent->BindAxis("CameraY", this, &APenguinController::PitchCamera);
	InputComponent->BindAxis("CameraX", this, &APenguinController::YawCamera);
}

void APenguinController::Movement_LeftRight(float amount)
{
	if (Controller && amount)
	{
		AddMovementInput(GetActorForwardVector(), amount);
	}
}

void APenguinController::Movement_UpDown(float amount)
{
	if (Controller && amount)
	{
		AddMovementInput(GetActorRightVector(), amount);
	}
}

void APenguinController::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void APenguinController::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}
