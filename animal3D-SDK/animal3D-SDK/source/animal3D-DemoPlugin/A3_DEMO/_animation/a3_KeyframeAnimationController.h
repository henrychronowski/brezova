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
	
	a3_KeyframeAnimationController.h
	Keyframe animation clip controller. Basically a frame index manager. Very 
	limited in what one can do with this; could potentially be so much more.
*/

#ifndef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H
#define __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H


#include "a3_KeyframeAnimation.h"
#include "a3_Hierarchy.h"
#include "a3_HierarchyState.h"
#include "a3_HierarchyStateBlend.h"
#include "a3_Kinematics.h"
#include <stdlib.h>
//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_ClipController			a3_ClipController;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// clip controller
// metaphor: playhead
struct a3_ClipController
{
	a3byte name[a3keyframeAnimation_nameLenMax];

	// index of clip in pool and keyframe in clip
	a3ui32 clipIndex, keyframeIndex;

	// clip time, keyframe time and speed in steps
	a3i32 clipTime_step, keyframeTime_step, playback_step;

	// clip time, keyframe time and speed in seconds
	a3f64 clipTime_sec, keyframeTime_sec, playback_sec, playback_stepPerSec, playback_secPerStep;

	// clip and keyframe interpolation parameters
	a3f64 clipParam, keyframeParam;

	a3real currentSpeed;
	a3vec3 currentDirection;
	a3vec3 currentTarget;
	a3ui32 currentState;

	// clip pool and pointers
	a3_ClipPool const* clipPool;
	a3_Clip const* clip;
	a3_Keyframe const* keyframe;
};


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool, const a3i32 playback_step, const a3f64 playback_stepPerSec);

// update clip controller
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, a3f64 dt);

// set playback state
a3i32 a3clipControllerSetPlayback(a3_ClipController* clipCtrl, const a3i32 playback_step, const a3f64 playback_stepPerSec);

// set clip to play
a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool, const a3i32 playback_step, const a3f64 playback_stepPerSec);

a3i32 a3clipControllerBlendClip(a3_ClipController* clipCtrl, a3_HierarchyState* active_HS, const a3_HierarchyPoseGroup* active_PoseGroup, a3_ClipPool* clipPool, a3_Clip* clip0, a3_Clip* clip1);

void a3clipControllerUpdateMovement(a3_ClipController* clipCtrl, a3_AIController* aiCtrl);

a3vec3 a3clipControllerGetTargetLookaAt(a3_AIController* aiCtrl, a3vec3 targetPos);

a3i32 a3ClipControllerUpdateTarget(a3_ClipController* clipCtrl, a3vec3 newTarget);

a3i32 a3clipControllerBranchTransition(a3_ClipController* clipCtrl, const a3_Clip* clipA, a3_Clip* clipB, a3real param, a3real goal);
a3_HierarchyPose* a3clipControllerBranchTransitionBlend(a3_ClipController* clipCtrlA, a3_ClipController* clipCtrlB, a3_HierarchyState* active_HS, const a3_HierarchyPoseGroup * active_PoseGroup, a3real blendParam);
//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_KeyframeAnimationController.inl"


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H