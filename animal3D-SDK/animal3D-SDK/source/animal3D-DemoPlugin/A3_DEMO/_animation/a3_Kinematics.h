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
	
	a3_Kinematics.h
	Hierarchical kinematics solvers.
*/

#ifndef __ANIMAL3D_KINEMATICS_H
#define __ANIMAL3D_KINEMATICS_H


#include "a3_HierarchyState.h"
//#include "A3_DEMO/a3_DemoMode1_Animation.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_AIController a3_AIController;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// general forward kinematics: 
// given local transforms for hierarchy nodes, calculate object-space: 
//		if not root, 
//			object-space node = object-space parent * local-space node
//		else
//			object-space node = local-space node

// forward kinematics solver given an initialized hierarchy state
a3i32 a3kinematicsSolveForward(const a3_HierarchyState *hierarchyState);

// forward kinematics solver starting at a specified joint
a3i32 a3kinematicsSolveForwardPartial(const a3_HierarchyState *hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount);

// single node FK solver
void a3kinematicsSolveForwardSingle(const a3_HierarchyState* hierarchyState, const a3ui32 index, const a3ui32 parentIndex);


//-----------------------------------------------------------------------------

// general inverse kinematics: 
// given object transforms for hierarchy nodes, calculate local-space: 
//		if not root, 
//			local-space node = inverse object-space parent * object-space node
//		else
//			local-space node = object-space node

// inverse kinematics solver given an initialized hierarchy state
a3i32 a3kinematicsSolveInverse(const a3_HierarchyState *hierarchyState);

// inverse kinematics solver starting at a specified joint
a3i32 a3kinematicsSolveInversePartial(const a3_HierarchyState *hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount);

// single node IK solver
void a3kinematicsSolveInverseSingle(const a3_HierarchyState* hierarchyState, const a3ui32 index, const a3ui32 parentIndex);


//-----------------------------------------------------------------------------

// AI Controller functionality
//	
//

struct a3_AIController
{
	a3vec3 targetLocation;
	a3vec3 targetVelocity;
	a3i16 targetIndex;
	a3vec3 predictLocation;
	a3vec3 curLocation;
	a3vec3 curVelocity;

	a3f64 devianceThreshold;

	a3real tmpTimeTrack;
	a3real fireSpeed;
};

// Sets default values for the AI controller
a3i32 a3AIReset(a3_AIController* inout);

// Update the estimate to reflect current target status
a3i32 a3AIUpdate(a3_AIController* inout, a3vec3* pos, a3vec3* vel, const a3f64 dt);

// Get the direction of movement necessary
a3vec3 a3AIGetMovementInput(const a3_AIController* in);

// Get the necessary aim point
a3vec3 a3AIGetAimInput(const a3_AIController* in, const a3vec3 * neckPos);

// Fire
a3i32 a3AIFire(a3_AIController* in);

//-----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_Kinematics.inl"


#endif	// !__ANIMAL3D_KINEMATICS_H