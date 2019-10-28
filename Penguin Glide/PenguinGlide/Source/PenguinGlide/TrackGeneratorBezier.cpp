
#include "TrackGeneratorBezier.h"

#include <iostream>
#include <vector>

#include "SpawnableObject.h"
#include "TempPlayer.h"
#include "Snowball.h"
#include "Runtime/Engine/Public/EngineUtils.h"

#include "DrawDebugHelpers.h"

const int pointSpacer = 30; //!< The amount of points along the curve
const int nodes = 4; //!< Our bezier curve always uses 4 nodes

TArray<MeshSection> meshes; //!< Array holding the created meshes
ASpawnableObject* spawnableObject; //!< Pointer to the spawnableObject

//!< Sets default values
ATrackGeneratorBezier::ATrackGeneratorBezier()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;

	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	mesh->bUseAsyncCooking = true;
	arraySize = pointSpacer;
}

/**
 *BackToFront manages the re-use of the existing meshes to create new meshes. It passes the mesh before the newly created one to the
 *CreateMesh function so the meshes align properly. Meshes the player has already passed are overwritten.
 */
void ATrackGeneratorBezier::BackToFront()
{
	int meshesSize = meshes.Num(); //!< Gets the number of meshes existing
	if (placementCounter == 0) //!< If the placementCounter is 0, it has to call the last mesh in the array instead of just the previous one
	{
		meshes[placementCounter] = ChangeSection(meshes[meshesSize - 1]); 
	}
	else
	{
		meshes[placementCounter] = ChangeSection(meshes[placementCounter - 1]);
	}
	CreateMesh(meshes[placementCounter].points, placementCounter);//!< Creates the mesh using the points of the section before it
	placementCounter++;
	if (placementCounter == meshesSize) //!< Once the counter goes over the amount of meshes in the array, reset it to 0
	{
		placementCounter = 0;
	}

}

/**
 *Manages the creation of the mesh sections and calls the creation of the objects that are spawned on the track
 *that depend on the vertices of the track. These objects can be created at the same time as the mesh, so it is included at the
 *same time as the meshes are created
 */
