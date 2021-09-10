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
	
	a3_KeyframeAnimationController.c
	Implementation of keyframe animation controller.
*/

#include "../a3_KeyframeAnimationController.h"

#include <string.h>


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	// Set name
	for (a3ui32 i = 0; i < a3keyframeAnimation_nameLenMax; i++)
	{
		clipCtrl_out->name[i] = ctrlName[i];
	}

	// Set reference to clip pool
	clipCtrl_out->clipPool = clipPool;

	// Set index of current clip
	clipCtrl_out->clip = clipIndex_pool;

	// Set playback direction
	clipCtrl_out->playbackDirection = 1;

	// Set current clip
	clipCtrl_out->currentClip = &clipPool->clip[clipIndex_pool];

	// Set current keyframe
	clipCtrl_out->keyframe = clipPool->clip[clipIndex_pool].firstKeyframe;
	clipCtrl_out->currentKeyframe = &clipCtrl_out->currentClip->keyframePool->keyframe[clipPool->clip->firstKeyframe];

	clipCtrl_out->timeMultiplier = 1.0f;

	return clipCtrl_out->playbackDirection;
}


void a3clipControllerSetKeyframe(a3_ClipController* clipCtrl, const a3ui32 keyframeIndex)
{
	// Validate index
	if (keyframeIndex > 0 && keyframeIndex < clipCtrl->currentClip->keyframePool->count)
	{
		// Set index based on keyframeIndex
		clipCtrl->keyframe = keyframeIndex;
		clipCtrl->currentKeyframe = &clipCtrl->currentClip->keyframePool->keyframe[keyframeIndex];
		clipCtrl->keyframeTime = 0.0f;
	}
}

//-----------------------------------------------------------------------------
