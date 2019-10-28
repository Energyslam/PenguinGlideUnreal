#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/Public/TimerManager.h"
#include "ProceduralMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "WakParticleSystem.h"
#include "ParticleDefinitions.h"
#include "FallingObjectSpawner.h"
#include "SpeedBoostGenerator.h"
#include "TempPlayer.h"
#include "MeshSection.h"
#include "TrackGeneratorBezier.generated.h"

class ATempPlayer;
UCLASS()
/*!
 *  \brief     This class takes care of the procedurally generated track mesh
 *  \details   For the generation of the track, Unreal Engines ProceduralMeshComponent is used. This component makes it
 *  possible to create a procedurally generated mesh. To calculate the shape of the mesh, we use an algorithm to calculate
 *  the points on a cubic Bezier curve. This way, the track wil be curvy and interesting for the player while also
 *  ensuring that the generated curve looks natural, like a river. This algorithm is also used to create the half pipe
 *  shape of the mesh.
 *
 *  The track is composed out of several sections. When the game starts, 4 of these mesh sections are created which all fit to
 *  one another. The process of creating these mesh sections is as follows:
 *
 *  The following steps are taken to create each mesh section:
 *  - Four random positioned nodes are taken for each mesh section, used to calculate the curve of the mesh section
 *  - The last node is equal to the first node of following mesh section to make sure the mesh sections connect
 *  - The vertices and the triangles are calculated, and based on the result, the mesh will be rendered
 *  - The vertex normals are calculating by normalizing the difference between the vertices and the origin
 *  - The uv's are calculated to be able to use a material on the track
 *  - Mesh will be rendered
 *
 *  When the player has reached a certain position on the track, the mesh section on the back will be placed to the front
 *  to prevent the mesh from getting infinitely long. This prevents unnecessary calculations and unnecessary rendering.
 *
 *  This generator is also used to create snowballs, speedboosts and power-ups as they rely on the positions of the
 *  vertices. This is done by giving the vertice array to the appropriate method in their respective classes.
 *
 *  <b>Note: This script should be placed in the scene for the track to be able to spawn.</b>
 *  \author    Max Groen & Lisa Groen
 *  \version   1
 *  \date      09/06/2019
 */
class PENGUINGLIDE_API ATrackGeneratorBezier : public AActor
{
	GENERATED_BODY()

public:
	/** \brief Called every frame.
	 *	\details   All the data of the track will be calculated, the track will be drawn and respawned
	*/
	virtual void Tick(float DeltaTime) override;

public:	
	const int steepness = 1;

	UPROPERTY(EditAnywhere)
		int obstacleRampup = 12000;

	int arraySize;
	int placementCounter = 0;
	int yPerSection = 0;
	int zPosition = 0; //!< \initial z position of the track
	int replacementSection = 3; //!< \Mesh section to be replaced, based on player position
	int triangleAmount;

	UPROPERTY(EditAnywhere)
		float trackBlob = 15.0f;
	UPROPERTY(EditAnywhere)
		int yPerSectBase;

	UPROPERTY(EditAnywhere)
		float spawnSpeed = 3.0f;

	UPROPERTY(EditAnywhere)
		float zOffset;
		
	FVector spawnPosition; //!< \SpawnPosition of the player

	UPROPERTY(EditAnywhere)
		TArray<FVector> initialNodes = { 
									FVector(0,0,0),
									FVector(50, 50, 0), 
									FVector(200, -50, 0), 
									FVector(400, 100, 0) 
									}; //!< \Nodes of first mesh section

	UPROPERTY(EditAnywhere)
		class ASpeedBoostGenerator* speedBoostGenerator;

	UPROPERTY(EditAnywhere)
		class AFallingObjectSpawner* fallingObjectSpawner;

	UPROPERTY(EditAnywhere)
		class ATempPlayer* tempPlayer;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* iceMaterial; //!< \Material of the track

	UPROPERTY(EditAnywhere)
		UMaterialInterface* speedBoostMaterial;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* wakParticleSystem;


	/** \brief Constructor
	 * \details - All tick events are enabled
	 * - The Track Mesh is set as root component from the class
	*/
	ATrackGeneratorBezier();

	void CreateSnowball(TArray<FVector>& verts, int vertAmount, int column);
	void CreateTriangles(int verticeAmount, TArray<int32>* triangles);
	void CreateVertices(TArray<FVector>& points, TArray<FVector>& vertices, TArray<FVector>& normals);

	/** \brief Takes the mesh section on the back of the track and places it to the front of the track
	 *\details  Gets called based on where the player is located on the track. This is done for performance reasons, so the mesh
	 *does not keep growing in amount of faces to be rendered.
	 *
	 *- Calls ChangeSection() for the mesh section on the back of the track, to calculate the new mesh section
	 *- Calls CreateMesh() to calculate the new mesh data and render the mesh section
	 *- Changes placementcounter to keep track of the index of the mesh section on the back
	 *
	 *<b>Note: </b> 
	*/
	void BackToFront();

	/** \brief Creates a particle system at the location of the wak to indicate danger
	 *\details Gets called based on where the player is located on the track
	*/
	void CreateWakParticles(FVector);

	/** \brief Calculates the vertices and triangles of the mesh and draws the mesh
	*
	*/
	void CreateMesh(TArray<FVector> points, int counter);

	/** \brief Changes the nodes of the mesh section and makes sure the pieces roughly connect
	*
	*/
	MeshSection ChangeSection(MeshSection sect);

	/** \brief Gets coordinates on the track to spawn the player
	*
	*/
	FVector GetPlayerSpawnPosition(FVector rightVert, FVector leftVert);

	/** \brief Calculates the arc length of a given curve
	*
	*/
	float CalculateArcLengthY(TArray<FVector>& vertices, int& yCount);
	float CalculateArcLengthX(TArray<FVector>& vertices, int& xCount, int& yCount);

	/** \brief Returns the lowest vertice of the track
	*
	*/
	float GetEndingVerts();

protected:
	/** \Called when the game starts or when spawned
	*
	*/
	virtual void BeginPlay() override;

private:
	bool endingVertsInitialized; //!< \Determines whether endingVerts array is initialized
	bool spawnPositionInitialized = false;
	bool canSpawnObjects = false;
	UProceduralMeshComponent *mesh;
	UProceduralMeshComponent *speedBoost;
	FVector trackScale;
	TArray<FVector> endingVerts; //!< \Holds ending vertices of most recently spawned mesh section
	FTimerHandle timehandle; //!< \Handle to manage the timer
};