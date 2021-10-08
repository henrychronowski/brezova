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
	
	a3_HierarchyState.c
	Implementation of transform hierarchy state.
*/

#include "../a3_HierarchyState.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define COMMENT '#'
#define END_FILE "EndOfFile"
#define SECTION_HEADER "[Header]"
#define SECTION_SEGMENT_NAME_HIERARCHY "[SegmentNames&Hierarchy]"
#define SECTION_BASE_POSITION "[BasePosition]"
#define SECTION_FIRST_CHAR '['



//-----------------------------------------------------------------------------

// initialize pose set given an initialized hierarchy and key pose count
a3i32 a3hierarchyPoseGroupCreate(a3_HierarchyPoseGroup *poseGroup_out, const a3_Hierarchy *hierarchy, const a3ui32 poseCount)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (poseGroup_out && hierarchy && !poseGroup_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 hierarchyPosesSize = sizeof(a3_HierarchyPose) * poseCount;
		a3ui32 spatialPosePoolSize = sizeof(a3_SpatialPose) * poseCount * hierarchy->numNodes;

		// allocate everything (one malloc)
		//??? = (...)malloc(sz);
		a3ui8* posePool = (a3ui8*) malloc(hierarchyPosesSize + spatialPosePoolSize);

		// set pointers
		poseGroup_out->hierarchy = hierarchy;
		poseGroup_out->hierarchyPoses = (a3_HierarchyPose*)posePool;
		poseGroup_out->poseCount = poseCount;
		poseGroup_out->spatialPosePool = (a3_SpatialPose*)(posePool + hierarchyPosesSize);
		poseGroup_out->spatialPoseCount = poseCount * hierarchy->numNodes;

		// reset all data
		for (a3ui32 i = 0; i < poseCount; i++)
		{
			poseGroup_out->hierarchyPoses[i].spatialPose = poseGroup_out->spatialPosePool + (i * hierarchy->numNodes);
		}

		for (a3ui32 j = 0; j < poseGroup_out->spatialPoseCount; j++)
		{
			a3spatialPoseReset(poseGroup_out->spatialPosePool + j);
		}

		// done
		return 1;
	}
	return -1;
}

// release pose set
a3i32 a3hierarchyPoseGroupRelease(a3_HierarchyPoseGroup *poseGroup)
{
	// validate param exists and is initialized
	if (poseGroup && poseGroup->hierarchy)
	{
		// release everything (one free)
		//free(???);
		free(poseGroup->hierarchyPoses);

		// reset pointers
		poseGroup->hierarchy = 0;
		poseGroup->hierarchyPoses = 0;
		poseGroup->spatialPosePool = 0;

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// initialize hierarchy state given an initialized hierarchy
a3i32 a3hierarchyStateCreate(a3_HierarchyState *state_out, const a3_Hierarchy *hierarchy)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (state_out && hierarchy && !state_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 nodeCount = hierarchy->numNodes;
		a3ui32 spatialPoseCount = nodeCount * 3;

		// allocate everything (one malloc)
		a3_SpatialPose* startPose = (a3_SpatialPose*)malloc(sizeof(a3_SpatialPose) * spatialPoseCount);
		
		// set pointers
		state_out->hierarchy = hierarchy;
		state_out->samplePose = (a3_HierarchyPose) {startPose};
		state_out->localSpacePose = (a3_HierarchyPose) {startPose + nodeCount};
		state_out->objectSpacePose = (a3_HierarchyPose) {startPose + nodeCount + nodeCount};

		// reset all data
		a3hierarchyPoseReset(&state_out->samplePose, nodeCount);
		a3hierarchyPoseReset(&state_out->localSpacePose, nodeCount);
		a3hierarchyPoseReset(&state_out->objectSpacePose, nodeCount);

		// done
		return 1;
	}
	return -1;
}

// release hierarchy state
a3i32 a3hierarchyStateRelease(a3_HierarchyState *state)
{
	// validate param exists and is initialized
	if (state && state->hierarchy)
	{
		// release everything (one free)
		//free(???);

		// reset pointers
		state->hierarchy = 0;

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// load HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadHTR(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->poseCount && hierarchy_out && !hierarchy_out->numNodes && resourceFilePath && *resourceFilePath)
	{
		FILE* inFile;

		inFile = fopen(resourceFilePath, "r");

		if(!inFile)
			return -1;

		a3byte* line = malloc(sizeof(a3byte) * 128);

		printf("Loading animation file...\n");

		while (strcmp(line, END_FILE) != 0)
		{
			a3i32 commentResult = a3CheckComment(line, inFile);
			if (strcmp(line, SECTION_HEADER) == 0)
				a3ReadHTRHeader(line, inFile, poseGroup_out, hierarchy_out);
			if(strcmp(line, SECTION_SEGMENT_NAME_HIERARCHY) == 0)
			{
				a3ReadHTRNamesAndHierarchy(line, inFile, hierarchy_out);
				a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, poseGroup_out->poseCount);
			}
			if(strcmp(line, SECTION_BASE_POSITION) == 0)
				a3ReadHTRBasePosition(line, inFile, poseGroup_out, hierarchy_out);
			if (line[0] == SECTION_FIRST_CHAR)
			{
				for (a3ui32 i = 1; i < poseGroup_out->poseCount -1; i++)
				{
					line = strtok(line, "[");
					line[strcspn(line, "]")] = '\0';
					a3ReadHTRBonePoses(line, inFile, poseGroup_out, hierarchy_out);
				}
			}
		}
		fclose(inFile);
		printf("Finished!\n");
		free(line);
		return 1;
	}
	return -1;
}

// load BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadBVH(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->poseCount && hierarchy_out && !hierarchy_out->numNodes && resourceFilePath && *resourceFilePath)
	{

	}
	return -1;
}


