// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionCameraShake.h"

UCollisionCameraShake::UCollisionCameraShake() {
	OscillationDuration = 1.25f;
	OscillationBlendInTime = 0.5f;
	OscillationBlendOutTime = 0.5f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(1, 5);
	RotOscillation.Pitch.Frequency = FMath::RandRange(10, 15);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(1, 5);
	RotOscillation.Yaw.Frequency = FMath::RandRange(10, 15);

}