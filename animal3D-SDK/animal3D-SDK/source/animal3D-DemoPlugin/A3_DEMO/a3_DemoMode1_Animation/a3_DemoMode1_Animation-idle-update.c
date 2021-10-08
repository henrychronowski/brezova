/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_DemoMode1_Animation-idle-update.c
	Demo mode implementations: animation scene.

	********************************************
	*** UPDATE FOR ANIMATION SCENE MODE      ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode1_Animation.h"
#include "A3_DEMO/_animation/a3_Kinematics.h"
//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"


#include <stdio.h>


//-----------------------------------------------------------------------------
// UPDATE

void a3demo_update_objects(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3boolean useZYX, a3boolean applyScale);
void a3demo_update_defaultAnimation(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3ui32 axis);
void a3demo_update_bindSkybox(a3_DemoSceneObject* obj_camera, a3_DemoSceneObject* obj_skybox);
void a3demo_update_pointLight(a3_DemoSceneObject* obj_camera, a3_DemoPointLight* pointLightBase, a3ui32 count);

void a3demo_applyScale_internal(a3_DemoSceneObject* sceneObject, a3real4x4p s);

void a3animation_update(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt)
{
	a3ui32 i;
	a3_DemoModelMatrixStack matrixStack[animationMaxCount_sceneObject];

	// active camera
	a3_DemoProjector const* activeCamera = demoMode->projector + demoMode->activeCamera;
	a3_DemoSceneObject const* activeCameraObject = activeCamera->sceneObject;

	// temp scale mat
	a3mat4 scaleMat = a3mat4_identity;

	a3demo_update_objects(demoState, dt,
		demoMode->object_scene, animationMaxCount_sceneObject, 0, 0);
	a3demo_update_objects(demoState, dt,
		demoMode->object_camera, animationMaxCount_cameraObject, 1, 0);

	a3demo_updateProjectorViewProjectionMat(demoMode->proj_camera_main);

	// apply scales to objects
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
		a3demo_applyScale_internal(demoMode->object_scene + i, scaleMat.m);

	// update skybox
	a3demo_update_bindSkybox(demoMode->obj_camera_main, demoMode->obj_skybox);

	// update matrix stack data
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
	{
		a3demo_updateModelMatrixStack(matrixStack + i,
			activeCamera->projectionMat.m, activeCameraObject->modelMat.m, activeCameraObject->modelMatInv.m,
			demoMode->object_scene[i].modelMat.m, a3mat4_identity.m);
	}

	// ****TODO:
	// Resolve animation: "the 4 steps"
	// inteprolate
	// concatenate (with base)
	// convert
	// FK

	a3hierarchyPoseCopy(&demoMode->hierarchyState_skel->localSpacePose, demoMode->hierarchyPoseGroup_skel->hierarchyPoses, demoMode->hierarchyState_skel->hierarchy->numNodes);
	a3hierarchyPoseConvert(&demoMode->hierarchyState_skel->localSpacePose, demoMode->hierarchyState_skel->hierarchy->numNodes, 0, a3poseEulerOrder_xyz);
	a3kinematicsSolveForward(demoMode->hierarchyState_skel);


	// upload results of FK to uniform buffers UBO
	a3mat4 skeletonMVP;
	a3real4x4Product(skeletonMVP.m, activeCamera->viewProjectionMat.m, demoMode->obj_skeleton->modelMat.m);

	a3_SpatialPose* pose = demoMode->hierarchyState_skel->objectSpacePose.spatialPose;

	a3i32 parentIndex;
	a3real3rk up;

	for (a3ui32 i = 0; i < demoMode->hierarchyState_skel->hierarchy->numNodes; i++)
	{
		a3mat4* bone = demoMode->skeletonPose_transformLMVP_bone;
		a3mat4* joint = demoMode->skeletonPose_transformLMVP_joint;
		parentIndex = demoMode->hierarchyState_skel->hierarchy->nodes[i].parentIndex;
		
		if (parentIndex != -1)
		{
			
			bone[i] = a3mat4_identity;

			// Get the difference in position from current parent to current joint
			a3real3Diff(bone[i].m[2], pose[i].transform.m[3], pose[parentIndex].transform.m[3]);

			if (bone[i].m20 * bone[i].m21 == 0)
			{
				up = a3vec3_z.v;
			}
			else
			{
				up = a3vec3_y.v;
			}

			a3real3MulS(a3real3CrossUnit(bone[i].m[0], up, bone[i].m[2]), 0.25f);
			a3real3MulS(a3real3CrossUnit(bone[i].m[1], bone[i].m[2], bone[i].m[0]), 0.25f);

			bone[i].v3 = pose[parentIndex].transform.v3;
		}
		else
		{
			//demoMode->skeletonPose_transformLMVP_joint[i] = pose->transform;
			a3real4x4SetScale(bone[i].m, 0.0f);
			bone[i].v3 = pose[i].transform.v3;
		}
		// Apply the MVP for the skeleton
		a3real4x4Concat(skeletonMVP.m, bone[i].m);

		a3real4x4SetScale(joint[i].m, 0.25f);	

		a3real4x4Concat(pose[i].transform.m, joint[i].m);
		a3real4x4Concat(skeletonMVP.m, joint[i].m);
	}

	a3bufferRefillOffset(demoState->ubo_transformLMVP_bone, 0, 0, sizeof(demoMode->skeletonPose_transformLMVP_bone), demoMode->skeletonPose_transformLMVP_bone);
	a3bufferRefillOffset(demoState->ubo_transformLMVP_joint, 0, 0, sizeof(demoMode->skeletonPose_transformLMVP_joint), demoMode->skeletonPose_transformLMVP_joint);
}


//-----------------------------------------------------------------------------
