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

	out_vec.x = powf(v1.x * powf(v0.x, -1), u) * v0.x; //powf(v1.x, u) * powf(v1.x, u-1);
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

inline a3_HierarchyPose* a3ClipOpLerp(a3_HierarchyPose* pose_out, a3_ClipController* controller0, a3_ClipController* controller1, a3real u, a3ui32 const nodeCount)
{
	if (pose_out && controller0 && controller1)
	{
		a3_HierarchyPose p0;
		a3_HierarchyPose p1;

		a3_HierarchyPose* nextPose = controller0->currentClip->keyframePool->keyframe[(controller0->keyframe - controller0->currentClip->firstKeyframe + 1) % controller0->currentClip->keyframeCount].data;
		a3hierarchyPoseOpLERP(&p0, controller0->currentKeyframe->data, nextPose, controller0->keyframeParameter, nodeCount);

		nextPose = controller1->currentClip->keyframePool->keyframe[(controller1->keyframe - controller1->currentClip->firstKeyframe + 1) % controller1->currentClip->keyframeCount].data;
		a3hierarchyPoseOpLERP(&p1, controller1->currentKeyframe->data, nextPose, controller1->keyframeParameter, nodeCount);

		a3hierarchyPoseOpLERP(pose_out, &p0, &p1, u, nodeCount);
	}

	return pose_out;
}

inline a3_HierarchyPose* a3ClipOpAdd(a3_HierarchyPose* pose_out, a3_ClipController* controller0, a3_ClipController* controller1, a3ui32 const nodeCount)
{
	if (pose_out && controller0 && controller1)
	{
		a3_HierarchyPose p0;
		a3_HierarchyPose p1;

		a3_HierarchyPose* nextPose = controller0->currentClip->keyframePool->keyframe[(controller0->keyframe - controller0->currentClip->firstKeyframe + 1) % controller0->currentClip->keyframeCount].data;
		a3hierarchyPoseOpLERP(&p0, controller0->currentKeyframe->data, nextPose, controller0->keyframeParameter, nodeCount);

		nextPose = controller1->currentClip->keyframePool->keyframe[(controller1->keyframe - controller1->currentClip->firstKeyframe + 1) % controller1->currentClip->keyframeCount].data;
		a3hierarchyPoseOpLERP(&p1, controller1->currentKeyframe->data, nextPose, controller1->keyframeParameter, nodeCount);

		a3hierarchyPoseOpConcat(pose_out, &p0, &p1, nodeCount);
	}
	
	return pose_out;
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
		// Assign the data from pose_in to pose_out
		*pose_out = *pose_in;
		
		// Invert quaternion rotation
		a3quatInvert(pose_out->orientation.v);
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

		a3spatialPoseConcat(pose_out, pose_lh, pose_rh);

		//// Concatenate orientation
		//a3quatProduct(pose_out->orientation.v, pose_lh->orientation.v, pose_rh->orientation.v);
		//// Concatenate Euler angles
		//a3real4Add(pose_out->angles.v, pose_rh->angles.v);
		//// Concatenate Scale
		//a3real4MulComp(pose_out->scale.v, pose_rh->scale.v);
		//// Concatenate Translation
		//a3real4Add(pose_out->translation.v, pose_rh->translation.v);

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

// pointer-based smoothstep operation for single spatial pose
inline a3_SpatialPose* a3SpatialPoseOpSmoothstep(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* const pose1, a3real const u)
{
	if (pose_out && pose0 && pose1)
	{
		// Perform lerp using the cubic hermite interpolation on u to get a new smoothed interpolation parameter
		a3real hermiteU = 3 * u * u - 2 * u * u * u;

		a3spatialPoseOpLERP(pose_out, pose0, pose1, hermiteU);
	}

	return pose_out;
}

// pointer-based descale operation for single spatial pose
inline a3_SpatialPose* a3SpatialPoseOpDescale(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_in, a3real u)
{
	if (pose_out && pose_in)
	{
		// Perform a lerp between the identity pose and the inverse control pose
		a3spatialPoseOpLERP(pose_out, a3spatialPoseOpIdentity(pose_out), a3SpatialPoseOpNegate(pose_in, pose_in), u);
	}

	return pose_out;
}

// pointer-based convert operation for single spatial pose
inline a3_SpatialPose* a3SpatialPoseOpConvert(a3_SpatialPose* pose_inout)
{
	if (pose_inout)
	{
		// Run the convert function on the pose
		a3spatialPoseConvert(pose_inout, a3poseChannel_none, a3poseEulerOrder_xyz);
	}

	return pose_inout;
}

// pointer-based revert operation for single spatial pose
inline a3_SpatialPose* a3SpatialPoseOpRevert(a3_SpatialPose* pose_inout)
{
	if (pose_inout)
	{
		// Calling the restore spatial pose function on the pose
		a3spatialPoseRestore(pose_inout, a3poseChannel_none, a3poseEulerOrder_xyz);
	}

	return pose_inout;
}

//inline a3_SpatialPose* a3spatialPoseOpFK(a3_SpatialPose* pose_out, a3_Hierarchy* hierarchy_in, a3_SpatialPose* localPose)
//{
//	if (pose_out && hierarchy_in && localPose)
//	{
//		a3SpatialPoseOpConcat(pose_out, pose_out, localPose);
//	}
//
//	return pose_out;
//}


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

// Pointer-based concat operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpConcat(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_lh, a3_HierarchyPose* const pose_rh, a3ui32 const nodeCount)
{
	if (pose_out && pose_lh && pose_rh)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3SpatialPoseOpConcat(&pose_out->pose[i], &pose_lh->pose[i], &pose_rh->pose[i]);
		}
	}

	return pose_out;
}

