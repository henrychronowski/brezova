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
	
	passTangentBasis_skin_transform_vs4x.glsl
	Calculate and pass tangent basis with skinning.
*/

#version 450

layout (location = 0) in vec4 aPosition;
layout (location = 2) in vec4 aNormal;
layout (location = 8) in vec4 aTexcoord;
layout (location = 10) in vec4 aTangent;
layout (location = 11) in vec4 aBitangent;
// Skinning attributes
// w = weights (location = 1)
// j = joint (location = 7)

// Rigid bind: 1 influence
// layout (location = 7) in int aBindIndex;

// Smooth bind: 4 influences at different weights
layout (location = 1) in vec4 aBindWeights;
layout (location = 7) in ivec4 aBindIndex;

uniform mat4 uP;
uniform mat4 uMV, uMV_nrm;
uniform mat4 uAtlas;

#define MAX_JOINTS 128
#define dquat mat2x4

uniform ubTransformBlend
{
	mat4 uSkinMat[MAX_JOINTS];	// S
	dquat uSkinDQ[MAX_JOINTS];	// Q

};

out vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
};

flat out int vVertexID;
flat out int vInstanceID;

// Rigid bind v = s[j] * v
vec4 skinRigidLinear(in vec4 v, in int j)
{
	return (uSkinMat[j] * v);
}

// Smooth bind v = sum(w[i] * (s[j[i]] * v)) where i=[0...3]
vec4 skinSmoothLinear(in vec4 v, in ivec4 j, in vec4 w)
{
	vec4 v_out = vec4(0.0);

	v_out += w[0] * ( uSkinMat[j[0]] * v);
	v_out += w[1] * ( uSkinMat[j[1]] * v);
	v_out += w[2] * ( uSkinMat[j[2]] * v);
	v_out += w[3] * ( uSkinMat[j[3]] * v);

	return v_out;
}

mat4 convDQ2Mat4(in dquat dq)
{
	// TODO: implement
	mat4 m_out = mat4(1.0);
	return m_out;
}

// Smooth dual quaternion blend (equivalent of nlerp for dual quaternions)
mat4 skinSmoothDQDLB(in ivec4 j, in vec4 w)
{
	// Q' = sum(w[i] * Q[j[i]]) where i=[0...3]
	// M' = convert(Q'/ |Q'|)
	dquat dq = dquat(0.0);

	dq += w[0] * uSkinDQ[j[0]];
	dq += w[1] * uSkinDQ[j[1]];
	dq += w[2] * uSkinDQ[j[2]];
	dq += w[3] * uSkinDQ[j[3]];

	return convDQ2Mat4(dq / length(dq[0]));
}

void main()
{
	// DUMMY OUTPUT: directly assign input position to output position
	//	gl_Position = aPosition;

	// 

	vTangentBasis_view = uMV_nrm * mat4(
		//vec4(aTangent.xyz, 0.0), 
		//vec4(aBitangent.xyz, 0.0), 
		//vec4(aNormal.xyz, 0.0), 
		//skinRigidLinear(vec4(aTangent.xyz, 0.0), aBindIndex[0]), 
		//skinRigidLinear(vec4(aBitangent.xyz, 0.0), aBindIndex[0]), 
		//skinRigidLinear(vec4(aNormal.xyz, 0.0), aBindIndex[0]), 
		skinSmoothLinear(vec4(aTangent.xyz, 0.0), aBindIndex, aBindWeights), 
		skinSmoothLinear(vec4(aBitangent.xyz, 0.0), aBindIndex, aBindWeights), 
		skinSmoothLinear(vec4(aNormal.xyz, 0.0), aBindIndex, aBindWeights), 
		vec4(0.0));
	vTangentBasis_view[3] = uMV * 
		//aPosition;
		//skinRigidLinear(aPosition, aBindIndex[0]);
		skinSmoothLinear(aPosition, aBindIndex, aBindWeights);
	gl_Position = uP * vTangentBasis_view[3];
	
	vTexcoord_atlas = uAtlas * aTexcoord;

	vVertexID = gl_VertexID;
	vInstanceID = gl_InstanceID;
}
