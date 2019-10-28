/*!
 *  \brief     This class handles the shake of the camera of the player
 *  \details   This class makes the screen do shakyshake
 *  \author    Isabel
 *  \version   1
 *  \date      01/06/2019
 *  \copyright Isabel
 */
#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "CollisionCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class PENGUINGLIDE_API UCollisionCameraShake : public UCameraShake
{
	GENERATED_BODY()

public:
	UCollisionCameraShake();
};