a3i32 a3CheckComment(a3byte* line, FILE* file)
{
	if (file)
	{
		a3byte* buffer = malloc(sizeof(a3byte) * 128);
		
		do 
		{
			fgets(buffer, 128, file);
		} while (buffer != END_FILE && (buffer[0] == '\n' || buffer[0] == COMMENT));

		if (buffer != END_FILE)
		{
			buffer[strcspn(buffer, "\n")] = '\0';
			strcpy_s(line, 128, buffer);
			free(buffer);
			return 1;
		}

		free(buffer);
	}
		
	return -1;
}

a3i32 a3ReadHTRHeader(a3byte* line, FILE* file, a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out)
{
	if (file)
	{
		a3byte* key = malloc(sizeof(a3byte) * 128);
		a3byte* value = malloc(sizeof(a3byte) * 128);

		while (true) // Henry made me do this
		{
			do
			{
				if (key[0] == COMMENT)
					fgets(key, 128, file);
				fscanf(file, "%s", key);
			} while (key != END_FILE && key[0] == COMMENT);

			if (strcmp(key, "FileType") == 0)
			{
				fscanf(file, "%s", value);
			}
			else if (strcmp(key, "DataType") == 0)
			{
				fscanf(file, "%s", value);
			}
			else if (strcmp(key, "FileVersion") == 0)
			{
				fscanf(file, "%s", value);

			}
			else if (strcmp(key, "NumSegments") == 0)
			{
				fscanf(file, "%s", value);
				hierarchy_out->numNodes = atoi(value);
			}
			else if (strcmp(key, "NumFrames") == 0)
			{
				fscanf(file, "%s", value);
				poseGroup_out->poseCount = atoi(value) + 1;
			}
			else if (strcmp(key, "DataFrameRate") == 0)
			{
				fscanf(file, "%s", value);
			}
			else if (strcmp(key, "EulerRotationOrder") == 0)
			{
				fscanf(file, "%s", value);
				a3GetEulerOrderFromValue(value);
			}
			else if (strcmp(key, "CalibrationUnits") == 0)
			{
				fscanf(file, "%s", value);
			}
			else if (strcmp(key, "RotationUnits") == 0)
			{
				fscanf(file, "%s", value);
			}
			else if (strcmp(key, "GlobalAxisofGravity") == 0)
			{
				fscanf(file, "%s", value);
			}
			else if (strcmp(key, "BoneLengthAxis") == 0)
			{
				fscanf(file, "%s", value);
			}
			else if (strcmp(key, "ScaleFactor") == 0)
			{
				fscanf(file, "%s", value);
			}
			else
			{
				strcpy_s(line, 128, key);
				break;
			}
		}

		free(key);
		free(value);
		return 1;
	}

	return -1;
}

a3i32 a3ReadHTRNamesAndHierarchy(a3byte* line, FILE* file, a3_Hierarchy* hierarchy_out)
{
	if (file)
	{
		a3byte* key = malloc(sizeof(a3byte) * 128);
		a3byte* value = malloc(sizeof(a3byte) * 128);

		// indices of joints, their parents and branching joints
		a3ui32 jointIndex = 0;
		a3i32 jointParentIndex = -1;

		// initialize hierarchy
		a3hierarchyCreate(hierarchy_out, hierarchy_out->numNodes, 0);

		while (true)
		{
			do
			{
				if (key[0] == COMMENT)
					fgets(key, 128, file);
				fscanf(file, "%s", key);
			} while (key != END_FILE && key[0] == COMMENT);

			if (strcmp(key, SECTION_BASE_POSITION) == 0)
			{
				strcpy_s(line, 128, key);
				break;
			}
			else
			{
				fscanf(file, "%s", value);

				if (strcmp(value, "GLOBAL") == 0)
				{
					jointParentIndex = -1;
				}
				else
				{
					jointParentIndex = a3hierarchyGetNodeIndex(hierarchy_out, value);
				}

				a3hierarchySetNode(hierarchy_out, jointIndex, jointParentIndex, key);
				jointIndex++;
			}
		}

		free(key);
		free(value);
		return 1;
	}

	return -1;
}

