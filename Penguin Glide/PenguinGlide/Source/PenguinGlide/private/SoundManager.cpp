// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/OutputDevice.h"
#include "Core.h"

bool isPlaying;
int speedDelay;
// Sets default values
ASoundManager::ASoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isPlaying = false;
}

// Called when the game starts or when spawned
void ASoundManager::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ASoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	speedDelay++;
	if (speedDelay > 200) {
		isPlaying = false;
	}

}

void ASoundManager::PlayPickupSound() {
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlayImpactSound"), ar, NULL, true);
}
void ASoundManager::PlayWahh() {
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlayWahh"), ar, NULL, true);
}

void ASoundManager::PlayGravityUp() {
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlayGravityDown"), ar, NULL, true);
}

void ASoundManager::PlayGravityDown() {
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlayGravityDown"), ar, NULL, true);
}

void ASoundManager::PlayIcecrack() {
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlayIcecrack"), ar, NULL, true);
}

void ASoundManager::PlayShake() {
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlayShake"), ar, NULL, true);
}

void ASoundManager::PlayMultiplier() {
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlayMultiplier"), ar, NULL, true);
}
void ASoundManager::PlayInstantPoints() {
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlayInstantPoints"), ar, NULL, true);
}

void ASoundManager::StartFadeOut() {
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("StartFadeOut"), ar, NULL, true);
}

void ASoundManager::PlaySpeedUpSound() {
	if (!isPlaying)
	{
		isPlaying = true;
		speedDelay = 0;
		FOutputDeviceNull ar;
		this->CallFunctionByNameWithArguments(TEXT("PlaySpeedUpSound"), ar, NULL, true);
	}
}
