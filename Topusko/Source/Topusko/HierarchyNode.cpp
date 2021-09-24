// Advanced Animation Programming Lab2: Ethan Heil, Henry Chronowski


#include "HierarchyNode.h"

HierarchyNode::HierarchyNode()
{
	mName = FString();
	mIndex = -1;
	mParentIndex = 0;
}

HierarchyNode::~HierarchyNode()
{
}

int HierarchyNode::InitHierarchyNode(FString name, int index, int parentIndex)
{
	mName = name;
	mIndex = index;
	mParentIndex = parentIndex;

	return 1;
}