void ATrackGeneratorBezier::CreateMesh(TArray<FVector> points, int counter)
{
	if (counter == 2) {
		canSpawnObjects = true;
	}
    triangleAmount = 30; //! Set triangleAmount to an even number. These are the amount of horizontal vertices
	triangleAmount -= triangleAmount % 2; //! incase someone sets triangleAmount to an uneven number, this reduces it by 1
	int vertAmount = (triangleAmount / 2) + 1;

	TArray<FVector> vertices; //!< FVector array to hold the vertices
	TArray<int32> triangles; //!< int32 array to hold the number of the triangle sides
	TArray<FVector2D> uvs; //!< FVector2D array to hold uvs used for the material of the track
	TArray<FVector> normals; //!<array of VERTEX normals

	CreateVertices(points, vertices, normals); //! Creates all the vertices used in the mesh

	if (canSpawnObjects) {
		int row = FMath::RandRange(-3, 3); //! Random row used to spawn objects
		int column = FMath::RandRange(4, arraySize - 4); //! Random coloum used to spawn objects
		int wakIndex = (vertAmount / 2) + (vertAmount * column) + row; //! 
		int wakDepth = 300;
		FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		if (playerPos.X > obstacleRampup * 2) {
			CreateWakParticles(FVector(vertices[wakIndex].X * trackScale.X,
				vertices[wakIndex].Y * trackScale.Y,
				vertices[wakIndex].Z * trackScale.Z)); //!< Creates the particles for the wak and sets their location
			vertices[wakIndex].Z -= wakDepth; //!< Wak is created at the vertex index. The depth of the wak is the assigned variable.
		}

		if (playerPos.X > obstacleRampup * 4) {
			if (counter % 2) //!< Only calls the method every other mesh to prevent too many snowballs
			{
				CreateSnowball(vertices, vertAmount, column); //!< Creates the snowball by given the vertices to the method and spawning the snowball on the vertice position
			}
		}

		if (playerPos.X > obstacleRampup * 3) {
			spawnableObject->GetVertices(vertices, trackScale, normals); //!< Sets the position of the spawnableObject
		}

		if (playerPos.X > obstacleRampup / 2){
			speedBoostGenerator->GenerateSpeedBoost(vertices, normals, vertAmount, counter, trackScale, column); //!< sets the position of the speedboostGenerator
		 }
		}
		

	CreateTriangles(vertAmount, &triangles); //!< Creates the triangles from the 

	float yArcLength = CalculateArcLengthY(vertices, vertAmount); //!< Returns the length of the vertical length of the curve of the track
	float xArcLength = CalculateArcLengthX(vertices, arraySize, vertAmount);//!< Returns the length of the horizontal curve of the track

	for (int x = 0; x < arraySize; x++) {
		for (int y = 0; y < vertAmount; y++) {
			uvs.Add(FVector2D(((yArcLength / (vertAmount - 1)) * y) / yArcLength, ((xArcLength / (arraySize - 1)) * x) / xArcLength)); //!< Adds the created uvs to the uv array
		}
	}

	endingVertsInitialized = true;
	zPosition += steepness; //!< Increases so each mesh curves downwards
	yPerSection++; //!< increased by one per mesh to create an offset for the built-in physics engine

	mesh->CreateMeshSection_LinearColor( //!< Creates the mesh using UProceduralMeshComponent function
		counter,
		vertices,
		triangles,
		normals,
		uvs,
		TArray<FLinearColor>(),
		TArray<FProcMeshTangent>(),
		true);
	mesh->SetMaterial(counter, iceMaterial); //!< assigns the mesh with a material
	uvs.Empty(); //!< Empties the uv array as unreal saves some of the information between builds which can cause errors

}

