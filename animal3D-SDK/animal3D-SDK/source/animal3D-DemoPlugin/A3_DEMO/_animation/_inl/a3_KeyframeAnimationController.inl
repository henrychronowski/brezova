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
		return 0;
		break;
	// Forwards
	case 1:
		// Pre resolution
		clipCtrl->clipTime += dt * clipCtrl->timeMultiplier;
		clipCtrl->keyframeTime += dt * clipCtrl->timeMultiplier;
		a3i32 lastKeyframeIndex = clipCtrl->currentClip->lastKeyframe;

		// Check for overflow resolution
		{
			a3f32 curClipDuration = clipCtrl->currentClip->duration;
			a3f32 curKeyframeDuration = clipCtrl->currentKeyframe0->duration;

			// Check if we exceeded length of the clip
			if (clipCtrl->clipTime >= curClipDuration)
			{
				// Set clip and play direction based on transition parameters
				clipCtrl->playbackDirection = clipCtrl->currentClip->forwardTransition.transition;
				clipCtrl->clip = clipCtrl->currentClip->forwardTransition.destinationIndex;
				clipCtrl->currentClip = &clipCtrl->clipPool->clip[clipCtrl->clip];

				// Reset clip time including overflow
				clipCtrl->clipTime = clipCtrl->clipTime - curClipDuration;
			}

			// Check if we exceeded the current keyframe
			if (clipCtrl->keyframeTime >= curKeyframeDuration)
			{
				// Check if we are on the last keyframe
				if (clipCtrl->currentKeyframe0->index == lastKeyframeIndex)
				{
					// Loop back to first keyframe
					if (clipCtrl->playbackDirection == -1)
					{
						clipCtrl->keyframe0 = clipCtrl->currentClip->lastKeyframe;
						clipCtrl->keyframe1 = clipCtrl->keyframe0 - 1;
					}
					else 
					{
						clipCtrl->keyframe0 = clipCtrl->currentClip->firstKeyframe;
						clipCtrl->keyframe1 = clipCtrl->keyframe0 + 1;
					}
					
					clipCtrl->currentKeyframe0 = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe0];
					clipCtrl->currentKeyframe1 = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe1];
				}
				else
				{
					// Increment keyframe index
					clipCtrl->keyframe0++;
					clipCtrl->keyframe1 = (clipCtrl->keyframe0 + 1) % clipCtrl->currentClip->keyframeCount + clipCtrl->currentClip->firstKeyframe;

					// Set new keyframe
					clipCtrl->currentKeyframe0 = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe0];
					clipCtrl->currentKeyframe0->index = clipCtrl->keyframe0;
					clipCtrl->currentKeyframe1 = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe1];
					clipCtrl->currentKeyframe1->index = clipCtrl->keyframe1;
				}

				// Reset keyframe time including overflow
				clipCtrl->keyframeTime = clipCtrl->keyframeTime - curKeyframeDuration;
			}
		}

		break;
	// Reverse
	case -1:
		// Pre resolution
		clipCtrl->clipTime -= dt * clipCtrl->timeMultiplier;
		clipCtrl->keyframeTime -= dt * clipCtrl->timeMultiplier;
		a3i32 firstKeyframeIndex = clipCtrl->currentClip->firstKeyframe;

		// Check for overflow resolution
		{
			//a3f32 curClipDuration = clipCtrl->currentClip->duration;
			//a3f32 curKeyframeDuration = clipCtrl->currentKeyframe0->duration;

			// Check if we pass the start of the clip
			if (clipCtrl->clipTime < 0)
			{
				// Set clip and play direction based on transition parameters
				clipCtrl->playbackDirection = clipCtrl->currentClip->reverseTransition.transition;
				clipCtrl->clip = clipCtrl->currentClip->reverseTransition.destinationIndex;
				clipCtrl->currentClip = &clipCtrl->clipPool->clip[clipCtrl->clip];
				
				// Reset clip time including overflow
				clipCtrl->clipTime = clipCtrl->clipTime + clipCtrl->currentClip->duration;//curClipDuration;
			}
			// Check if we pass the start of the current keyframe
			if (clipCtrl->keyframeTime < 0)
			{
				// Check if we are on the first keyframe
				if (clipCtrl->currentKeyframe0->index == firstKeyframeIndex)
				{
					// Loop back to last keyframe in the clip
					if (clipCtrl->playbackDirection == 1)
					{
						clipCtrl->keyframe0 = clipCtrl->currentClip->firstKeyframe;
						clipCtrl->keyframe1 = clipCtrl->keyframe0 + 1;
					}
					else 
					{
						clipCtrl->keyframe0 = clipCtrl->currentClip->lastKeyframe;
						clipCtrl->keyframe1 = clipCtrl->keyframe0 - 1;
					}

					clipCtrl->currentKeyframe0 = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe0];
					clipCtrl->currentKeyframe1 = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe1];
				}
				else
				{
					// Decrement keyframe index
					clipCtrl->keyframe0--;
					clipCtrl->keyframe1 = (clipCtrl->keyframe0 - 1) % clipCtrl->currentClip->keyframeCount + clipCtrl->currentClip->firstKeyframe;

					// Set new keyframe
					clipCtrl->currentKeyframe0 = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe0];
					clipCtrl->currentKeyframe1 = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe1];
				}

				// Reset keyframe time including overflow
				clipCtrl->keyframeTime = clipCtrl->keyframeTime + clipCtrl->currentKeyframe0->duration;//curKeyframeDuration;
			}
		}

		break;
	default:
		break;
	}

	// Post Resolution (Normalize)
	clipCtrl->clipParameter = clipCtrl->clipTime * clipCtrl->currentClip->durationInverse;
	clipCtrl->keyframeParameter = clipCtrl->keyframeTime * clipCtrl->currentKeyframe0->durationInverse;
	
	return 1;
}

