// Advanced Animation Programming Lab2: Ethan Heil, Henry Chronowski

#pragma once

#include "CoreMinimal.h"
#include "SpatialPose.h"
#include "Hierarchy.h"

/**
 * 
 */
class TOPUSKO_API HierarchicalPose
{
public:
	HierarchicalPose();
	~HierarchicalPose();

	int InitSpatialPoses(const size_t nodeCount);

public:

	SpatialPose* mSpatialPoses;
};
