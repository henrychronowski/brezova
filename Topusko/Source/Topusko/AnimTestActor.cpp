// Advanced Animation Programming Lab2: Ethan Heil, Henry Chronowski


#include "AnimTestActor.h"

// Sets default values
AAnimTestActor::AAnimTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void AAnimTestActor::BeginPlay()
{
	Super::BeginPlay();

	const size_t jointCount = 32;

	size_t jointIndex = 0;
	int jointParentIndex = -1;
	int rootJointIndex, upperSpineJointIndex, clavicleJointIndex, pelvisJointIndex;

	skeletonHierarchy = Hierarchy();
	skeletonHierarchy.CreateHierarchy(jointCount, 0);

	jointParentIndex = rootJointIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:root");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:spine_lower");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:spine_mid");
	jointParentIndex = upperSpineJointIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:spine_upper");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:neck");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:head");
	jointParentIndex = upperSpineJointIndex;
	jointParentIndex = clavicleJointIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:clavicle");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:shoulderblade_r");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:shoulder_r");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:elbow_r");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:forearm_r");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:wrist_r");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:hand_r");
	jointParentIndex = clavicleJointIndex;
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:shoulderblade_l");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:shoulder_l");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:elbow_l");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:forearm_l");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:wrist_l");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:hand_l");
	jointParentIndex = rootJointIndex;
	jointParentIndex = pelvisJointIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:pelvis");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:hip_r");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:knee_r");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:shin_r");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:ankle_r");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:foot_r");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:toe_r");
	jointParentIndex = pelvisJointIndex;
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:hip_l");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:knee_l");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:shin_l");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:ankle_l");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:foot_l");
	jointParentIndex = skeletonHierarchy.SetNode(jointIndex++, jointParentIndex, "skel:toe_l");

	hierarchyPosePool = HierarchicalPosePool();
	hierarchyPosePool.mHierarchy = 0;
	hierarchyPosePool.CreateHierarchicalPosePool(&skeletonHierarchy, 4);
	
}

// Called every frame
void AAnimTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

