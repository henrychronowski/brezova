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

		return 1;
	}
	return -1;
}

a3i32 a3AIUpdate(a3_AIController* inout, a3vec2* pos, const a3f64 dt)
{
	if (inout && dt >= __a3f64zero)
	{
		// Update current position and velocity of target
		inout->targetLocation = a3vec3_y;
		a3real3MulS(inout->targetLocation.v, 3);
		//printf("%f %f %f\n", inout->targetLocation.x, inout->targetLocation.y, inout->targetLocation.z);


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

a3vec3 a3AIGetMovementInput(const a3_AIController* in, const a3vec2* pos)
{
	if (in)
	{
		a3vec3 result = a3vec3_zero;
		result.x = pos->x;
		result.y = pos->y;
		
		a3real3Sub(result.v, in->targetLocation.v);
		a3real3GetNegative(result.v, result.v);
		a3real3Normalize(result.v);

		return result;
	}
	return a3vec3_zero;
}

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
