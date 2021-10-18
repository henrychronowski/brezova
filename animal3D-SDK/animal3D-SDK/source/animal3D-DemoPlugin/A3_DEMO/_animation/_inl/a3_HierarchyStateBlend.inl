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

// pointer-based reset/identity operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out)
{
	a3spatialPoseReset(pose_out);

	// done
	return pose_out;
}

inline a3_SpatialPose* a3spatialPoseOpConstruct(a3_SpatialPose* pose_out, a3vec4 const orientation, a3vec4 const angles, a3vec4 const scale, a3vec4 const translation)
{
	if (pose_out)
	{
		// Set spatial pose values
		pose_out->orientation = orientation;
		pose_out->angles = angles;
		pose_out->scale = scale;
		pose_out->translation = translation;

		// Calculate transform
		a3spatialPoseConvert(pose_out, a3poseChannel_none, a3poseEulerOrder_xyz);
	}

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseOpCopy(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_in)
{
	if (pose_out && pose_in)
		*pose_out = *pose_in; // Assign the data from pose_in to pose_out

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseOpNegate(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_in)
{
	if (pose_out && pose_in)
	{
		// Not doing quaternion inversion for now (q^-1 = conj(q) / ||q||)

		// Assign the data from pose_in to pose_out
		*pose_out = *pose_in;
		// Inverse translation
		a3real4MulS(pose_out->translation.v, -1);
		// Inverse Euler rotation
		a3real4MulS(pose_out->angles.v, -1);

		// Inverse scale
		pose_out->scale.x = 1 / pose_in->scale.x;
		pose_out->scale.y = 1 / pose_in->scale.y;
		pose_out->scale.z = 1 / pose_in->scale.z;

		a3spatialPoseConvert(pose_out, a3poseChannel_none, a3poseEulerOrder_xyz);
	}

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseOpConcat(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_lh, a3_SpatialPose* const pose_rh)
{
	if (pose_out && pose_lh && pose_rh)
	{
		// Assign the data from pose_in to pose_out
		*pose_out = *pose_lh;

		// Concatenate orientation
		a3quatProduct(pose_out->orientation.v, pose_lh->orientation.v, pose_rh->orientation.v);
		// Concatenate Euler angles
		a3real4Add(pose_out->angles.v, pose_rh->angles.v);
		// Concatenate Scale
		a3real4MulComp(pose_out->scale.v, pose_rh->angles.v);
		// Concatenate Translation
		a3real4Add(pose_out->translation.v, pose_rh->translation.v);

		// Calculate transformation
		a3spatialPoseConvert(pose_out, a3poseChannel_none,	a3poseEulerOrder_xyz);
	}

	return pose_out;
}

// pointer-based LERP operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u)
{

	// done
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

	// done
	return pose_out;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H