a3i32 a3ReadHTRBasePosition(a3byte* line, FILE* file, a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out)
{
	if (file)
	{
		a3byte* key = malloc(sizeof(a3byte) * 128);
		a3f32* tX = malloc(sizeof(a3f32));
		a3f32* tY = malloc(sizeof(a3f32));
		a3f32* tZ = malloc(sizeof(a3f32));
		a3f32* rX = malloc(sizeof(a3f32));
		a3f32* rY = malloc(sizeof(a3f32));
		a3f32* rZ = malloc(sizeof(a3f32));
		a3f32* boneLength = malloc(sizeof(a3f32));

		// indices of joints, their parents and branching joints
		a3ui32 jointIndex = 0;
		a3ui32 p = 0;

		a3_SpatialPose* spatialPose = 0;

		// initialize hierarchy
		//a3hierarchyCreate(hierarchy_out, hierarchy_out->numNodes, 0);

		for (size_t i = 0; i < hierarchy_out->numNodes; i++)
		{
			do
			{
				if (key[0] == COMMENT)
					fgets(key, 128, file);
				fscanf(file, "%s", key);
			} while (key != END_FILE && key[0] == COMMENT);



			jointIndex = a3hierarchyGetNodeIndex(hierarchy_out, key);
			spatialPose = poseGroup_out->hierarchyPoses[p].spatialPose + jointIndex;

			fscanf(file, "%f %f %f %f %f %f %f", tX, tY, tZ, rX, rY, rZ, boneLength);
			a3spatialPoseSetRotation(spatialPose, *rX, *rY, *rZ);
			a3spatialPoseSetTranslation(spatialPose, *tX, *tY, *tZ);


		}

		strcpy_s(line, 128, key);

		free(key);
		free(tX);
		free(tY);
		free(tZ);
		free(rX);
		free(rY);
		free(rZ);
		free(boneLength);
		return 1;
	}

	return -1;
}

a3i32 a3ReadHTRBonePoses(a3byte* line, FILE* file, a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out)
{
	if (file)
	{
		a3byte* key = malloc(sizeof(a3byte) * 128);
		a3f32* tX = malloc(sizeof(a3f32));
		a3f32* tY = malloc(sizeof(a3f32));
		a3f32* tZ = malloc(sizeof(a3f32));
		a3f32* rX = malloc(sizeof(a3f32));
		a3f32* rY = malloc(sizeof(a3f32));
		a3f32* rZ = malloc(sizeof(a3f32));
		a3f32* boneLength = malloc(sizeof(a3f32));

		a3ui32 jointIndex = a3hierarchyGetNodeIndex(hierarchy_out, line);
		a3ui32 offset = 1;
		a3_SpatialPose* spatialPose = 0;

		while(true)
		{
			do
			{
				if (key[0] == COMMENT)
					fgets(key, 128, file);
				fscanf(file, "%s", key);
			} while (key != END_FILE && key[0] == COMMENT);

			if (key[0] == SECTION_FIRST_CHAR)
			{
				break;
			}

			if (strcmp(key, END_FILE) == 0 || strcmp(line, END_FILE) == 0)
				return 1;

			spatialPose = poseGroup_out->hierarchyPoses[atoi(key) + offset].spatialPose + jointIndex;

			fscanf(file, "%f %f %f %f %f %f %f", tX, tY, tZ, rX, rY, rZ, boneLength);
			a3spatialPoseSetRotation(spatialPose, *rX, *rY, *rZ);
			a3spatialPoseSetTranslation(spatialPose, *tX, *tY, *tZ);
		
		}
		
		strcpy_s(line, 128, key);

		free(key);
		free(tX);
		free(tY);
		free(tZ);
		free(rX);
		free(rY);
		free(rZ);
		free(boneLength);
		return 1;
	}

	return -1;
}

a3_SpatialPoseEulerOrder a3GetEulerOrderFromValue(a3byte* value)
{
	if(value == "XYZ")
		return a3poseEulerOrder_xyz;
	if(value == "XZY")
		return a3poseEulerOrder_xzy;
	if(value == "YXZ")
		return a3poseEulerOrder_yxz;
	if(value == "YZX")
		return a3poseEulerOrder_yzx;
	if(value == "ZXY")
		return a3poseEulerOrder_zxy;
	if(value == "ZYX")
		return a3poseEulerOrder_zyx;

	return a3poseEulerOrder_xyz;
}

//-----------------------------------------------------------------------------
