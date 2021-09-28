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
	
	a3_SpatialPose.inl
	Implementation of inline spatial pose operations.
*/


#ifdef __ANIMAL3D_SPATIALPOSE_H
#ifndef __ANIMAL3D_SPATIALPOSE_INL
#define __ANIMAL3D_SPATIALPOSE_INL


//-----------------------------------------------------------------------------

// set rotation values for a single node pose
inline a3i32 a3spatialPoseSetRotation(a3_SpatialPose* spatialPose, const a3f32 rx_degrees, const a3f32 ry_degrees, const a3f32 rz_degrees)
{
	if (spatialPose)
	{

	}
	return -1;
}

// scale
inline a3i32 a3spatialPoseSetScale(a3_SpatialPose* spatialPose, const a3f32 sx, const a3f32 sy, const a3f32 sz)
{
	if (spatialPose)
	{

	}
	return -1;
}

// translation
inline a3i32 a3spatialPoseSetTranslation(a3_SpatialPose* spatialPose, const a3f32 tx, const a3f32 ty, const a3f32 tz)
{
	if (spatialPose)
	{

	}
	return -1;
}


//-----------------------------------------------------------------------------

// reset single node pose
inline a3i32 a3spatialPoseReset(a3_SpatialPose* spatialPose)
{
	if (spatialPose)
	{
		spatialPose->transform = a3mat4_identity;	// mul
		spatialPose->rotate_quat = a3vec4_w;		// mul
		spatialPose->rotate_euler = a3vec4_zero;	// add
		spatialPose->translate = a3vec4_zero;		// add
		spatialPose->scale = a3vec4_one;			// mul
	}
	return -1;
}

inline a3i32 a3spatialPoseInit(a3_SpatialPose* spatialPose, a3mat4 transform, a3vec4 rotation, a3vec4 translate, a3vec4 scale)
{
	if (spatialPose)
	{
		spatialPose->transform = transform;
		spatialPose->rotate_euler = rotation;
		spatialPose->translate = translate;
		spatialPose->scale = scale;
	}
}

// convert single node pose to matrix
inline a3i32 a3spatialPoseConvert(a3mat4* mat_out, const a3_SpatialPose* spatialPose_in, const a3_SpatialPoseChannel channel, const a3_SpatialPoseEulerOrder order)
{
	if (mat_out && spatialPose_in)
	{
		// RST -> mat4
		// 
		// Scale then rotate then translate
		// RIGHT TO LEFT
		// M = T * R2 * R1 * R0 * S
		// (occurs like this)
		// M = T * ((R2 * R1 * R0) * S)
		// 
		//          tx
		//          ty
		//          tz
		//  0  0  0  1

		// Scale
		a3real3MulS(mat_out->m[0], spatialPose_in->scale.x);
		a3real3MulS(mat_out->m[1], spatialPose_in->scale.y);
		a3real3MulS(mat_out->m[2], spatialPose_in->scale.z);

		// Rotate
		a3real4x4SetRotateXYZ(mat_out->m, spatialPose_in->rotate_euler.x, spatialPose_in->rotate_euler.y, spatialPose_in->rotate_euler.z);

		// Translate
		mat_out->v3.xyz = spatialPose_in->translate;


		// Scale
		// x 0 0 0
		// 0 y 0 0
		// 0 0 z 0
		// 0 0 0 1

		// Rotx
		// 1  0  0
		// 0  c -s
		// 0  s  c
		// Rotz
		// c -s  0
		// s  c  0
		// 0  0  1
		// Roty
		// c  0  s
		// 0  1  0
		//-s  0  c

		// Translation
		// 1 0 0 x
		// 0 1 0 y
		// 0 0 1 z
		// 0 0 0 1

		return 1;
	}
	return -1;
}

// copy operation for single node pose
inline a3i32 a3spatialPoseCopy(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_in)
{
	if (spatialPose_out && spatialPose_in)
	{
		spatialPose_out->transform = spatialPose_in->transform;
		spatialPose_out->rotate_euler = spatialPose_in->rotate_euler;
		spatialPose_out->translate = spatialPose_in->translate;
		spatialPose_out->scale = spatialPose_in->scale;
	}
	return -1;
}

inline a3i32 a3spatialPoseConcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_lh, const a3_SpatialPose* spatialPose_rh, const a3boolean usingQuaternion)
{
	if (spatialPose_out && spatialPose_lh && spatialPose_rh)
	{
		//spatialPose_out->transform; NO, matrix has no data yet

		if (usingQuaternion)
		{
			spatialPose_out->rotate_quat;	//quat: (lh * rh) = (w_l + v_l) * (w_r + v_r)
										// (w_l * w_r - v_l . v_r) + (w_l * v_r + w_r * v_l + v_l X v_r)		(real) + (vector)
		}
		else
		{
			spatialPose_out->rotate_euler;	//Euler: concat with addition, validate(lh + rh) -> constrain sum to rotational domain
		}
										
		spatialPose_out->scale;		// concat with multiplicatoin, comp(lh * rh) -> component-wise
		spatialPose_out->translate;	// addition, (lh + rh)

		return 0;
	}
	return -1;
}

// Lerp
a3i32 a3spatialPoseLerp(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose0, const a3_SpatialPose* spatialPose1, const a3real u, const a3boolean usingQuaternion)
{
	if (spatialPose_out && spatialPose0 && spatialPose1)
	{
		// Right to left
		//spatialPose_out->transform; NO, matrix has no data yet
		if (usingQuaternion)
		{
			spatialPose_out->rotate_quat;	// Quat (3 options): slerp(q0, q1, u)
											// = (sin([1-t]y)q0 + sin(ty)q1) / sin(y)
											// where y = arccos(q0 . q1)  (and also should be theta)
											// Comes from applying exp_lerp() to a quaternion + trig
											//
											// 2nd option is straight lerp
											// loses unit length causing uniform scale 
											//	s = |q|^2
											// 
											// 3rd option: nlerp() = normalized lerp
											//	Is non-uniform, causes easing throughout rotation
											// 
		}
		else
		{
			spatialPose_out->rotate_euler;	// Euler: lerp(p0, p1, u) -> (p1 - p0)u + p0
		}
		spatialPose_out->scale;		// lerp is ok but really should exp_lerp() -> ((p1 * (p0^-1))^u)p0  unsubstantiated function
		spatialPose_out->translate;	// lerp(p0, p1, u)

		return 0;
	}
	return -1;
}

//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_SPATIALPOSE_INL
#endif	// __ANIMAL3D_SPATIALPOSE_H