void ATrackGeneratorBezier::CreateVertices(TArray<FVector>& points, TArray<FVector>& vertices, TArray<FVector>& normals) {
	std::vector<FVector> verts(arraySize * 2); //<! vectorarray containing the horizontal vertices, no curve yet
	FVector firstControlPoint; //!< Leftvertice of the vertice row
	FVector secondControlPoint; //!< Rightvertice of the vertice row
	FVector pivot; //!< Pivot point of the track used for normals

	int vertAmount = (triangleAmount / 2) + 1; //!< amount of vertices on the track
	int vertIndex = 0; //!< keeps track of which vertice is used
	float trackWidth = 100.0f;
	int blobCheck = (int)FMath::RandRange(1, 4);
	for (int i = 0; i < arraySize; i++)
	{
		FVector forwardDirection = FVector::ZeroVector; //!< sets the forwardDirection to 0,0,0 because we are adding unto it.
		if (i < arraySize - 1)
		{
			forwardDirection += points[i + 1] - points[i];
		}

		if (i > 0) //!< First point doesn't contain a previous point, so starts at 1 instead of 0
		{
			forwardDirection += points[i] - points[i - 1];
		}
		forwardDirection.Normalize(); //!< Normalizes the created forward direction

		FVector left = FVector(-forwardDirection.Y, forwardDirection.X, 0); //!< Left point in the direction of the forwardvector

		if (blobCheck % 4 == 0) {
			if (i < arraySize / 2) {
				trackWidth += trackBlob;
			}

			if (i >= arraySize / 2) {
				trackWidth -= trackBlob;
			}
		}

		verts[vertIndex] = points[i] + left * trackWidth; //!< Creates the left vertice in the direction of the point
		verts[vertIndex].Z += zPosition; //!< Adds z-position for the curve
		verts[vertIndex + 1] = points[i] - left * trackWidth; //!< Creates the right vertice in the direction of the point
		verts[vertIndex + 1].Z += zPosition; //!< Adds z-position for the curve
		zPosition -= steepness; //!< substract the constant steepness factor for the next mesh

		int trackDepth = 80;
		firstControlPoint = verts[vertIndex] - FVector(0, 0, trackDepth); //!< Assigns the first control point
		secondControlPoint = verts[vertIndex + 1] - FVector(0, 0, trackDepth); //!< Assigns the second control point

		float t = 0.0f;

		pivot = (verts[vertIndex] + (verts[vertIndex + 1] - verts[vertIndex]) / 2); //!< Calculates the pivot point of the track

		tempPlayer->AddTrackPivots(pivot * trackScale); //!< Gives the tempplayer the pivot points for its rotation

		for (int x = 0; x < vertAmount; x++)
		{
			//!< use cubic B�zier curve algorithm to calculate points on a curve in between two points
			FVector vertexPosition = pow((1 - t), 3) * verts[vertIndex] + (3 * t) * pow((1 - t), 2) * firstControlPoint
				+ (3 * pow(t, 2)) * (1 - t) * secondControlPoint + pow(t, 3) * verts[vertIndex + 1];

			if (i == 0 && endingVertsInitialized)
			{
				vertices.Add(endingVerts[x]); //!< set the first row of vertices equal to the last row of the previous mesh section
				if (x == vertAmount - 1)
				{
					endingVerts.Empty(); //!< empty endingVerts, to make room of the endingVerts of current mesh section
				}
			}
			else
			{
				vertices.Add(vertexPosition);
			}
			if (i == arraySize - 1)
			{
				endingVerts.Add(vertexPosition); //!< last row of mesh section, add vertices to endingVerts array
			}

			t += 1 / ((float)vertAmount - 1); //!< increase t to shift to the next point on the curve
			FVector normal = pivot - vertices[x + vertAmount * i];
			float normalLength;
			normalLength = sqrtf(powf(normal.X, 2) + powf(normal.Y, 2) + powf(normal.Z, 2));
			normal /= normalLength;
			normals.Add(normal);
		}
		vertIndex += 2;
	}

	int sizeVertices = vertices.Num();
	for (int i = 0; i < sizeVertices - 1; i++)
	{
		vertices[i].Z -= yPerSectBase * yPerSection;
	}
}

void ATrackGeneratorBezier::CreateTriangles(int vertAmount, TArray<int32>* triangles) {
	//!< loop through all the rows of a mesh section
	for (int x = 0; x < arraySize - 1; x++)
	{
		//!< loop through all the triangles of a row
		for (int i = 0; i < triangleAmount / 2; i++)
		{
			//!< create first triangle of face
			triangles->Add(i + x * vertAmount);
			triangles->Add(vertAmount + i + x * vertAmount);
			triangles->Add(1 + i + x * vertAmount);

			//!< create second triangle of face
			triangles->Add(vertAmount + i + x * vertAmount);
			triangles->Add(vertAmount + 1 + i + x * vertAmount);
			triangles->Add(1 + i + x * vertAmount);
		}
	}
}

