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
	// Lerp
	a3vec4 out_vec = a3vec4_zero;
	a3real4Lerp(out_vec.v, v0.v, v1.v, u);

	return out_vec;
}

inline a3vec4 a3vec4LogLerp(a3vec4 const v0, a3vec4 const v1, a3real const u)
{
	a3vec4 out_vec = a3vec4_zero;
	
	// Log lerp each component - https://www.cmu.edu/biolphys/deserno/pdf/log_interpol.pdf
	out_vec.x = powf(v1.x, u) * powf(v1.x, u-1);
	out_vec.x = powf(v1.y, u) * powf(v1.y, u-1);
	out_vec.x = powf(v1.z, u) * powf(v1.z, u-1);
	out_vec.x = powf(v1.w, u) * powf(v1.w, u-1);
	
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

inline a3_SpatialPose* a3SpatialPoseOpNearest(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* const pose1, a3real const u)
{
	if (pose_out)
	{
		// Check interpolation parameter
		if(u >= 0.5)
			*pose_out = *pose1;	// Second control pose
		else
			*pose_out = *pose0; // First control pose
	}

	return pose_out;
}

// pointer-based LERP operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u)
{
	if (pose_out && pose0 && pose1)
	{
		// Interpolate quaternion rotation
		pose_out->orientation = a3vec4SLerp(pose0->orientation, pose1->orientation, u);
		// Interpolate Euler rotation
		pose_out->angles = a3vec4Lerp(pose0->angles, pose1->angles, u);
		// Interpolate scale
		pose_out->scale = a3vec4LogLerp(pose0->scale, pose1->scale, u);
		// Interpolate translation
		pose_out->translation = a3vec4Lerp(pose0->translation, pose1->translation, u);

		// Calculate transform
		a3spatialPoseConvert(pose_out, a3poseChannel_none, a3poseEulerOrder_xyz);
	}

	// done
	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseOpCubic(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3_SpatialPose const* pose2, a3_SpatialPose const* pose3, a3real const u)
{
	if (pose_out && pose0 && pose1 && pose2 && pose3)
	{
		// Interpolate quaternion rotation
		a3real4CatmullRom(pose_out->orientation.v, pose0->orientation.v, pose1->orientation.v, pose2->orientation.v, pose3->orientation.v, u);
		// Interpolate Euler rotation
		a3real4CatmullRom(pose_out->angles.v, pose0->angles.v, pose1->angles.v, pose2->angles.v, pose3->angles.v, u);
		// Interpolate scale
		a3real4CatmullRom(pose_out->scale.v, pose0->scale.v, pose1->scale.v, pose2->scale.v, pose3->scale.v, u);
		// Interpolate translation
		a3real4CatmullRom(pose_out->translation.v, pose0->translation.v, pose1->translation.v, pose2->translation.v, pose3->translation.v, u);

		// Calculate transform
		a3spatialPoseConvert(pose_out, a3poseChannel_none, a3poseEulerOrder_xyz);
	}

	return pose_out;
}

//-----------------------------------------------------------------------------


inline a3_SpatialPose* a3SpatialPoseOpDeconcat(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_lh, a3_SpatialPose* const pose_rh)
{
	if (pose_out && pose_lh && pose_rh)
	{
		// Assign the data from pose_in to pose_out
		*pose_out = *pose_lh;

		// deconcatenate orientation
		a3quatProduct(pose_out->orientation.v, pose_lh->orientation.v, a3quatInvert(pose_rh->orientation.v));
		// deconcatenate Euler angles
		a3real4Sub(pose_out->angles.v, pose_rh->angles.v);
		// deconcatenate Scale
		a3real4DivComp(pose_out->scale.v, pose_rh->angles.v);
		// deconcatenate Translation
		a3real4Sub(pose_out->translation.v, pose_rh->translation.v);

		// Calculate transformation
		a3spatialPoseConvert(pose_out, a3poseChannel_none, a3poseEulerOrder_xyz);
	}

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseOpScale(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_in, a3real blendParam)
{
	if (pose_out && pose_in)
	{
		// Interpolate between poses
		a3spatialPoseOpLERP(pose_out, a3spatialPoseOpIdentity(pose_out), pose_in, blendParam);
	}

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseOpTriangular(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* pose1, a3_SpatialPose* pose2, a3real u1, a3real u2)
{
	if (pose_out && pose0 && pose1 && pose2)
	{
		a3real u0 = 1 - u1 - u2;
		a3_SpatialPose* tmp;

		// Scale initial pose (pose 0)
		a3SpatialPoseOpScale(pose_out, pose0, u0);
		// Scale pose 1
		a3SpatialPoseOpScale(tmp, pose1, u1);
		// Concatenate pose 0 and pose 1
		a3SpatialPoseOpConcat(pose_out, pose_out, tmp);
		// Scale pose 2
		a3SpatialPoseOpScale(tmp, pose2, u2);
		// Concatenate pose 2 with the result of concat(pose0, pose1)
		a3SpatialPoseOpConcat(pose_out, pose_out, tmp);
	}

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseOpBiNearest(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* pose1, a3_SpatialPose* poseA, a3_SpatialPose* poseB, a3real u0, a3real u1, a3real u)
{
	if (pose_out && pose0 && pose1 && poseA && poseB)
	{
		if (u >= 0.5f)
		{
			if (u1 >= 0.5f)
			{
				*pose_out = *poseB;
			}
			else
			{
				*pose_out = *poseA;
			}
		}
		else if(u0 >= 0.5f)
		{
			*pose_out = *pose1;
		}
		else
		{
			*pose_out = *pose0;
		}
	}

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

inline a3_SpatialPose* a3SpatialPoseOpBiCubic(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* pose1, a3_SpatialPose* pose2, a3_SpatialPose* pose3, a3_SpatialPose* pose4, a3_SpatialPose* pose5, a3_SpatialPose* pose6, a3_SpatialPose* pose7, a3_SpatialPose* pose8, a3_SpatialPose* pose9, a3_SpatialPose* pose10, a3_SpatialPose* pose11, a3_SpatialPose* pose12, a3_SpatialPose* pose13, a3_SpatialPose* pose14, a3_SpatialPose* pose15, a3real u0, a3real u1, a3real u2, a3real u3, a3real u4)
{
	if (pose_out && pose0 && pose1 && pose2 && pose3 && pose4 && pose5 && pose6 && pose7 && pose8 && pose9 && pose10 && pose11 && pose12 && pose13 && pose14 && pose15)
	{
		a3_SpatialPose* tmp0;
		a3_SpatialPose* tmp1;
		a3_SpatialPose* tmp2;

		// Blend individuals
		a3SpatialPoseOpCubic(pose_out, pose0, pose1, pose2, pose3, u0);
		a3SpatialPoseOpCubic(tmp0, pose4, pose5, pose6, pose7, u1);
		a3SpatialPoseOpCubic(tmp1, pose8, pose9, pose10, pose11, u2);
		a3SpatialPoseOpCubic(tmp2, pose12, pose13, pose14, pose15, u3);

		// Blend the blend results
		a3SpatialPoseOpCubic(pose_out, pose_out, tmp0, tmp1, tmp2, u4);
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
inline a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out, a3ui32 const nodeCount)
{
	if (pose_out)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3spatialPoseOpIdentity(&pose_out->pose[i]);
		}
	}
	// done
	return pose_out;
}

// pointer-based LERP operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u, a3ui32 const nodeCount)
{
	if (pose_out && pose0 && pose1)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3spatialPoseOpLERP(&pose_out->pose[i], &pose0->pose[i], &pose1->pose[i], u);
		}
	}

	// done
	return pose_out;
}

// pointer-based construct operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpConstruct(a3_HierarchyPose* pose_out, a3vec4 const orientation, a3vec4 const angles, a3vec4 const scale, a3vec4 const translation, a3ui32 const nodeCount)
{
	if (pose_out)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3spatialPoseOpConstruct(&pose_out->pose[i], orientation, angles, scale, translation);
		}
	}

	return pose_out;
}

// Pointer-based copy operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpCopy(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_in, a3ui32 const nodeCount)
{
	if (pose_out && pose_in)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3spatialPoseCopy(&pose_out->pose[i], &pose_in->pose[i]);
		}
	}

	return pose_out;
}

// Pointer-based invert operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpInvert(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_in, a3ui32 const nodeCount)
{
	if (pose_out && pose_in)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3SpatialPoseOpNegate(&pose_out->pose[i], &pose_in->pose[i]);
		}
	}

	return pose_out;
}

//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H