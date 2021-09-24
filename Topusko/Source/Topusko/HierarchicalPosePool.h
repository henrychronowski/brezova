// Advanced Animation Programming Lab2: Ethan Heil, Henry Chronowski

#pragma once

#include "CoreMinimal.h"
#include "HierarchicalPose.h"

/**
 * 
 */
class TOPUSKO_API HierarchicalPosePool
{
public:
	HierarchicalPosePool();
	~HierarchicalPosePool();

	int CreateHierarchicalPosePool(const Hierarchy* hierarchy, const size_t poseCount);
	int DeleteHierarchicalPosePool();

public:
	
	const Hierarchy* mHierarchy;
	SpatialPose* mSpatialPosePool;
	HierarchicalPose* mHierarchicalPoses;

	size_t mHierarchicalPoseCount;
	size_t mSpatialPoseCount;
};