void ATrackGeneratorBezier::CreateSnowball(TArray<FVector>& vertices, int vertAmount, int column) {
	FActorSpawnParameters spawn; //!< Spawnparameters needed for the spawactor function
	TArray<FVector> snowballPositions; //!< FVector array for holding the positions used for doing its movement
	float randomSnowballPositionOne = FMath::RandRange(0, pointSpacer - 1); //!< random location on the track along its curve
	float randomSnowballPositionTwo = FMath::RandRange(0, pointSpacer - 1); //!< random location on the track along its curve
	while (randomSnowballPositionOne == column) //!< if the position is the same as a position already used, get a new position
	{
		randomSnowballPositionOne = FMath::RandRange(0, pointSpacer - 1);
	}
	while (randomSnowballPositionOne == randomSnowballPositionTwo || randomSnowballPositionTwo == column) //!< if the position is the same as a position already used, get a new position
	{
		randomSnowballPositionTwo = FMath::RandRange(0, pointSpacer - 1);
	}
	snowballPositions.Add(vertices[vertAmount * randomSnowballPositionOne] * trackScale); //!< Adds the left position for the movement
	snowballPositions.Add(vertices[vertAmount * randomSnowballPositionOne + (vertAmount / 3)] * trackScale); //!< adds the left control point
	snowballPositions.Add(vertices[vertAmount * randomSnowballPositionOne + (vertAmount / 3 * 2)] * trackScale); //!< adds the right control point
	snowballPositions.Add(vertices[vertAmount * randomSnowballPositionOne + (vertAmount - 1)] * trackScale); //!< Adds the right position for the movement

	ASnowball* snowballone = GetWorld()->SpawnActor<ASnowball>(
		vertices[vertAmount * randomSnowballPositionOne] * trackScale,
		FRotator::ZeroRotator,
		spawn); //!< uses the UFunction SpawnActor to create the snowball in the game
	snowballone->SetPositions(snowballPositions, zOffset);

	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	if (playerPos.X > obstacleRampup * 6) {
		int spawnSecond = FMath::RandRange(1, 2);
		if (spawnSecond == 1)
		{
			snowballPositions.Empty(); //!< Empties the position array to be used again
			snowballPositions.Add(vertices[vertAmount * randomSnowballPositionTwo] * trackScale);
			snowballPositions.Add(vertices[vertAmount * randomSnowballPositionTwo + (vertAmount / 3)] * trackScale);
			snowballPositions.Add(vertices[vertAmount * randomSnowballPositionTwo + (vertAmount / 3 * 2)] * trackScale);
			snowballPositions.Add(vertices[vertAmount * randomSnowballPositionTwo + (vertAmount - 1)] * trackScale);

			ASnowball* snowballtwo = GetWorld()->SpawnActor<ASnowball>(vertices[vertAmount * randomSnowballPositionTwo] * trackScale, FRotator::ZeroRotator, spawn);
			snowballtwo->SetPositions(snowballPositions, zOffset);
		}
	}
}

void ATrackGeneratorBezier::CreateWakParticles(FVector pos)
{
	FActorSpawnParameters SpawnInfo; //!< Default spawnParameters for SpawnActor
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //!< Makes sure the particlesystem always spawns
	auto ParticleActor = GetWorld()->SpawnActor<AWakParticleSystem>(pos, FRotator::ZeroRotator, SpawnInfo); //!< Spawns to actor
}

float ATrackGeneratorBezier::GetEndingVerts()
{
	float endingVert = endingVerts[endingVerts.Num() - 1 - 8].Z * trackScale.Z; //!< Gets the Z position of the middle vertice at the end of the mesh
	return endingVert;
}

MeshSection ATrackGeneratorBezier::ChangeSection(MeshSection sect)
{
	MeshSection newSect(pointSpacer); //!< Creates a new mesh section
	newSect.UpdateMesh(sect.nodes[3], sect.nodes[2]); //!< assigns the new mesh its positions based on the previous mesh section given
	return newSect;
}

FVector ATrackGeneratorBezier::GetPlayerSpawnPosition(FVector rightVert, FVector leftVert)
{
	FVector width = rightVert - leftVert; //!< Calculates the width between the left and right vertices on the horizontal axis
	spawnPosition = rightVert + (width / 2); //!< sets the position to the middle
	spawnPosition.X += 40; //!< Sets the position forward a bit to make sure it doesn't spawn outside of the track and falls off
	spawnPositionInitialized = true; //!< sets the position to initialized
	return spawnPosition;
}

/**
 *Calculates the curve length in the vertical axis
 */
float ATrackGeneratorBezier::CalculateArcLengthY(TArray<FVector>& vertices, int& yCount)
{
	float arcLength = 0;
	for (int i = 0; i < yCount - 1; i++) 
	{
		FVector trackScale = this->GetActorScale();
		FVector segmentDistance = (vertices[i] - vertices[i + 1]);//!< Calculates the distance from one point to the next
		arcLength += sqrt(pow(segmentDistance.X, 2) + pow(segmentDistance.Y, 2) + pow(segmentDistance.Z, 2)); //!< Used a mathmatical function to calculate the curve length
	}
	return arcLength;
}

