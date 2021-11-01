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
	
	a3_KeyframeAnimation.c
	Implementation of keyframe animation interfaces.
*/

#include "../a3_KeyframeAnimation.h"

#include <stdlib.h>
#include <string.h>


// macros to help with names
#define A3_CLIP_DEFAULTNAME		("unnamed clip")
#define A3_CLIP_SEARCHNAME		((clipName && *clipName) ? clipName : A3_CLIP_DEFAULTNAME)


//-----------------------------------------------------------------------------

// allocate keyframe pool
a3i32 a3keyframePoolCreate(a3_KeyframePool* keyframePool_out, const a3ui32 count)
{
	// Init array of Keyframes
	keyframePool_out->keyframe = malloc(count * sizeof(a3_Keyframe));

	for (a3ui32 i = 0; i < count; i++)
	{
		keyframePool_out->keyframe->data = 0;
		keyframePool_out->keyframe->duration = 0;
		keyframePool_out->keyframe->durationInverse = 0;
		keyframePool_out->keyframe->index = 0;
	}

	keyframePool_out->count = count;

	return 1;
}

// release keyframe pool
a3i32 a3keyframePoolRelease(a3_KeyframePool* keyframePool)
{
	// Deallocate Keyframe array
	free(keyframePool->keyframe);
	
	return 1;
}

// initialize keyframe
a3i32 a3keyframeInit(a3_Keyframe* keyframe_out, const a3real duration, const a3ui32 value_x)
{
	// Set duration & inverse duration
	keyframe_out->duration = duration;
	keyframe_out->durationInverse = 1.0f / duration;

	// Set keyframe data
	keyframe_out->data = value_x;

	return 1;
}


// allocate clip pool
a3i32 a3clipPoolCreate(a3_ClipPool* clipPool_out, const a3ui32 count)
{
	// Init array of Clips
	clipPool_out->clip = malloc(count * sizeof(a3_Clip));

	for (a3ui32 i = 0; i < count; i++)
	{
		clipPool_out->clip->duration = 0;
		clipPool_out->clip->durationInverse = 0;
		clipPool_out->clip->firstKeyframe = 0;
		clipPool_out->clip->lastKeyframe = 0;
		clipPool_out->clip->index = 0;
		clipPool_out->clip->keyframeCount = 0;
	}

	clipPool_out->count = count;

	return 1;
}

// release clip pool
a3i32 a3clipPoolRelease(a3_ClipPool* clipPool)
{
	// Deallocate Keyframe array
	free(clipPool->clip);

	return 1;
}

// initialize clip with first and last indices
a3i32 a3clipInit(a3_Clip* clip_out, const a3byte clipName[a3keyframeAnimation_nameLenMax], const a3_KeyframePool* keyframePool, const a3ui32 firstKeyframeIndex, const a3ui32 finalKeyframeIndex)
{
	// Set clip name
	for (a3ui32 i = 0; i < a3keyframeAnimation_nameLenMax; i++)
	{
		clip_out->name[i] = clipName[i];
	}

	// Set keyframe pool referenced by clip
	clip_out->keyframePool = keyframePool;

	// Set first and last keyframe indices
	clip_out->firstKeyframe = firstKeyframeIndex;
	clip_out->lastKeyframe = finalKeyframeIndex;

	// Calculate Count
	clip_out->keyframeCount = finalKeyframeIndex - firstKeyframeIndex + 1;

	// Calcualte clip duration
	a3clipCalculateDuration(clip_out);

	return 1;
}

// get clip index from pool
a3i32 a3clipGetIndexInPool(const a3_ClipPool* clipPool, const a3byte clipName[a3keyframeAnimation_nameLenMax])
{
	for (a3ui32 i = 0; i < a3keyframeAnimation_nameLenMax; i++)
	{
		if (clipPool->clip[i].name == clipName)
			return i;
	}

	return 1;
}


//-----------------------------------------------------------------------------
