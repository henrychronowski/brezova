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
#include "a3_KeyframeAnimationController.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus

#endif	// __cplusplus
	
// Robust blend mode for any op
typedef a3_SpatialPose*(*a3_SpatialPoseBlendOp)(
	a3_SpatialPose* p_out,
	a3_SpatialPose const* ctrl[],
	a3real const param[]
);

typedef struct a3_SpatialPoseBlendNode
{
	a3_SpatialPoseBlendOp op;
	a3_SpatialPose* p_out;
	a3_SpatialPose const* p_ctrl[8];
	a3real const* param[8];
} a3_SpatialPoseBlendNode;

inline a3_SpatialPoseBlendNode* a3spatialPoseBlendNodeCall(
	a3_SpatialPoseBlendNode* b
)
{
	b->op(b->p_out, b->p_ctrl, b->param[0]);
	return b;
}

// example: lerp
inline a3_SpatialPose* a3_SpatialPoseBlendLerp(
	a3_SpatialPose* p_out,
	a3_SpatialPose const* p_ctrl[2],
	a3real const param[1]
)
{
	if (p_out && p_ctrl)
	{
		a3_SpatialPose const* p0 = p_ctrl[0];
		a3_SpatialPose const* p1 = p_ctrl[1];
		a3real const u = param[0];

		a3spatialPoseLerp(p_out, p0, p1, u);
	}

	return p_out;
}

//// object based
//typedef a3_SpatialPose(*a3SpatialPoseBlendOpLerp)(a3_SpatialPose const p0, a3_SpatialPose const p1, a3real const u);
//
//// pointer based
//typedef a3_SpatialPose* (*a3_SpatialPoseBlendOpLerp)(a3_SpatialPose* p_out, a3_SpatialPose const* p0, a3_SpatialPose const* p1, a3real const u);


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

// Clip operations


a3_HierarchyPose* a3ClipOpLerp(a3_HierarchyPose* pose_out, a3_ClipController* controller0, a3_ClipController* controller1, a3real u, a3ui32 const nodeCount);

a3_HierarchyPose* a3ClipOpAdd(a3_HierarchyPose* pose_out, a3_ClipController* controller0, a3_ClipController* controller1, a3ui32 const nodeCount);

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

// pointer-based smoothstep operation for single spatial pose
a3_SpatialPose* a3SpatialPoseOpSmoothstep(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* const pose1, a3real const u);

// pointer-based descale operation for single spatial pose
a3_SpatialPose* a3SpatialPoseOpDescale(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_in, a3real u);

// pointer-based convert operation for single spatial pose
a3_SpatialPose* a3SpatialPoseOpConvert(a3_SpatialPose* pose_inout);

// pointer-based revert operation for single spatial pose
a3_SpatialPose* a3SpatialPoseOpRevert(a3_SpatialPose* pose_inout);

//-----------------------------------------------------------------------------

// Pointer-based deconcatenate
a3_SpatialPose* a3SpatialPoseOpDeconcat(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_lh, a3_SpatialPose* const pose_rh);

// Pointer-based pose scale
a3_SpatialPose* a3SpatialPoseOpScale(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_in, a3real blendParam);

// Pointer-based triangular pose interpolation
a3_SpatialPose* a3SpatialPoseOpTriangular(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* pose1, a3_SpatialPose* pose2, a3real u1, a3real u2);

// Pointer-based bi-nearest
a3_SpatialPose* a3SpatialPoseOpBiNearest(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* pose1, a3_SpatialPose* poseA, a3_SpatialPose* poseB, a3real u0, a3real u1, a3real u);

// Pointer-based bi-linear interpolation
a3_SpatialPose* a3SpatialPoseOpBiLinear(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* pose1, a3_SpatialPose* poseA, a3_SpatialPose* poseB, a3real u0, a3real u1, a3real u);

