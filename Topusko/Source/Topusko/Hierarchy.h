// Advanced Animation Programming Lab2: Ethan Heil, Henry Chronowski

#pragma once

#include "CoreMinimal.h"
#include "HierarchyNode.h"

/**
 * 
 */
class TOPUSKO_API Hierarchy
{
public:
	Hierarchy();
	~Hierarchy();

	int CreateHierarchy(const size_t numNodes, const FString* names);
	int DeleteHierarchy();

	int SetNode(const size_t index, const int parentIndex, const FString name);

public:

	HierarchyNode* mHierarchyNodes;
	size_t mNodeCount;
};
