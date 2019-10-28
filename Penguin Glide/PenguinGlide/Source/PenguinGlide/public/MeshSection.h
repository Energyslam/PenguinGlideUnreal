#pragma once

#include "CoreMinimal.h"

/*!
 *  \brief     This class is the section out of which the track is built. It holds the curve and points along the curve, ready to be used to create a mesh.
 *  \details   The mesh keeps track of the nodes it has used in order to create its Bezier curve. It splits this bezier curve into an amount of smaller points
 *					so these points can be used to create vertices.
 *  \author    Max Groen
 *  \version   1
 *  \date      05/05/2019
 *  \copyright Energyslam™
 */
class PENGUINGLIDE_API MeshSection
{
public:
	MeshSection(int segmentAmount);

	int nodeAmount = 4; //!< We always use 4 points for our curve.
	int pointSpacer; //!< The amount of points along the curve. Gets set in the constructor
	int offsetY = 600; //!< \uses this value for the random range in generating the Y position for nodes
	TArray<FVector> nodes;
	TArray<FVector> points;

	/** \brief Initializes the first mesh section.
	*
	* The class needs a seperate initialize function as the update function uses the nodes of the previous mesh section.
	* This function only uses the initial nodes to create the first mesh section
	*/
	void InitializeMesh(TArray<FVector>); 

	/** \brief Creates or updates a mesh section
	*
	* The class needs a seperate initialize function as the update function uses the nodes of the previous mesh section.
	* This function only uses the initial nodes to create the first mesh section
	*/
	void UpdateMesh(FVector lastNode, FVector previousNode);
	void setPoints();

};