// Pointer-based bi-cubic interpolation
a3_SpatialPose* a3SpatialPoseOpBiCubic(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* pose1, a3_SpatialPose* pose2, a3_SpatialPose* pose3, a3_SpatialPose* pose4, a3_SpatialPose* pose5, a3_SpatialPose* pose6, a3_SpatialPose* pose7, a3_SpatialPose* pose8, a3_SpatialPose* pose9, a3_SpatialPose* pose10, a3_SpatialPose* pose11, a3_SpatialPose* pose12, a3_SpatialPose* pose13, a3_SpatialPose* pose14, a3_SpatialPose* pose15, a3real u0, a3real u1, a3real u2, a3real u3, a3real u4);

//a3_SpatialPose* a3spatialPoseOpFK(a3_SpatialPose* pose_out, a3_Hierarchy* hierarchy_in, a3_SpatialPose* localPose);

//-----------------------------------------------------------------------------

// data-based reset/identity
a3_SpatialPose a3spatialPoseDOpIdentity();

// data-based LERP
a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u);


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out, a3ui32 const nodeCount);

// pointer-based LERP operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u, a3ui32 const nodeCount);

// Pointer-based construct operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpConstruct(a3_HierarchyPose* pose_out, a3vec4 const orientation, a3vec4 const angles, a3vec4 const scale, a3vec4 const translation, a3ui32 const nodeCount);

// Pointer-based copy operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpCopy(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_in, a3ui32 const nodeCount);

// Pointer-based invert operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpInvert(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_in, a3ui32 const nodeCount);

// Pointer-based concat operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpConcat(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_lh, a3_HierarchyPose* const pose_rh, a3ui32 const nodeCount);

// Pointer-based nearest for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpNearest(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_lh, a3_HierarchyPose* const pose_rh, a3real const u);

// Pointer-based cubic for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpCubic(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3_HierarchyPose const* pose2, a3_HierarchyPose const* pose3, a3real const u, a3ui32 const nodeCount);

// Pointer-based deconcatenate for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpDeconcat(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_lh, a3_HierarchyPose* const pose_rh, a3ui32 const nodeCount);

// Pointer-based scale for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpScale(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_in, a3real blendParam, a3ui32 const nodeCount);

// Pointer-based triangular interpolation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpTriangular(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* pose1, a3_HierarchyPose* pose2, a3real u1, a3real u2, a3ui32 const nodeCount);

// Pointer-based bi-nearest interpolation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpBiNearest(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* pose1, a3_HierarchyPose* poseA, a3_HierarchyPose* poseB, a3real u0, a3real u1, a3real u, a3ui32 const nodeCount);

// Pointer-based bi-linear interpolation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpBiLinear(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* pose1, a3_HierarchyPose* poseA, a3_HierarchyPose* poseB, a3real u0, a3real u1, a3real u, a3ui32 const nodeCount);

// Pointer-based bi-cubic interpolation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpBiCubic(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* pose1, a3_HierarchyPose* pose2, a3_HierarchyPose* pose3, a3_HierarchyPose* pose4, a3_HierarchyPose* pose5, a3_HierarchyPose* pose6, a3_HierarchyPose* pose7, a3_HierarchyPose* pose8, a3_HierarchyPose* pose9, a3_HierarchyPose* pose10, a3_HierarchyPose* pose11, a3_HierarchyPose* pose12, a3_HierarchyPose* pose13, a3_HierarchyPose* pose14, a3_HierarchyPose* pose15, a3real u0, a3real u1, a3real u2, a3real u3, a3real u4, a3ui32 const nodeCount);

// Pointer-based smoothstep interpolation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpSmoothstep(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1, const a3real u, a3ui32 const nodeCount);

// Pointer-based descale for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpDescale(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_in, const a3real u, a3ui32 const nodeCount);

// Pointer-based convert for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpConvert(a3_HierarchyPose* pose_inout, a3ui32 const nodeCount);

// Pointer-based revert for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpRevert(a3_HierarchyPose* pose_inout, a3ui32 const nodeCount);

//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_HierarchyStateBlend.inl"


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_H