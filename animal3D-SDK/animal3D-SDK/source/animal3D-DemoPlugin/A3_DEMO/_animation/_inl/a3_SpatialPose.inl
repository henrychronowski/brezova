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
		spatialPose->rotation.x = rx_degrees;
		spatialPose->rotation.y = ry_degrees;
		spatialPose->rotation.z = rz_degrees;
	}
	return -1;
}

// scale
inline a3i32 a3spatialPoseSetScale(a3_SpatialPose* spatialPose, const a3f32 sx, const a3f32 sy, const a3f32 sz)
{
	if (spatialPose)
	{
		spatialPose->scale.x = sx;
		spatialPose->scale.y = sy;
		spatialPose->scale.z = sz;
	}
	return -1;
}

// translation
inline a3i32 a3spatialPoseSetTranslation(a3_SpatialPose* spatialPose, const a3f32 tx, const a3f32 ty, const a3f32 tz)
{
	if (spatialPose)
	{
		spatialPose->translate.x = tx;
		spatialPose->translate.y = ty;
		spatialPose->translate.z = tz;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// reset single node pose
inline a3i32 a3spatialPoseReset(a3_SpatialPose* spatialPose)
{
	if (spatialPose)
	{
		spatialPose->transform = a3mat4_identity;
		spatialPose->rotation = a3vec3_zero;
		spatialPose->translate = a3vec3_zero;
		spatialPose->scale = a3vec3_one;
	}
	return -1;
}

inline a3i32 a3spatialPoseInit(a3_SpatialPose* spatialPose, a3mat4 transform, a3vec3 rotation, a3vec3 translate, a3vec3 scale)
{
	if (spatialPose)
	{
		spatialPose->transform = transform;
		spatialPose->rotation = rotation;
		spatialPose->translate = translate;
		spatialPose->scale = scale;
	}
}

// convert single node pose to matrix
inline a3i32 a3spatialPoseConvert(a3mat4* mat_out, const a3_SpatialPose* spatialPose_in, const a3_SpatialPoseChannel channel, const a3_SpatialPoseEulerOrder order)
{
	if (mat_out && spatialPose_in)
	{
		// Translate
		mat_out->v3.xyz = spatialPose_in->translate;

		// Check euler angle order
		switch (order)
		{
			case a3poseEulerOrder_xyz:
				// Rotate
				a3real4x4SetRotateX(mat_out->m, spatialPose_in->rotation.x);
				a3real4x4SetRotateY(mat_out->m, spatialPose_in->rotation.y);
				a3real4x4SetRotateZ(mat_out->m, spatialPose_in->rotation.z);
			break;
			case a3poseEulerOrder_zxy:
				// Rotate
				a3real4x4SetRotateZ(mat_out->m, spatialPose_in->rotation.z);
				a3real4x4SetRotateX(mat_out->m, spatialPose_in->rotation.x);
				a3real4x4SetRotateY(mat_out->m, spatialPose_in->rotation.y);
			break;
			case a3poseEulerOrder_yxz:
				// Rotate
				a3real4x4SetRotateY(mat_out->m, spatialPose_in->rotation.y);
				a3real4x4SetRotateZ(mat_out->m, spatialPose_in->rotation.z);
				a3real4x4SetRotateX(mat_out->m, spatialPose_in->rotation.x);
			break;
			case a3poseEulerOrder_zyx:
				// Rotate
				a3real4x4SetRotateZ(mat_out->m, spatialPose_in->rotation.z);
				a3real4x4SetRotateY(mat_out->m, spatialPose_in->rotation.y);
				a3real4x4SetRotateX(mat_out->m, spatialPose_in->rotation.x);
			break;
			case a3poseEulerOrder_yzx:
				// Rotate
				a3real4x4SetRotateY(mat_out->m, spatialPose_in->rotation.y);
				a3real4x4SetRotateZ(mat_out->m, spatialPose_in->rotation.z);
				a3real4x4SetRotateX(mat_out->m, spatialPose_in->rotation.x);
			break;
			case a3poseEulerOrder_xzy:
				// Rotate
				a3real4x4SetRotateX(mat_out->m, spatialPose_in->rotation.x);
				a3real4x4SetRotateZ(mat_out->m, spatialPose_in->rotation.z);
				a3real4x4SetRotateY(mat_out->m, spatialPose_in->rotation.y);
			break;
		}

		// Scale
		a3real3MulS(mat_out->m[0], spatialPose_in->scale.x);
		a3real3MulS(mat_out->m[1], spatialPose_in->scale.y);
		a3real3MulS(mat_out->m[2], spatialPose_in->scale.z);

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
		spatialPose_out->rotation = spatialPose_in->rotation;
		spatialPose_out->translate = spatialPose_in->translate;
		spatialPose_out->scale = spatialPose_in->scale;
	}
	return -1;
}

inline a3i32 a3spatialPoseConcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_lh, const a3_SpatialPose* spatialPose_rh)
{
	if (spatialPose_out && spatialPose_lh && spatialPose_rh)
	{
		// Concat rotation
		a3real3Sum(spatialPose_out->rotation.v, spatialPose_lh->rotation.v, spatialPose_rh->rotation.v);

		// Concat scale
		spatialPose_out->scale = spatialPose_lh->scale;
		a3real3MulComp(spatialPose_out->scale.v, spatialPose_rh->scale.v);

		// Concat translation
		a3real3Sum(spatialPose_out->translate.v, spatialPose_lh->translate.v, spatialPose_rh->translate.v);

		return 1;
	}
	return -1;
}

// Lerp
a3i32 a3spatialPoseLerp(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose0, const a3_SpatialPose* spatialPose1, const a3real u)
{
	if (spatialPose_out && spatialPose0 && spatialPose1)
	{
		// Right to left
		//spatialPose_out->transform; NO, matrix has no data yet
		spatialPose_out->rotation;	// Euler: lerp(p0, p1, u) -> (p1 - p0)u + p0
		spatialPose_out->scale;		// lerp is ok but really should exp_lerp() -> ((p1 * (p0^-1))^u)p0  unsubstantiated function
		spatialPose_out->translate;	// lerp(p0, p1, u)

		a3real3Lerp(spatialPose_out->rotation.v, spatialPose0->rotation.v, spatialPose1->rotation.v, u);

		// TODO: Need to change to explerp
		a3real3Lerp(spatialPose_out->scale.v, spatialPose0->scale.v, spatialPose1->scale.v, u);

		a3real3Lerp(spatialPose_out->scale.v, spatialPose0->translate.v, spatialPose1->translate.v, u);

		return 0;
	}
	return -1;
}

//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_SPATIALPOSE_INL
#endif	// __ANIMAL3D_SPATIALPOSE_H