#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundManager.h"
#include "Materials/Material.h"
#include "Snowball.generated.h"

UCLASS()
/*!
 *  \brief     This class is used to handle the snowball movement and collision with the player
 *  \details   The snowball uses a bezier curve to move across the horizontal curve of the track. A bezier curve is chosen
 *					because there was familiarity with the math and having two control points allows for more control over the curve
 *					that it follows. The track will give the node positions during the mesh creation. It relies on the left and right vertices
 *					for its start and end node, and the amount of vertices on the horizontal axis divided by 2/4 and 3/4 for its control points.
 *					The t for the lerp function of the bezier curve continuously counts up and down so the snowball moves positions. This is chosen
 *					over using actual physics due to difficulty of having the snowball follow the correct path using the physics engine.
 *					It has a pointer to the SoundManager in the level to play the impact sound as we're not using a blueprint for the snowball. Playing
 *					sound through c++ has proven to be difficult to implement.
 *
 *  \author    Max Groen
 *  \version   1
 *  \date      04/06/2019
 *  \copyright Energyslam™
 */
class PENGUINGLIDE_API ASnowball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnowball();

	UPROPERTY(EditAnywhere)
		int amazingTimer; //!< manual timer so the snowball does not keep triggering sound when it has a collision with the player

	UPROPERTY(EditAnywhere)
		float lerpValue; //!< the position of the curve it is currently at between 0 and 1

	float speedValue; //!< float for the speed at which the snowball lerps
	bool countingDown; //!< bool keeping track of whether the snowball is moving up or down (between 0 and 1)
	TArray<FVector> lerpPositions; //!< positions which the snowball moves between
	UMaterial* StoredMaterial; //!< Temporary stored material pointer used for the constructor helper

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* sphereVisual; //!< pointer to static mesh component of the actor

	UPROPERTY(EditAnywhere)
		class USphereComponent* sphereComponent; //!< The sphere used for its collision box

	UPROPERTY(EditAnywhere)
		class ASoundManager* soundManager; //!< pointer to the soundmanager so it can play sound on collision

	void SetPositions(TArray<FVector> lerpPos, float offset); //!< initializes the positions for the bezier curve
	void OnDeath(); //!< function to handle player death

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnCompOverlap(class UPrimitiveComponent *ThisComp, AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