/**
 *Calculates the curve length in the horizontal axis
 */
float ATrackGeneratorBezier::CalculateArcLengthX(TArray<FVector>& vertices, int& xCount, int& yCount)
{
	float arcLength = 0;
	for (int i = 0; i < xCount - 1; i++) 
	{
		FVector segmentDistance = (vertices[i * yCount] - vertices[(i + 1) * yCount]); //!< Calculates the distance from one point to the next
		arcLength += sqrt(pow(segmentDistance.X, 2) + pow(segmentDistance.Y, 2) + pow(segmentDistance.Z, 2)); //!< Used a mathmatical function to calculate the curve length
	}
	return arcLength;
}

void ATrackGeneratorBezier::BeginPlay()
{
	Super::BeginPlay();
	trackScale = this->GetActorScale(); //!< sets the scale of the actor assigned in the editor
	endingVertsInitialized = false; //!< endingVerts don't exist at the start
	meshes.Empty(); //!< It stores meshes somewhere in between plays, have to always empty it at the start or it will do funky things

	TArray<FVector> initialPoints;

	//!< Call ATrackGeneratorBezier::BackToFront() every spawnSpeed seconds, set true to loop
	for (int i = 0; i < nodes * 2; i++)
	{
		meshes.Add(MeshSection(pointSpacer));
	}
	meshes[0].InitializeMesh(initialNodes); //!< Have to create the first mesh with a different method as it doesn't have previous nodes yet
	CreateMesh(meshes[0].points, 0); //!< Creates the second mesh manually as it needs the first mesh that has no previous meshes
	for (int i = 1; i < nodes * 2; i++)
	{ //!< starts at 1 because 0 has already been done
		meshes[i] = ChangeSection(meshes[i - 1]);
		CreateMesh(meshes[i].points, i);
	}
}

void ATrackGeneratorBezier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mesh->GetProcMeshSection(replacementSection) != nullptr) //!< Only do this if the mesh exists
	{
		int sectionVertAmount = mesh->GetProcMeshSection(replacementSection)->ProcVertexBuffer.Num();
		FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(); //!< Get player position

	  /**
		*Manages the respawn of the mesh sections based on player position
		*/
		if (playerPos.X > mesh->GetProcMeshSection(replacementSection)->ProcVertexBuffer[sectionVertAmount - 1].Position.X * trackScale.X
			&& playerPos.Z < mesh->GetProcMeshSection(replacementSection)->ProcVertexBuffer[sectionVertAmount - 1].Position.Z * trackScale.Z)
		{
			BackToFront();
			replacementSection++;
			replacementSection %= 8;

			int rowSpawn = 248;
			int offset = FMath::RandRange(-2, 2);
			rowSpawn += offset;

			FVector fallingObjectLocation = mesh->GetProcMeshSection(replacementSection)->ProcVertexBuffer[rowSpawn].Position * trackScale;
			FVector fallingObjectNormal = mesh->GetProcMeshSection(replacementSection)->ProcVertexBuffer[rowSpawn].Normal;
			UE_LOG(LogTemp, Error, TEXT("FallingObjectLocation: %s, index: %f"), *fallingObjectNormal.ToString(), (float)replacementSection);
			fallingObjectSpawner->Spawn(fallingObjectLocation, fallingObjectNormal);//mesh->GetProcMeshSection(fallingObjectSpawnIndex)->ProcVertexBuffer[sectionVertAmount - 15].Position * trackScale, mesh->GetProcMeshSection(fallingObjectSpawnIndex)->ProcVertexBuffer[sectionVertAmount - 15].Normal);
			//mesh->GetProcMeshSection(replacementSection)->ProcVertexBuffer[sectionVertAmount - 1].Normal;
		}
	}
}
