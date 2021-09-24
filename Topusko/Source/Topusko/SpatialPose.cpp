// Advanced Animation Programming Lab2: Ethan Heil, Henry Chronowski


#include "SpatialPose.h"

SpatialPose::SpatialPose()
{
}

SpatialPose::~SpatialPose()
{
}

int SpatialPose::CreateSpatialPose(FMatrix transform, FVector rotate, FVector translate, FVector scale)
{
	mTransform = transform;
	mRotate = rotate;
	mTranslate = translate;
	mScale = scale;

	return 1;
}
