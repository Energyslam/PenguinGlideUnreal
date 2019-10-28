#pragma once
#include "SoundManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Engine/Public/TimerManager.h"
#include "TempPlayer.h"
#include "SpeedBoostGenerator.generated.h"

class ATempPlayer;
UCLASS()
/*!
 *  \brief     This class generates the speedboost and ramp
 *  \details   For the generation of the meshes, Unreal Engines ProceduralMeshComponent is used. This component makes it
 *  possible to create a procedurally generated mesh. The function responsible for generating the meshes is called in the 
 *  track class, each time a new part of the track gets spawned, because the newly spawned track part is still empty and yet 
 *  has to get a speedboost.
 *
 *  <b>Generation process</b>
 *  The speedboosts gets spawned on a random location in the middle of the track. To find the position of where to be generated,
 *  it uses the vertex coordinates of the track. Based on the position of 1 vertex of the track, the rest of the speedboost
 *  gets generated. Before the generating process will happen, there will be checked whether there is already an object on the chosen
 *  random position. If there is already an object on that position, a new random position on the track will be chosen untill one is found
 *  which is empty. There are two seperate functions which are responsible for calculating the vertex and the triangle data. After 
 *  the speedboost is generated, there is a 50 percent change that a ramp will be generated right after the speedboost. There is a
 *  seperate function that handles the generation of the ramp.
 *
 *  <b>Speed up process</b>
 *  For the speedboost, overlap events are enabled in the constructor. The speedboost mesh is set as rootcomponent, because otherwise
 *  collission detection cannot take place. Every tick, there will be checked whether the speedboost overlaps with the player. If this is
 *  true the following events will take place:
 *  - the speed of the player will be multiplied with a value greater than 1
 *  - the field of view will temporarily increase in size
 *  - a sound will be played through the SoundManager class
 *
 *  <b>Design choices</b>
 *  I chose to use mesh generation for creating the arrows, because this way it can easily fit exactly on the generated track, but still
 *  have collision detection on its own. The ramp also gets generated in this class because it is part of the speedboost as well;
 *  it always gets generated, paired together with the speedboost. This is because the player needs a speedboost before entering the ramp
 *  to get launched in the air.
 *
 *  <b>Note: This script should be placed in the scene for the meshes to be able to spawn.</b>
 *  \author    Lisa Groen
 *  \version   1
 *  \date      11/06/2019
 */
class PENGUINGLIDE_API ASpeedBoostGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	/** \brief Constructor
	 *	\details   - The procedural mesh component of the speedboost and ramp are initialized.
	 *	- All overlap settings are set for the speedboost to be able to detect overlap events.
	*/	
	ASpeedBoostGenerator();

	/** \brief Generates the mesh for the speedboost
	 *	\details   - Checks what position on the track is available (there is no other object on that spot present), untill it finds an emtpy spot
	 *	- Calls CalculateSpeedboostVertices() to calculate the vertices of the speedboost
	 *	- Calls CreateTriangle() to create each triangle of the mesh
	 *	- Generates a random int which makes sure that there is a 50% chance of calling CreateRamp()
	*/
	void GenerateSpeedBoost(TArray<FVector> vertices, TArray<FVector> trackNormals, int trackVertAmount, int counter, FVector trackScale, int engagedVert);

protected:
	/** \brief Calls when game starts or when spawned in the scene
	 *	\details   Starts overlap detection of the speedboost and loops through the inspector to find the soundmanager.
	*/
	virtual void BeginPlay() override;

