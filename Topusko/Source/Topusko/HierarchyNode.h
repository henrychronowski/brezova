// Advanced Animation Programming Lab2: Ethan Heil, Henry Chronowski

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TOPUSKO_API HierarchyNode
{
public:
	HierarchyNode();
	~HierarchyNode();

	int InitHierarchyNode(FString name, int index, int parentIndex);

public: 
	FString mName;
	int mIndex;
	int mParentIndex;
};
