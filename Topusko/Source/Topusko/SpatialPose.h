// Advanced Animation Programming Lab2: Ethan Heil, Henry Chronowski

#pragma once

#include "CoreMinimal.h"
#include "Math/Matrix.h"

/**
 * 
 */
class TOPUSKO_API SpatialPose
{
public:
	SpatialPose();
	~SpatialPose();

	int CreateSpatialPose(FMatrix transform = FMatrix::Identity, FVector rotate = FVector::ZeroVector, FVector translate = FVector::ZeroVector, FVector scale = FVector::OneVector);

public:
	
	FMatrix mTransform;
	FVector mRotate, mTranslate, mScale;
};
