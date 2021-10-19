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
	
	a3_HierarchyStateBlend.h
	Hierarchy blend operations.
*/

#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#define __ANIMAL3D_HIERARCHYSTATEBLEND_H


#include "a3_HierarchyState.h"

#include "a3_Kinematics.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus

#endif	// __cplusplus
	
// Blend operation function pointer
typedef a3vec4(*a3_BlendOpLerp)(a3vec4 const v0, a3vec4 const v1, a3real const u);
typedef struct a3_SpatialPoseBlendOpLerp
{
	a3_BlendOpLerp opOrientation, opAngles, opScale, opTranslation;
} a3_SpatialPoseBlendOpLerp;


a3vec4 a3vec4Lerp(a3vec4 const v0, a3vec4 const v1, a3real const u);

a3vec4 a3vec4LogLerp(a3vec4 const v0, a3vec4 const v1, a3real const u);

a3vec4 a3vec4SLerp(a3vec4 const v0, a3vec4 const v1, a3real const u);

a3vec4 a3vec4NLerp(a3vec4 const v0, a3vec4 const v1, a3real const u);

//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out);

// Pointer-based construct operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpConstruct(a3_SpatialPose* pose_out, a3vec4 const orientation, a3vec4 const angles, a3vec4 const scale, a3vec4 const translation);

// Pointer-based copy operation
a3_SpatialPose* a3SpatialPoseOpCopy(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_in);

// Pointer-based negate operation
a3_SpatialPose* a3SpatialPoseOpNegate(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_in);

// Pointer-based concatenate
a3_SpatialPose* a3SpatialPoseOpConcat(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_lh, a3_SpatialPose* const pose_rh);

// Pointer-based nearest operation
a3_SpatialPose* a3SpatialPoseOpNearest(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* const pose1, a3real const u);

// pointer-based LERP operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u);

// Pointer-based cubic interpolation operation
a3_SpatialPose* a3SpatialPoseOpCubic(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3_SpatialPose const* pose2, a3_SpatialPose const* pose3, a3real const u);


//-----------------------------------------------------------------------------

// Pointer-based deconcatenate
a3_SpatialPose* a3SpatialPoseOpDeconcat(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_lh, a3_SpatialPose* const pose_rh);

// Pointer-based pose scale
a3_SpatialPose* a3SpatialPoseOpScale(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_in, a3real blendParam);

// Pointer-based triangular pose interpolation
a3_SpatialPose* a3SpatialPoseOpTriangular(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* pose1, a3_SpatialPose* pose2, a3real u1, a3real u2);

//-----------------------------------------------------------------------------

// data-based reset/identity
a3_SpatialPose a3spatialPoseDOpIdentity();

// data-based LERP
a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u);


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out);

// pointer-based LERP operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_HierarchyStateBlend.inl"


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_H