// Fill out your copyright notice in the Description page of Project Settings.

#include "MeshSection.h"

#include <vector>
#include <iostream>

MeshSection::MeshSection(int segmentAmount)
{
	pointSpacer = segmentAmount;
	points.Init(FVector::ZeroVector, pointSpacer);
	nodes.Init(FVector::ZeroVector, nodeAmount);
}

/** This function is used for the initial mesh. It has to be created using this method as it adds to the readability of the code and makes it clear this function
 *initializes the first mesh. This is only called at the start of the program as no previous sections exist yet. 
 *
	*/
void MeshSection::InitializeMesh(TArray<FVector> newNodes)
{
	for (int i = 0; i < nodeAmount; i++) 
	{
		nodes[i] = newNodes[i]; //!< nodes are assigned here. You can set these in the editor if you want to use different ones than the initially assigned ones
	}

	setPoints();
}

/** This function takes care of changes the nodes of the mesh and creating new points. These newly created points are used in the TrackGeneratorBezier
 *to create a new mesh, re-using points of previous nodes to make sure the meshes fit on each other in the same direction
*/
void MeshSection::UpdateMesh(FVector lastNode, FVector previousNode)
{

	nodes[0] = FVector(previousNode.X + (lastNode.X - previousNode.X), previousNode.Y, 0.0f);
	nodes[1] = FVector(lastNode.X + (lastNode.X - previousNode.X), lastNode.Y, 0.0f);
	nodes[2] = FVector(lastNode.X + ((lastNode.X - previousNode.X) * 2),
				FMath::RandRange((int)lastNode.Y - offsetY, (int)lastNode.Y + offsetY),
				0);
	nodes[3] = FVector(lastNode.X + ((lastNode.X - previousNode.X) * 3),
				FMath::RandRange((int)lastNode.Y - offsetY, (int)lastNode.Y + offsetY),
				0);

	/**
	 *	This loop splits the bezier curve into smaller points. It splits it in the amount that pointSpacer has been given. If this is not done
	 *	the mesh will only exist out of its four initial nodes which only creates four huge blocks as track parts. Not pretty!
	 */
	setPoints();

	/**
	 *	Each points needs an offset in the Y direction to make sure it aligns with the previous mesh. If this is not done, it will create a part of the mesh over the old mesh
	 */
	for (int i = 0; i < pointSpacer; i++) {
		points[i].Y += lastNode.Y - previousNode.Y;
	}

	for (int i = 0; i < nodeAmount; i++) 
	{
		nodes[i].Y += lastNode.Y - previousNode.Y;
	}
}

void MeshSection::setPoints() {
	for (int i = 0; i < pointSpacer; i++)
	{
		points[i] = FMath::Lerp(
			FMath::Lerp(
				FMath::Lerp(nodes[0], nodes[1], (float)i / (pointSpacer - 1)),
				FMath::Lerp(nodes[1], nodes[2], (float)i / (pointSpacer - 1)),
				(float)i / (pointSpacer - 1)),
			FMath::Lerp(
				FMath::Lerp(nodes[1], nodes[2], (float)i / (pointSpacer - 1)),
				FMath::Lerp(nodes[2], nodes[3], (float)i / (pointSpacer - 1)),
				(float)i / (pointSpacer - 1)),
				(float)i / (pointSpacer - 1));
	}
}