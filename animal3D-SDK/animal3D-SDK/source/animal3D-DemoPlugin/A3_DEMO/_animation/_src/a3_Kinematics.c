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
	
	a3_Kinematics.c
	Implementation of kinematics solvers.
*/

#include "../a3_Kinematics.h"
#include <stdio.h>

a3i32 a3AIReset(a3_AIController * inout)
{
	if (inout)
	{
		inout->targetLocation = a3vec3_zero;
		inout->targetVelocity = a3vec3_zero;
		inout->targetIndex = 0;
		inout->predictLocation = a3vec3_zero;
		inout->curLocation = a3vec3_zero;
		inout->curVelocity = a3vec3_zero;
		inout->fireSpeed = 10.0;

		inout->tmpTimeTrack = a3real_zero;

		return 1;
	}
	return -1;
}

a3i32 a3AIUpdate(a3_AIController* inout, a3vec3* pos, a3vec3* vel, const a3f64 dt)
{
	if (inout && dt >= __a3f64zero)
	{
		inout->tmpTimeTrack += (a3real)dt;

		// Update current position and velocity of target
		inout->targetLocation = a3vec3_y;
		a3real3Add(inout->targetLocation.v, a3vec3_x.v);
		a3real3MulS(inout->targetLocation.v, a3absolute(a3sinrv(inout->tmpTimeTrack) * 10));
		//printf("%f|%f|%f // %f|%f|%f\n", inout->targetLocation.x, inout->targetLocation.y, inout->targetLocation.z, pos->x, pos->y, pos->z);
		//printf("%f\n", inout->tmpTimeTrack);
		inout->curLocation = *pos;
		inout->curVelocity = *vel;

		// Check for firing solution
		a3AIFire();

		return 1;
	}

	return -1;
}

a3i32 a3AIFire()
{
	//printf("No fire solution found\n");
	return 1;
}

a3vec3 a3AIGetMovementInput(const a3_AIController* in)
{
	if (in)
	{
		a3vec3 result = in->targetLocation;
			//in->targetLocation;
		
		a3real3Sub(result.v, in->curLocation.v);//in->curLocation.v);
		a3real3Normalize(result.v);

		if(a3absolute(result.x + result.y + result.z) >= 0.1)
			return result;
	}
	return a3vec3_zero;
}

a3vec3 a3AIGetAimInput(const a3_AIController* in, const a3vec3* neckPos)
{
	if (in)
	{
		a3vec3 result;
		result = a3vec3_x;
		/*a3vec3 PT;

		a3real sT, speedFactor;

		PT = in->targetLocation;

		a3real3Sub(PT.v, neckPos->v);
		sT = a3real3Dot(in->targetVelocity.v, PT.v) / a3real3Length(PT.v);
		speedFactor = sT - in->fireSpeed;*/
		

		result = in->targetLocation;
		//a3real3MulS(result.v, 10.0);
		//a3real3Add(a3real3MulS(a3real3MulS(a3real3Sub(result.v, neckPos->v), speedFactor), 1.0f / a3real3Distance(in->targetLocation.v, neckPos->v)), in->targetVelocity.v);

		return result;
	}

	return a3vec3_zero;
}

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