// Pointer-based nearest
inline a3_HierarchyPose* a3hierarchyPoseOpNearest(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_lh, a3_HierarchyPose* const pose_rh, a3real const u)
{
	if (pose_out && pose_lh && pose_rh)
	{
		if (u >= 0.5f)
		{
			*pose_out->pose = *pose_rh->pose;
		}
		else
		{
			*pose_out->pose = *pose_lh->pose;
		}
	}
	return pose_out;
}

// Pointer-based cubic
inline a3_HierarchyPose* a3hierarchyPoseOpCubic(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3_HierarchyPose const* pose2, a3_HierarchyPose const* pose3, a3real const u, a3ui32 const nodeCount)
{
	if (pose_out && pose0 && pose1 && pose2 && pose3)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3SpatialPoseOpCubic(&pose_out->pose[i], &pose0->pose[i], &pose1->pose[i], &pose2->pose[i], &pose3->pose[i], u);
		}
	}

	return pose_out;
}

// Pointer-based deconcatenate for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpDeconcat(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_lh, a3_HierarchyPose* const pose_rh, a3ui32 const nodeCount)
{
	if (pose_out && pose_lh && pose_rh)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3SpatialPoseOpDeconcat(&pose_out->pose[i], &pose_lh->pose[i], &pose_rh->pose[i]);
		}
	}

	return pose_out;
}

// Pointer-based scale for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpScale(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_in, a3real blendParam, a3ui32 const nodeCount)
{
	if (pose_out && pose_in)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3SpatialPoseOpScale(&pose_out->pose[i], &pose_in->pose[i], blendParam);
		}
	}

	return pose_out;
}

// Pointer-based triangular interpolation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpTriangular(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* pose1, a3_HierarchyPose* pose2, a3real u1, a3real u2, a3ui32 const nodeCount)
{
	if (pose_out && pose0 && pose1 && pose2)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3SpatialPoseOpTriangular(&pose_out->pose[i], &pose0->pose[i], &pose1->pose[i], &pose2->pose[i], u1, u2);
		}
	}

	return pose_out;
}

