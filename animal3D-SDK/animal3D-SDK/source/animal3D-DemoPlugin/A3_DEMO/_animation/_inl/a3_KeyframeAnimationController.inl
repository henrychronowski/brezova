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
	inline definitions for keyframe animation controller.
*/

#ifdef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H
#ifndef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#define __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL


//-----------------------------------------------------------------------------

// update clip controller
inline a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, const a3real dt)
{	
	switch (clipCtrl->playbackDirection)
	{
	// Paused
	case 0:
		break;
	// Forwards
	case 1:
		// Pre resolution
		clipCtrl->clipTime += dt;
		clipCtrl->keyframeTime += dt;

		// Check for overflow resolution
		{
			a3f32 curClipDuration = clipCtrl->currentClip->duration;
			a3f32 curKeyframeDuration = clipCtrl->currentKeyframe->duration;

			// Check if we exceeded length of the clip
			if (clipCtrl->clipTime >= curClipDuration)
			{
				// Reset clip time including overflow
				clipCtrl->clipTime = clipCtrl->clipTime - curClipDuration;
			}

			if (clipCtrl->keyframeTime >= curKeyframeDuration)
			{
				if (clipCtrl->currentKeyframe->index == clipCtrl->currentClip->lastKeyframe)
				{
					// Loop back to first keyframe
					clipCtrl->keyframe = clipCtrl->currentClip->firstKeyframe;
					clipCtrl->currentKeyframe = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe];
				}
				else
				{
					// Increment keyframe index
					clipCtrl->keyframe++;

					// Set new keyframe
					clipCtrl->currentKeyframe = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe];
				}

				// Reset keyframe time including overflow
				clipCtrl->keyframeTime = clipCtrl->keyframeTime - curKeyframeDuration;
			}
		}

		break;
	// Reverse
	case -1:
		break;
	default:
		break;
	}
	
	return -1;
}

// set clip to play
inline a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	// Set reference to current clip
	clipCtrl->currentClip = &clipPool->clip[clipIndex_pool];
	
	// Update clip index
	clipCtrl->clip = clipIndex_pool;
	
	return 1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H