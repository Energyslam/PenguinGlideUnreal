#pragma once
#include <iostream>
#include <vector>
#include "PowerUp.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnableObject.generated.h"

UCLASS()
/*!
 *  \brief     This class takes care of spawning powerups on the track
 *  \details   Each time a new mesh section is created in the class TrackGeneratorBezier, the function GetVertices() is called in the function
 *  CreateMesh() of TrackGeneratorBezier. Spawning the objects takes place in the following order:
 *
 *  - GetVertices() get called to get the vertices of the track on which powerups can be spawned
 *  - From all the vertices on the track, a random one will be chosen on which the powerup will spawn
 *  - If there is already an other object on that vertex, a new one will be picked untill it finds one which is free
 *  - The powerup will get a small offset in the direction of the normal of the vertex it spawns on
 *	 - SpawnObject() is called to spawn the powerup.
 *
 *  This process will repeat itself each time a new mesh section spawns.

 *  <b>Design choices</b>
 *  We chose to spawn a powerup each time a new mesh seciton is created, because we want the powerups to spawn frequently since they sometimes
 *  might be tricky to pick up. This way the player has enough oppurtunities to grab a powerup. We also chose to give the powerup a small offset
 *  from the track to give the impression its floating above the track.
 *
 *  <b>Note: - This script should be placed in the scene for the meshes to be able to spawn.
 *  - The actor blueprint of the object to be spawned should be assigned in the inspector.</b>
 *  \author    Lisa Groen
 *  \version   1
 *  \date      10/3/2019
 */
class PENGUINGLIDE_API ASpawnableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	/** \brief Constructor
	 *	\details   - Actors tickevents are set to true.
	*/
	ASpawnableObject();

	/** \brief This Function is responsible for spawning the objects.
	 *	\details Objects get spawned using Unreal Engines World->SpawnActor<AActor>() 
	*/
	void SpawnObject();

	/** \brief This function is responsible for getting the track vertices and setting the exact location on which the powerup will spawn
	 *	\details - Takes a random vertex on the track untill it finds a vertex on which nothing has spawned yet
	 * - Gives a small offset in the direction of the vertex normal to create a floating effect
	 *
	 * <h3>Parameters</h3>
	 *	-  <b style="color:#80b3ff;">\a TArray<FVector> vertices</b> The vertices of the track
	 *	-  <b style="color:#80b3ff;">\a FVector trackScale</b> The scale of the track, the vertex should be multiplied by this number to find the exact location
	 *	-  <b style="color:#80b3ff;">\a TArray<FVector> normals</b> The vertex normals of the track, is used to calculate the offset of the powerups.
	*/
	FVector GetVertices(TArray<FVector> vertices, FVector trackScale, TArray<FVector> normals);

private:
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class AActor>> powerUpsToSpawn;

public:	
	/** \brief Tick function of UE4, is called every frame
	 *	\details calls SpawnObject() if the spawning position is calculated
	*/
	virtual void Tick(float DeltaTime) override;

};