// set clip to play
inline a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	// Set name
	for (a3ui32 i = 0; i < a3keyframeAnimation_nameLenMax; i++)
	{
		//clipCtrl->name[i] = clipPool[clipIndex_pool].clip->name[i];
	}

	// Set reference to clip pool
	clipCtrl->clipPool = clipPool;

	// Set index of current clip
	clipCtrl->clip = clipIndex_pool;

	// Set playback direction
	clipCtrl->playbackDirection = 1;

	// Set current clip
	clipCtrl->currentClip = &clipPool->clip[clipIndex_pool];

	// Set current keyframe
	clipCtrl->keyframe0 = clipPool->clip[clipIndex_pool].firstKeyframe;
	clipCtrl->currentKeyframe0 = &clipCtrl->currentClip->keyframePool->keyframe[clipPool->clip->firstKeyframe];
	clipCtrl->keyframe1 = (clipCtrl->keyframe0 + 1) % clipCtrl->currentClip->keyframeCount + clipCtrl->currentClip->firstKeyframe;
	clipCtrl->currentKeyframe1 = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe1];

	// Reset times
	clipCtrl->clipTime = 0;
	clipCtrl->keyframeTime = 0;
	
	return 1;
}

// Evaluate the current value at time
inline a3i32 a3clipControllerEvaluate(a3_ClipController const* clipCtrl, a3_Sample* sample_out)
{
	if (clipCtrl && clipCtrl->currentClip && sample_out)
	{
		// 0: no interpolation: step function
		*sample_out = clipCtrl->currentKeyframe0->sample;

		// 1: nearest
		// if (param < 0.5) then keyframe0, else keyframe 1

		// 2: lerp
		// k = k0 + (k1 - k0)param
		sample_out->time = clipCtrl->keyframeTime;
		const a3_Keyframe* k0 = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe0];
		const a3_Keyframe* k1 = &clipCtrl->currentClip->keyframePool->keyframe[clipCtrl->keyframe1];

		switch (clipCtrl->playbackDirection)
		{
		// Lerp forwards
		case(-1):
			sample_out->position.x = a3lerp(
				k1->sample.position.x,
				k0->sample.position.x,
				clipCtrl->keyframeParameter
			);
			sample_out->position.y = a3lerp(
				k1->sample.position.y,
				k0->sample.position.y,
				clipCtrl->keyframeParameter
			);
			sample_out->position.z = a3lerp(
				k1->sample.position.z,
				k0->sample.position.z,
				clipCtrl->keyframeParameter
			);
			break;
		// Lerp reverse
		case(1):
			sample_out->position.x = a3lerp(
				k0->sample.position.x,
				k1->sample.position.x,
				clipCtrl->keyframeParameter
			);
			sample_out->position.y = a3lerp(
				k0->sample.position.y,
				k1->sample.position.y,
				clipCtrl->keyframeParameter
			);
			sample_out->position.z = a3lerp(
				k0->sample.position.z,
				k1->sample.position.z,
				clipCtrl->keyframeParameter
			);
			break;
		default:
			break;
		}
		

		// 3: spline (catmull-rom, cubic hermite, etc)


		return clipCtrl->keyframe0;
	}

	return -1;
}

//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H