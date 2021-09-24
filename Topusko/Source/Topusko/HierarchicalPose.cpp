// Advanced Animation Programming Lab2: Ethan Heil, Henry Chronowski


#include "HierarchicalPose.h"

HierarchicalPose::HierarchicalPose()
{
}

HierarchicalPose::~HierarchicalPose()
{
}

int HierarchicalPose::InitSpatialPoses(const size_t nodeCount)
{
	if (mSpatialPoses && nodeCount)
	{
		for (size_t i = 0; i < nodeCount; i++)
		{
			mSpatialPoses[i].CreateSpatialPose();
		}

		return 1;
	}

	return -1;
}
