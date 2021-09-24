// Advanced Animation Programming Lab2: Ethan Heil, Henry Chronowski


#include "Hierarchy.h"

Hierarchy::Hierarchy()
{
}

Hierarchy::~Hierarchy()
{
	DeleteHierarchy();
}

int Hierarchy::CreateHierarchy(const size_t numNodes, const FString* names)
{
	if (!mHierarchyNodes)
	{
		const size_t dataSize = sizeof(HierarchyNode) * numNodes;

		mHierarchyNodes = (HierarchyNode*)malloc(dataSize);
		memset(mHierarchyNodes, 0, dataSize);

		mNodeCount = numNodes;

		for (int i = 0; i < numNodes; ++i)
		{
			mHierarchyNodes[i].mName = names[i];
		}

		return mNodeCount;
	}
	
	return -1;
}

int Hierarchy::DeleteHierarchy()
{
	if (mHierarchyNodes)
	{
		free(mHierarchyNodes);
		mHierarchyNodes = 0;
		mNodeCount = 0;

		return 1;
	}

	return -1;
}
