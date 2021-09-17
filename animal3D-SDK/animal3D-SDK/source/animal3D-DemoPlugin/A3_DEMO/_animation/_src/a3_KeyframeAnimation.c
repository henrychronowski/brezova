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
#include <stdio.h>


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
		keyframePool_out->keyframe->sample.value = 0;
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
a3i32 a3keyframeInit(a3_Keyframe* keyframe_out, const a3real duration, const a3ui32 value_x, const a3vec3 pos)
{
	// Set duration & inverse duration
	keyframe_out->duration = duration;
	keyframe_out->durationInverse = 1.0f / duration;

	// Set keyframe data
	keyframe_out->sample.value = (a3real)value_x;
	keyframe_out->sample.position = pos;

	return 1;
}

a3i32 a3clipTransitionInit(a3_ClipTransition* transition_out, a3i32 transition, a3_ClipPool* clipPool, a3i32 nextClip)
{
	transition_out->transition = transition;
	transition_out->clipPool = clipPool;
	transition_out->destinationIndex = nextClip;

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
a3i32 a3clipInit(a3_Clip* clip_out, const a3byte clipName[a3keyframeAnimation_nameLenMax], const a3_KeyframePool* keyframePool, const a3ui32 firstKeyframeIndex, const a3ui32 finalKeyframeIndex, a3_ClipPool* clipPool)
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

	a3clipTransitionInit(&clip_out->forwardTransition, 0, clipPool, 0);
	a3clipTransitionInit(&clip_out->reverseTransition, 0, clipPool, 0);

	// Calcualte clip duration
	a3clipCalculateDuration(clip_out);
	
	return 1;
}

// get clip index from pool
a3i32 a3clipGetIndexInPool(const a3_ClipPool* clipPool, const a3byte clipName[a3keyframeAnimation_nameLenMax])
{
	for (a3ui32 i = 0; i < a3keyframeAnimation_nameLenMax; i++)
	{
		if(clipPool->clip[i].name == clipName)
			return i;
	}

	return 1;
}


void a3CustomInitFromFile(a3_ClipPool* clipPool, a3_KeyframePool* keyframePool, const char* filepath)
{
	FILE* file;
	a3ui32 count = 0;
	a3real dur = 0.0f;
	a3real x = 0, y = 0, z = 0;
	char buff[20];

	file = fopen(filepath, "r");
	if (file == NULL)
	{
		printf("%s", "failed to open file");
		return;
	}
	fgets(buff, 12, file);
	count = atoi(buff);
	printf("%i\n", count);

	a3keyframePoolCreate(keyframePool, count);

	for (a3ui32 i = 0; i < count; i++)
	{
		fscanf(file, "%f", &dur);

		fscanf(file, "%f", &x);

		fscanf(file, "%f", &y);

		fscanf(file, "%f", &z);

		a3vec3 pos;
		pos.x = x;
		pos.y = y;
		pos.z =z;

		a3keyframeInit(&keyframePool->keyframe[i], dur, i, pos);

		printf("%f %f %f %f\n", dur, x, y, z);
	}

	fscanf(file, "%u", &count);
	printf("%i\n", count);

	a3clipPoolCreate(clipPool, count);

	a3byte name[a3keyframeAnimation_nameLenMax];
	a3ui32 first, last, fwdTarget, revTarget;
	a3i32 fwdDir, revDir;

	for (a3ui32 i = 0; i < count; i++)
	{
		fscanf(file, "%s", &name);

		fscanf(file, "%u", &first);

		fscanf(file, "%u", &last);

		fscanf(file, "%u", &fwdTarget);

		fscanf(file, "%i", &fwdDir);
		
		fscanf(file, "%u", &revTarget);
		
		fscanf(file, "%i", &revDir);

		a3clipInit(&clipPool->clip[i], name, keyframePool, first, last, clipPool);
		a3clipTransitionInit(&clipPool->clip[i].forwardTransition, fwdDir, clipPool, fwdTarget);
		a3clipTransitionInit(&clipPool->clip[i].reverseTransition, revDir, clipPool, revTarget);

		printf("%s %u %u %u %i %u %i\n", name, first, last, fwdTarget, fwdDir, revTarget, revDir);
	}

	fclose(file);
}

//-----------------------------------------------------------------------------
