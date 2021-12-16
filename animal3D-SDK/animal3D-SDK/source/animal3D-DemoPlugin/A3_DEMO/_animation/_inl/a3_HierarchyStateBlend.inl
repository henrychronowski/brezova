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
	
	a3_HierarchyStateBlend.inl
	Implementation of inline hierarchical blend operations.
*/


#ifdef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_INL
#define __ANIMAL3D_HIERARCHYSTATEBLEND_INL

//-----------------------------------------------------------------------------

inline a3vec4 a3vec4Lerp(a3vec4 const v0, a3vec4 const v1, a3real const u)
{
	a3vec4 out_vec = a3vec4_zero;
	a3real4Lerp(out_vec.v, v0.v, v1.v, u);
	return out_vec;
}

inline a3vec4 a3vec4LogLerp(a3vec4 const v0, a3vec4 const v1, a3real const u)
{
	a3vec4 out_vec = a3vec4_zero;

	out_vec.x = powf(v1.x * powf(v0.x, -1), u) * v0.x;
	out_vec.y = powf(v1.y * powf(v0.y, -1), u) * v0.y;
	out_vec.z = powf(v1.z * powf(v0.z, -1), u) * v0.z;
	out_vec.w = powf(v1.w * powf(v0.w, -1), u) * v0.w;

	return out_vec;
}

inline a3vec4 a3vec4SLerp(a3vec4 const v0, a3vec4 const v1, a3real const u)
{
	a3vec4 out_vec = a3vec4_zero;
	a3real4Slerp(out_vec.v, v0.v, v1.v, u);

	return out_vec;
}

inline a3vec4 a3vec4NLerp(a3vec4 const v0, a3vec4 const v1, a3real const u)
{
	a3vec4 out_vec = a3vec4_zero;
	a3real4NLerp(out_vec.v, v0.v, v1.v, u);

	return out_vec;
}

//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out)
{
	pose_out->transformMat = a3mat4_identity;
	// ...

	// done
	return pose_out;
}

// pointer-based LERP operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u)
{
	if (pose_out && pose0 && pose1)
	{
		pose_out->rotate = a3vec4Lerp(pose0->rotate, pose1->rotate, u);
		pose_out->scale = a3vec4LogLerp(pose0->scale, pose1->scale, u);
		pose_out->translate = a3vec4Lerp(pose0->translate, pose1->translate, u);

		a3spatialPoseConvert(pose_out, a3poseChannel_none, a3poseEulerOrder_xyz);
	}
	// done
	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseOpBiLinear(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* pose1, a3_SpatialPose* poseA, a3_SpatialPose* poseB, a3real u0, a3real u1, a3real u)
{
	if (pose_out && pose0 && pose1 && poseA && poseB)
	{
		a3_SpatialPose* tmp;

		// Interpolate pose 0 and pose 1
		a3spatialPoseOpLERP(pose_out, pose0, pose1, u0);

		// Interpolate pose A and pose B
		a3spatialPoseOpLERP(tmp, poseA, poseB, u1);

		// Interpolate the results of the previous 2 interpolations
		a3spatialPoseOpLERP(pose_out, pose_out, tmp, u);
	}

	return pose_out;
}


//-----------------------------------------------------------------------------

// data-based reset/identity
inline a3_SpatialPose a3spatialPoseDOpIdentity()
{
	a3_SpatialPose const result = { a3mat4_identity /*, ...*/ };
	return result;
}

// data-based LERP
inline a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u)
{
	a3_SpatialPose result = { 0 };
	// ...

	// done
	return result;
}


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out)
{

	// done
	return pose_out;
}

// pointer-based LERP operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u)
{
	if (pose_out && pose0 && pose1)
	{
		
	}

	// done
	return pose_out;
}

inline a3_HierarchyPose* a3hierarchyPoseOpBiLinear(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* pose1, a3_HierarchyPose* poseA, a3_HierarchyPose* poseB, a3real u0, a3real u1, a3real u, a3ui32 const nodeCount)
{
	if (pose_out && pose0 && pose1 && poseA && poseB)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3SpatialPoseOpBiLinear(pose_out->pose + i, pose0->pose + i, pose1->pose + i, poseA->pose + i, poseB->pose + i, u0, u1, u);
		}
	}

	return pose_out;
}

//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H