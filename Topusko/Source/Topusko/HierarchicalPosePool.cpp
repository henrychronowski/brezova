// Advanced Animation Programming Lab2: Ethan Heil, Henry Chronowski


#include "HierarchicalPosePool.h"

HierarchicalPosePool::HierarchicalPosePool()
{
}

HierarchicalPosePool::~HierarchicalPosePool()
{
}

int HierarchicalPosePool::CreateHierarchicalPosePool(const Hierarchy* hierarchy, const size_t poseCount)
{
	if (hierarchy && poseCount)
	{
		size_t dataSize = sizeof(HierarchicalPose) * poseCount;

		mHierarchicalPoses = (HierarchicalPose*)malloc(dataSize);
		memset(mHierarchicalPoses, 0, dataSize);

		mHierarchicalPoseCount = poseCount;

		dataSize = sizeof(SpatialPose) * (poseCount * hierarchy->mNodeCount);
		memset(mSpatialPosePool, 0, dataSize);

		mSpatialPoseCount = poseCount * hierarchy->mNodeCount;

		mHierarchy = hierarchy;

		for (int j = 0; j < mSpatialPoseCount; j++)
		{
			mSpatialPosePool[j].CreateSpatialPose();
		}

		mHierarchicalPoses->InitSpatialPoses(mHierarchicalPoseCount);

		return 1;
	}

	return -1;
}

int HierarchicalPosePool::DeleteHierarchicalPosePool()
{
	if (mHierarchy)
	{
		free(mHierarchicalPoses);
		free(mSpatialPosePool);
		free((void*)mHierarchy);

		mHierarchy = nullptr;

		return 1;
	}

	return -1;
}
