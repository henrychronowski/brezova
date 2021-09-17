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
	
	a3_KeyframeAnimation.inl
	Inline definitions for keyframe animation.
*/

#ifdef __ANIMAL3D_KEYFRAMEANIMATION_H
#ifndef __ANIMAL3D_KEYFRAMEANIMATION_INL
#define __ANIMAL3D_KEYFRAMEANIMATION_INL


//-----------------------------------------------------------------------------

// calculate clip duration as sum of keyframes' durations
inline a3i32 a3clipCalculateDuration(a3_Clip* clip)
{
	a3f32 clipDuration = 0;

	// Add duration of all keyframes in referenced keyframe pool
	for (a3ui32 i = clip->firstKeyframe; i <= clip->lastKeyframe; i++)
	{
		clipDuration += clip->keyframePool->keyframe[i].duration;
	}

	// Set clip duration
	clip->duration = clipDuration;

	// Set inverse duration
	clip->durationInverse = 1.0f / clipDuration;

	return 1;
}

// calculate keyframes' durations by distributing clip's duration
inline a3i32 a3clipDistributeDuration(a3_Clip* clip, const a3real newClipDuration)
{
	// Calculate new duration for each keyframe
	a3f32 newDuration = newClipDuration / clip->keyframeCount;

	// Set new duration and inverse duration for each keyframe
	for (a3ui32 i = 0; i < clip->keyframeCount; i++)
	{
		clip->keyframePool->keyframe[i].duration = newDuration;
		clip->keyframePool->keyframe[i].durationInverse = 1.0f / newDuration;
	}

	// Set new duration and inverse duration for clip
	clip->duration = newClipDuration;
	clip->durationInverse = 1.0f / newClipDuration;

	return 1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATION_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATION_H