// Pointer-based bi-nearest interpolation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpBiNearest(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* pose1, a3_HierarchyPose* poseA, a3_HierarchyPose* poseB, a3real u0, a3real u1, a3real u, a3ui32 const nodeCount)
{
	if (pose_out && pose0 && pose1 && poseA && poseB)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3SpatialPoseOpBiNearest(&pose_out->pose[i], &pose0->pose[i], &pose1->pose[i], &poseA->pose[i], &poseB->pose[i], u0, u1, u);
		}
	}

	return pose_out;
}

// Pointer-based bi-linear interpolation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpBiLinear(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* pose1, a3_HierarchyPose* poseA, a3_HierarchyPose* poseB, a3real u0, a3real u1, a3real u, a3ui32 const nodeCount)
{
	if (pose_out && pose0 && pose1 && poseA && poseB)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3SpatialPoseOpBiLinear(&pose_out->pose[i], &pose0->pose[i], &pose1->pose[i], &poseA->pose[i], &poseB->pose[i], u0, u1, u);
		}
	}

	return pose_out;
}

// Pointer-based bi-cubic interpolation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpBiCubic(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* pose1, a3_HierarchyPose* pose2, a3_HierarchyPose* pose3, a3_HierarchyPose* pose4, a3_HierarchyPose* pose5, a3_HierarchyPose* pose6, a3_HierarchyPose* pose7, a3_HierarchyPose* pose8, a3_HierarchyPose* pose9, a3_HierarchyPose* pose10, a3_HierarchyPose* pose11, a3_HierarchyPose* pose12, a3_HierarchyPose* pose13, a3_HierarchyPose* pose14, a3_HierarchyPose* pose15, a3real u0, a3real u1, a3real u2, a3real u3, a3real u4, a3ui32 const nodeCount)
{
	if (pose_out && pose0 && pose1 && pose2 && pose3 && pose4 && pose5 && pose6 && pose7 && pose8 && pose9 && pose10 && pose11 && pose12 && pose13 && pose14 && pose15)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3SpatialPoseOpBiCubic(&pose_out->pose[i], &pose0->pose[i], &pose1->pose[i], &pose2->pose[i], &pose3->pose[i], &pose4->pose[i], &pose5->pose[i], &pose6->pose[i], &pose7->pose[i], &pose8->pose[i], &pose9->pose[i], &pose10->pose[i], &pose11->pose[i], &pose12->pose[i], &pose13->pose[i], &pose14->pose[i], &pose15->pose[i], u0, u1, u2, u3, u4);
		}
	}

	return pose_out;
}

// Pointer-based smoothstep interpolation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpSmoothstep(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1, const a3real u, a3ui32 const nodeCount)
{
	if (pose_out && pose0 && pose1)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			// hermite interpolation of u is carried out within spatial pose op, maybe could optimize?
			a3SpatialPoseOpSmoothstep(&pose_out->pose[i], &pose0->pose[i], &pose1->pose[i], u);
		}
	}

	return pose_out;
}

// Pointer-based descale for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpDescale(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_in, const a3real u, a3ui32 const nodeCount)
{
	if (pose_out && pose_in)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			// call spatial pose descale
			a3SpatialPoseOpDescale(&pose_out->pose[i], &pose_in->pose[i], u);
		}
	}

	return pose_out;
}

// Pointer-based convert for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpConvert(a3_HierarchyPose* pose_inout, a3ui32 const nodeCount)
{
	if (pose_inout)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			// call spatial pose convert
			a3SpatialPoseOpConvert(&pose_inout->pose[i]);
		}
	}

	return pose_inout;
}

// Pointer-based revert for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpRevert(a3_HierarchyPose* pose_inout, a3ui32 const nodeCount)
{
	if (pose_inout)
	{
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			// call spatial pose revert
			a3SpatialPoseOpRevert(&pose_inout->pose[i]);
		}
	}

	return pose_inout;
}

//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H