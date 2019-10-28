// Fill out your copyright notice in the Description page of Project Settings.

#include "BloodscreenClass.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/OutputDevice.h"
#include "Core.h"

// Sets default values
ABloodscreenClass::ABloodscreenClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABloodscreenClass::BeginPlay()
{
	Super::BeginPlay();
	//FOutputDeviceNull ar;
	//this->CallFunctionByNameWithArguments(TEXT("KutBeginPlay"), ar, NULL, true);
}

// Called every frame
void ABloodscreenClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABloodscreenClass::createWidget() {
	FOutputDeviceNull ar;
   this->CallFunctionByNameWithArguments(TEXT("KutBeginPlay"), ar, NULL, true);
}