private:
	UProceduralMeshComponent *speedboostMesh; //!< \brief This is the procedural mesh component of the speedboost
	UProceduralMeshComponent *rampMesh; //!< \brief This is the procedural mesh component of the ramp

	UPROPERTY(EditAnywhere)
		UMaterialInterface* speedBoostMaterial; //!< \brief Material of the speedboost, should be assigned in the inspector

	UPROPERTY(EditAnywhere)
		UMaterialInterface* rampMaterial; //!< \brief Material of the ramp, should be assigned in the inspector

	UPROPERTY(EditAnywhere)
		ATempPlayer* player; //!< \brief Pointer to the player in the scene, should be assigned in the inspector

	bool isSpeedUp = false; //!< \brief This bool wil be set to true on overlap to prevent double overlap detection on the same speedboost
	FTimerHandle timehandle; //!< \brief Unreal Engines timerhandle to handle the timer

	UFUNCTION()
	/** \brief Causes the player to slow down over time untill the default speed has been reached once again
	 *	\details   Subtracts a value of the speed untill it reaches the default speed. Sets bool isSpeedUp back to false, so the player
	 *	will regain the ability to speed up.
	*/
		void SpeedUp();

	UPROPERTY(EditAnywhere)
		class ASoundManager* soundManager; //!< \brief Pointer to the soundManager which handles the in game soundeffects, should be assigned in the inspector

	/** \brief <h3>A function to calculate the normal of a point</h3>
	 *	\details   This function calculates and normalized the normal. In this class, it is used to determine the thickness of the speedboost mesh.
	 *	
	 * <h3>Parameters</h3>
	 *	- <b style="color:#80b3ff;">\a FVector firstPoint</b> The point of which the normal is calculated
	 *	- <b style="color:#80b3ff;">\a FVector secondPoint</b> The direction point of the normal - the normalized direction towards from firstPoint to Secondpoint is the final result
	*/
	FVector CalculateNormal(FVector firstPoint, FVector secondPoint);

	/** \brief <h3>Generates the ramp mesh</h3>
	 *	\details   Calculates the vertices, triangles and uvs of the ramp mesh and draws it on the screen.
	 *	
	 * <h3>Parameters</h3>
	 *	-  <b style="color:#80b3ff;">\a int arrowAmount</b> the amount of arrows of the speedboost determines the offset on the position of the ramp on the track
	 *	-  <b style="color:#80b3ff;">\a int position</b> the relative position of the ramp
	 *	-  <b style="color:#80b3ff;">\a TArray<FVector> trackVertices</b> The position of the vertices of the track. Used to determine the coordinates of the ramp.
	 *	-  <b style="color:#80b3ff;">\a TArray<FVector> trackNormals</b> The normals of the track, used to determine the angle of the ramp.
	 *	-  <b style="color:#80b3ff;">\a int trackVertAmount</b> The amount of Vertices on the track, used to determine on which row of the track the ramp will spawn.
	 *	-  <b style="color:#80b3ff;">\a int counter</b> The mesh section index of the track. The mesh section index of the ramp will be set equal to this number.
	 *	-  <b style="color:#80b3ff;">\a FVector trackScale</b> The scale of the Track. The positions of the ramp vertices should be multiplied by this number, to get the right position coordinates.
	 * 
	*/
	void GenerateRamp(int arrowAmount, int position, TArray<FVector> trackVertices, TArray<FVector> trackNormals, int trackVertAmount, int counter, FVector trackScale);

	/** \brief <h3>Calculates a single triangle from three vertices</h3>
	 *	\details  This function calculates a triangle from three vertices and returns this value. It's reusable for different classes. Triangles should be drawn counter-clockwise.
	 *	
	 * <h3>Parameters</h3>
	 *	- <b style="color:#80b3ff;">\a int firstVertex</b> the first vertex of the triangle
	 *	- <b style="color:#80b3ff;">\a int secondVertex</b> the second vertex of the triangle
	 *	- <b style="color:#80b3ff;">\a int thirdVertex</b> the third vertex of the triangle
	 *	- <b style="color:#80b3ff;">\a int vertexAmount</b> A factor that is used by the speedboost to calculate the position of the arrow vertices when there are multiple arrows
	 *	- <b style="color:#80b3ff;">\a int arrowIndex</b> The index of the arrow that is being drawn. Only used for the speedboost.
	 *
	*/
	TArray<int32> CreateTriangle(int firstVertex, int secondVertex, int thirdVertex, int vertexAmount, int arrowIndex);

	/** \brief <h3>Calculates the vertices of the speedboost</h3>
	 *	\details  This function is responsible for calculating the vertices of the speedboost mesh.
	 *
	 * <h3>Parameters</h3>
	 *	- <b style="color:#80b3ff;">\a TArray<FVector> trackVertices</b> The vertices of the track, used to determine the location of the speedboost vertices.
	 *	- <b style="color:#80b3ff;">\a int position</b> The position of the top vertex of the arrow-shaped mesh. From this position the rest of the vertices are calculated.
	 *	- <b style="color:#80b3ff;">\a int i</b> The index of the arrow. Determines which arrow is being drawn.
	 *	- <b style="color:#80b3ff;">\a int trackVertAmount</b> The amount of Vertices on the track, used to determine on which row of the track the ramp will spawn. 
	 *	- <b style="color:#80b3ff;">\a FVector trackScale</b> The scale of the Track. The positions of the ramp vertices should be multiplied by this number, to get the right position coordinates.
	 * - <b style="color:#80b3ff;">\a int arrowVertices</b> The amount of vertices on one arrow. Is used to determine the index of the arrow when there are more arrows than 1.
	 * - <b style="color:#80b3ff;">\a int arrowThickness</b> A factor that determines the thickness of the arrows.
	*/
	TArray<FVector> CalculateSpeedboostVertices(TArray<FVector> trackVertices, int position, int i, int trackVertAmount, FVector trackScale, int arrowVertices, int arrowThickness);

	UFUNCTION()
		/** \brief <h3>Causes the player to speed up</h3>
		 *	\details   Gets called when there is overlap between the player and the speedboost. When there is overlap:
		 *	- The players speed will be multiplied with a factor greater than 1
		 *	- The field of view wil temporarily increase, so the player gets a nice feel of speeding up
		 *	- A sound will be played
		 *
		 *	 Lastly, a timer will start which will cause the player to slow down after x seconds
		 *
		 *	 * <h3>Parameters</h3>
		 *	- <b style="color:#80b3ff;">\a UPrimitiveComponent* ThisComp</b> The component from which the overlap event is detected.
		 *	- <b style="color:#80b3ff;">\a AActor *OtherActor</b> The actor that overlaps with ThisComp.
		 *	- <b style="color:#80b3ff;">\a UPrimitiveComponent* OtherComp</b> The component that overlaps with ThisComp.
		 *	- <b style="color:#80b3ff;">\a int32 OtherBodyIndex</b> The body index of the object that overlaps with ThisComp. 
		 *	- <b style="color:#80b3ff;">\a bool bFromSweep</b> Set to true if it is hit by a sweep function.
		 * - <b style="color:#80b3ff;">\a FHitResult &SweepResult</b> If bFromSweep is true, this contains the data of the sweep.
		*/
		void Notify(class UPrimitiveComponent *ThisComp, AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};