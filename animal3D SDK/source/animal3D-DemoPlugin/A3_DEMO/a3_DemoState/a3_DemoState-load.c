/*
	Copyright 2011-2021 Daniel S. Buckstein

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
	
	a3_DemoState_loading.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     LOADING in this file.                    ***
	****************************************************
*/

#include "../_a3_demo_utilities/a3_DemoMacros.h"


//-----------------------------------------------------------------------------

// uncomment this to link extension library (if available)
//#define A3_USER_ENABLE_EXTENSION

// **WARNING: FOR TESTING/COMPARISON ONLY, DO NOT USE IN DELIVERABLE BUILDS**
// uncomment this to allow shader decoding (if available)
#define A3_USER_ENABLE_SHADER_DECODING


//-----------------------------------------------------------------------------

#ifdef A3_USER_ENABLE_SHADER_DECODING
// override shader loading function name before including
#define a3shaderCreateFromFileList a3shaderCreateFromFileListEncoded
#endif	// A3_USER_ENABLE_SHADER_DECODING


#ifdef _WIN32
#ifdef A3_USER_ENABLE_EXTENSION
// force link extension lib
#pragma comment(lib,"animal3D-A3DX.lib")
#endif	// A3_USER_ENABLE_EXTENSION
#ifdef A3_USER_ENABLE_SHADER_DECODING
// add lib for shader decoding
#pragma comment(lib,"animal3D-UtilityLib.lib")
#endif	// A3_USER_ENABLE_SHADER_DECODING
#endif	// _WIN32


// define resource directories
#define A3_DEMO_RES_DIR	"../../../../resource/"
#define A3_DEMO_GLSL	A3_DEMO_RES_DIR"glsl/"
#define A3_DEMO_TEX		A3_DEMO_RES_DIR"tex/"
#define A3_DEMO_OBJ		A3_DEMO_RES_DIR"obj/"

// define resource subdirectories
#define A3_DEMO_VS		A3_DEMO_GLSL"4x/vs/"
#define A3_DEMO_TS		A3_DEMO_GLSL"4x/ts/"
#define A3_DEMO_GS		A3_DEMO_GLSL"4x/gs/"
#define A3_DEMO_FS		A3_DEMO_GLSL"4x/fs/"
#define A3_DEMO_CS		A3_DEMO_GLSL"4x/cs/"


//-----------------------------------------------------------------------------

#include "../a3_DemoState.h"

#include <stdio.h>


//-----------------------------------------------------------------------------
// GENERAL UTILITIES

a3real4x4r a3demo_setAtlasTransform_internal(a3real4x4p m_out,
	const a3ui16 atlasWidth, const a3ui16 atlasHeight,
	const a3ui16 subTexturePosX, const a3ui16 subTexturePosY,
	const a3ui16 subTextureWidth, const a3ui16 subTextureHeight,
	const a3ui16 subTextureBorderPadding, const a3ui16 subTextureAdditionalPadding)
{
	a3real4x4SetIdentity(m_out);
	m_out[0][0] = (a3real)(subTextureWidth) / (a3real)(atlasWidth);
	m_out[1][1] = (a3real)(subTextureHeight) / (a3real)(atlasHeight);
	m_out[3][0] = (a3real)(subTexturePosX + subTextureBorderPadding + subTextureAdditionalPadding) / (a3real)(atlasWidth);
	m_out[3][1] = (a3real)(subTexturePosY + subTextureBorderPadding + subTextureAdditionalPadding) / (a3real)(atlasHeight);
	return m_out;
}


// initialize texture set
inline void a3demo_initTextureSet(a3_Texture const* textureSet[4],
	a3_Texture const* tex_dm, a3_Texture const* tex_sm, a3_Texture const* tex_nm, a3_Texture const* tex_hm)
{
	textureSet[0] = tex_dm;
	textureSet[1] = tex_sm;
	textureSet[2] = tex_nm;
	textureSet[3] = tex_hm;
}


//-----------------------------------------------------------------------------
// uniform helpers

#define a3demo_getUniformLocation(demoProgram, handleName, getLocFunc) (demoProgram->handleName = getLocFunc(demoProgram->program, #handleName))
#define a3demo_getUniformLocationValid(demoProgram, handleName, getLocFunc) if (a3demo_getUniformLocation(demoProgram, handleName, getLocFunc) >= 0)
#define a3demo_setUniformDefault(demoProgram, handleName, sendFunc, type, value) \
	a3demo_getUniformLocationValid(demoProgram, handleName, a3shaderUniformGetLocation) \
		sendFunc(type, demoProgram->handleName, 1, value)
#define a3demo_setUniformDefaultMat(demoProgram, handleName, sendFunc, type, value) \
	a3demo_getUniformLocationValid(demoProgram, handleName, a3shaderUniformGetLocation) \
		sendFunc(type, 0, demoProgram->handleName, 1, value)
#define a3demo_setUniformDefaultBlock(demoProgram, handleName, value) \
	a3demo_getUniformLocationValid(demoProgram, handleName, a3shaderUniformBlockGetLocation) \
		a3shaderUniformBlockBind(demoProgram->program, demoProgram->handleName, value)

#define a3demo_setUniformDefaultFloat(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendFloat, a3unif_single, value)
#define a3demo_setUniformDefaultVec2(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendFloat, a3unif_vec2, value)
#define a3demo_setUniformDefaultVec3(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendFloat, a3unif_vec3, value)
#define a3demo_setUniformDefaultVec4(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendFloat, a3unif_vec4, value)
#define a3demo_setUniformDefaultDouble(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendDouble, a3unif_single, value)
#define a3demo_setUniformDefaultDVec2(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendDouble, a3unif_vec2, value)
#define a3demo_setUniformDefaultDVec3(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendDouble, a3unif_vec3, value)
#define a3demo_setUniformDefaultDVec4(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendDouble, a3unif_vec4, value)
#define a3demo_setUniformDefaultInteger(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendInt, a3unif_single, value)
#define a3demo_setUniformDefaultIVec2(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendInt, a3unif_vec2, value)
#define a3demo_setUniformDefaultIVec3(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendInt, a3unif_vec3, value)
#define a3demo_setUniformDefaultIVec4(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendInt, a3unif_vec4, value)
#define a3demo_setUniformDefaultMat2(demoProgram, handleName) a3demo_setUniformDefaultMat(demoProgram, handleName, a3shaderUniformSendFloatMat, a3unif_mat2, a3mat2_identity.mm)
#define a3demo_setUniformDefaultMat3(demoProgram, handleName) a3demo_setUniformDefaultMat(demoProgram, handleName, a3shaderUniformSendFloatMat, a3unif_mat3, a3mat3_identity.mm)
#define a3demo_setUniformDefaultMat4(demoProgram, handleName) a3demo_setUniformDefaultMat(demoProgram, handleName, a3shaderUniformSendFloatMat, a3unif_mat4, a3mat4_identity.mm)


//-----------------------------------------------------------------------------
// LOADING

// utility to load geometry
void a3demo_loadGeometry(a3_DemoState *demoState)
{
	// tmp descriptor for loaded model
	typedef struct a3_TAG_DEMOSTATELOADEDMODEL {
		const a3byte *modelFilePath;
		const a3real *transform;
		a3_ModelLoaderFlag flag;
	} a3_DemoStateLoadedModel;

	// static model transformations
	static const a3mat4 downscale20x_y2z_x2y = {
		 0.00f, +0.05f,  0.00f,  0.00f,
		 0.00f,  0.00f, +0.05f,  0.00f,
		+0.05f,  0.00f,  0.00f,  0.00f,
		 0.00f,  0.00f,  0.00f, +1.00f,
	};
	static const a3mat4 scale1x_z2y = {
		+0.05f,  0.00f,  0.00f,  0.00f,
		 0.00f,  0.00f, -0.05f,  0.00f,
		 0.00f, +0.05f,  0.00f,  0.00f,
		 0.00f,  0.00f,  0.00f, +1.00f,
	};

	// pointer to shared vbo/ibo
	a3_VertexBuffer *vbo_ibo;
	a3_VertexArrayDescriptor *vao;
	a3_VertexDrawable *currentDrawable;
	a3ui32 sharedVertexStorage = 0, sharedIndexStorage = 0;
	a3ui32 numVerts = 0;
	a3ui32 i, j;


	// file streaming (if requested)
	a3_FileStream fileStream[1] = { 0 };
	const a3byte *const geometryStream = "./data/gpro_base_geom.dat";

	// geometry data
	a3_GeometryData displayShapesData[4] = { 0 };
	a3_GeometryData proceduralShapesData[7] = { 0 };
	a3_GeometryData loadedModelsData[1] = { 0 };
	a3_GeometryData morphingModelsData[1][5] = { 0 };
	const a3ui32 displayShapesCount = a3demoArrayLen(displayShapesData);
	const a3ui32 proceduralShapesCount = a3demoArrayLen(proceduralShapesData);
	const a3ui32 loadedModelsCount = a3demoArrayLen(loadedModelsData);
	const a3ui32 morphingModelsCount = a3demoArrayLen(morphingModelsData);
	const a3ui32 morphingModelsMaxTargets = a3demoArrayLen(*morphingModelsData);

	// common index format
	a3_IndexFormatDescriptor sceneCommonIndexFormat[1] = { 0 };
	a3ui32 bufferOffset, *const bufferOffsetPtr = &bufferOffset;

	// morphing vertex format
	a3_VertexFormatDescriptor morphFormat[1] = { 0 };
	a3_VertexAttributeDescriptor morphAttrib[16] = { 0 };
	a3ui32 const morphAttribCount = sizeof(morphAttrib) / sizeof(a3_VertexAttributeDescriptor);

	// procedural scene objects
	// attempt to load stream if requested
	if (demoState->streaming && a3fileStreamOpenRead(fileStream, geometryStream))
	{
		// read from stream

		// static display objects
		for (i = 0; i < displayShapesCount; ++i)
			a3fileStreamReadObject(fileStream, displayShapesData + i, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// procedurally-generated objects
		for (i = 0; i < proceduralShapesCount; ++i)
			a3fileStreamReadObject(fileStream, proceduralShapesData + i, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// loaded model objects
		for (i = 0; i < loadedModelsCount; ++i)
			a3fileStreamReadObject(fileStream, loadedModelsData + i, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// morphing objects
		for (i = 0; i < morphingModelsCount; ++i)
			for (j = 0; j < morphingModelsMaxTargets; ++j)
				a3fileStreamReadObject(fileStream, morphingModelsData[i] + j, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// done
		a3fileStreamClose(fileStream);
	}
	// not streaming or stream doesn't exist
	else if (!demoState->streaming || a3fileStreamOpenWrite(fileStream, geometryStream))
	{
		// create new data
		a3_ProceduralGeometryDescriptor displayShapes[a3demoArrayLen(displayShapesData)] = { a3geomShape_none };
		a3_ProceduralGeometryDescriptor proceduralShapes[a3demoArrayLen(proceduralShapesData)] = { a3geomShape_none };
		const a3_DemoStateLoadedModel loadedShapes[a3demoArrayLen(loadedModelsData)] = {
			{ A3_DEMO_OBJ"teapot/teapot.obj", downscale20x_y2z_x2y.mm, a3model_calculateVertexTangents },
		};
		const a3_DemoStateLoadedModel morphingShapes[a3demoArrayLen(morphingModelsData)][a3demoArrayLen(*morphingModelsData)] = {
			{
				{ A3_DEMO_OBJ"teapot/morph/teapot_base.obj", downscale20x_y2z_x2y.mm, a3model_calculateVertexTangents },
				{ A3_DEMO_OBJ"teapot/morph/teapot_scale.obj", downscale20x_y2z_x2y.mm, a3model_calculateVertexTangents },
				{ A3_DEMO_OBJ"teapot/morph/teapot_scale_x.obj", downscale20x_y2z_x2y.mm, a3model_calculateVertexTangents },
				{ A3_DEMO_OBJ"teapot/morph/teapot_scale_y.obj", downscale20x_y2z_x2y.mm, a3model_calculateVertexTangents },
				{ A3_DEMO_OBJ"teapot/morph/teapot_scale_z.obj", downscale20x_y2z_x2y.mm, a3model_calculateVertexTangents },
			},
		};

		// static scene procedural objects
		//	(axes, grid)
		a3proceduralCreateDescriptorAxes(displayShapes + 0, a3geomFlag_wireframe, 0.0f, 1);
		a3proceduralCreateDescriptorPlane(displayShapes + 1, a3geomFlag_wireframe, a3geomAxis_default, 20.0f, 20.0f, 20, 20);
		a3proceduralCreateDescriptorPyramid(displayShapes + 2, a3geomFlag_wireframe, a3geomAxis_default, 1.0f, 1.0f);
		a3proceduralCreateDescriptorDiamond(displayShapes + 3, a3geomFlag_wireframe, a3geomAxis_default, 0.5f, 1.0f, 4, 1);
		for (i = 0; i < displayShapesCount; ++i)
		{
			a3proceduralGenerateGeometryData(displayShapesData + i, displayShapes + i, 0);
			a3fileStreamWriteObject(fileStream, displayShapesData + i, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
		}

		// other procedurally-generated objects
		a3proceduralCreateDescriptorPlane(proceduralShapes + 0, a3geomFlag_tangents, a3geomAxis_default, 1.0f, 1.0f, 2, 2);
		a3proceduralCreateDescriptorBox(proceduralShapes + 1, a3geomFlag_tangents, 1.0f, 1.0f, 1.0f, 1, 1, 1);
		a3proceduralCreateDescriptorSphere(proceduralShapes + 2, a3geomFlag_tangents, a3geomAxis_default, 1.0f, 32, 24);
		a3proceduralCreateDescriptorCylinder(proceduralShapes + 3, a3geomFlag_tangents, a3geomAxis_x, 1.0f, 1.0f, 32, 4, 4);
		a3proceduralCreateDescriptorCapsule(proceduralShapes + 4, a3geomFlag_tangents, a3geomAxis_x, 1.0f, 1.0f, 32, 12, 4);
		a3proceduralCreateDescriptorTorus(proceduralShapes + 5, a3geomFlag_tangents, a3geomAxis_x, 1.0f, 0.25f, 32, 24);
		a3proceduralCreateDescriptorCone(proceduralShapes + 6, a3geomFlag_tangents, a3geomAxis_x, 1.0f, 1.0, 32, 1, 1);
		for (i = 0; i < proceduralShapesCount; ++i)
		{
			a3proceduralGenerateGeometryData(proceduralShapesData + i, proceduralShapes + i, 0);
			a3fileStreamWriteObject(fileStream, proceduralShapesData + i, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
		}

		// objects loaded from mesh files
		for (i = 0; i < loadedModelsCount; ++i)
		{
			a3modelLoadOBJ(loadedModelsData + i, loadedShapes[i].modelFilePath, loadedShapes[i].flag, loadedShapes[i].transform);
			a3fileStreamWriteObject(fileStream, loadedModelsData + i, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
		}

		// morphing objects
		for (i = 0; i < morphingModelsCount; ++i)
			for (j = 0; j < morphingModelsMaxTargets; ++j)
			{
				a3modelLoadOBJ(morphingModelsData[i] + j, morphingShapes[i][j].modelFilePath, morphingShapes[i][j].flag, morphingShapes[i][j].transform);
				a3fileStreamWriteObject(fileStream, morphingModelsData[i] + j, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
			}

		// done
		a3fileStreamClose(fileStream);
	}


	// GPU data upload process: 
	//	- determine storage requirements
	//	- allocate buffer
	//	- create vertex arrays using unique formats
	//	- create drawable and upload data

	// get storage size
	sharedVertexStorage = numVerts = 0;
	for (i = 0; i < displayShapesCount; ++i)
	{
		sharedVertexStorage += a3geometryGetVertexBufferSize(displayShapesData + i);
		numVerts += displayShapesData[i].numVertices;
	}
	for (i = 0; i < proceduralShapesCount; ++i)
	{
		sharedVertexStorage += a3geometryGetVertexBufferSize(proceduralShapesData + i);
		numVerts += proceduralShapesData[i].numVertices;
	}
	for (i = 0; i < loadedModelsCount; ++i)
	{
		sharedVertexStorage += a3geometryGetVertexBufferSize(loadedModelsData + i);
		numVerts += loadedModelsData[i].numVertices;
	}


	// morph targets setup
	//	- add required data from each model to new data descriptor
	{
		a3_VertexAttributeDescriptor* morphAttribPtr;
		a3_VertexAttributeName attribName;
		a3ui32 morphModelIndex = 0;

		for (i = 0, morphAttribPtr = morphAttrib, attribName = a3attrib_user00; i < morphingModelsMaxTargets; ++i)
			for (j = 0; j < 3; ++j, ++morphAttribPtr, ++attribName)
				// create descriptors for position, normal and tangent
				a3vertexAttribCreateDescriptor(morphAttribPtr, (a3_VertexAttributeName)attribName, a3attrib_vec3);
		// the final attribute will be for texture coordinates
		a3vertexAttribCreateDescriptor(morphAttribPtr, a3attrib_user15, a3attrib_vec2);

		// make format
		a3vertexFormatCreateDescriptor(morphFormat, morphAttrib, morphAttribCount);
		for (i = 0; i < morphingModelsCount; ++i)
		{
			sharedVertexStorage += a3vertexFormatGetStorageSpaceRequired(morphFormat, morphingModelsData[i]->numVertices);
			numVerts += morphingModelsData[i]->numVertices;
		}
	}


	// common index format required for shapes that share vertex formats
	a3geometryCreateIndexFormat(sceneCommonIndexFormat, numVerts);
	sharedIndexStorage = 0;
	for (i = 0; i < displayShapesCount; ++i)
		sharedIndexStorage += a3indexFormatGetStorageSpaceRequired(sceneCommonIndexFormat, displayShapesData[i].numIndices);
	for (i = 0; i < proceduralShapesCount; ++i)
		sharedIndexStorage += a3indexFormatGetStorageSpaceRequired(sceneCommonIndexFormat, proceduralShapesData[i].numIndices);
	for (i = 0; i < loadedModelsCount; ++i)
		sharedIndexStorage += a3indexFormatGetStorageSpaceRequired(sceneCommonIndexFormat, loadedModelsData[i].numIndices);
	for (i = 0; i < morphingModelsCount; ++i)
		sharedIndexStorage += a3indexFormatGetStorageSpaceRequired(sceneCommonIndexFormat, morphingModelsData[i]->numIndices);

	// create shared buffer
	vbo_ibo = demoState->vbo_staticSceneObjectDrawBuffer;
	a3bufferCreateSplit(vbo_ibo, "vbo/ibo:scene", a3buffer_vertex, sharedVertexStorage, sharedIndexStorage, 0, 0);
	sharedVertexStorage = 0;


	// create vertex formats and drawables
	// axes: position and color
	vao = demoState->vao_position_color;
	a3geometryGenerateVertexArray(vao, "vao:pos+col", displayShapesData + 0, vbo_ibo, sharedVertexStorage);
	currentDrawable = demoState->draw_axes;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 0, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);

	// grid: position attribute only
	// overlay objects are also just position
	vao = demoState->vao_position;
	a3geometryGenerateVertexArray(vao, "vao:pos", displayShapesData + 1, vbo_ibo, sharedVertexStorage);
	currentDrawable = demoState->draw_grid;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 1, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_edge;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 2, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_node;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 3, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);

	// models
	vao = demoState->vao_tangentbasis_texcoord;
	a3geometryGenerateVertexArray(vao, "vao:tb+tc", loadedModelsData + 0, vbo_ibo, sharedVertexStorage);
	currentDrawable = demoState->draw_teapot;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, loadedModelsData + 0, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unit_plane_z;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 0, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unit_box;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 1, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unit_sphere;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 2, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unit_cylinder;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 3, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unit_capsule;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 4, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unit_torus;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 5, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unit_cone;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 6, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);

	// morphing models
	vao = demoState->vao_tangentbasis_texcoord_morph;
	a3vertexArrayCreateDescriptor(vao, "vao:tb+tc+morph", vbo_ibo, morphFormat, sharedVertexStorage);
	{
		a3_VertexAttributeDataDescriptor morphAttribData[16] = { 0 }, * morphAttribDataPtr;
		a3_VertexAttributeName attribName;
		a3ui32 morphModelIndex = 0;

		for (i = 0, morphAttribDataPtr = morphAttribData, attribName = a3attrib_user00; i < morphingModelsMaxTargets; ++i)
			for (j = 0; j < 3; ++j, ++morphAttribDataPtr, ++attribName)
				a3vertexAttribDataCreateDescriptor(morphAttribDataPtr, (a3_VertexAttributeName)attribName, morphingModelsData[morphModelIndex][i].attribData[j * j]);
		a3vertexAttribDataCreateDescriptor(morphAttribDataPtr, a3attrib_user15, morphingModelsData[morphModelIndex]->attribData[a3attrib_geomTexcoord]);

		a3vertexArrayStore(vao, morphAttribData, morphingModelsData[morphModelIndex]->numVertices, 0, 0);
		a3indexBufferStore(vbo_ibo, sceneCommonIndexFormat, morphingModelsData[morphModelIndex]->indexData, morphingModelsData[morphModelIndex]->numIndices, 0, &i, 0);

		currentDrawable = demoState->draw_teapot_morph;
		a3vertexDrawableCreateIndexed(currentDrawable, vao, vbo_ibo, sceneCommonIndexFormat, morphingModelsData[morphModelIndex]->primType, i, morphingModelsData[morphModelIndex]->numIndices);
	}

	// release data when done
	for (i = 0; i < displayShapesCount; ++i)
		a3geometryReleaseData(displayShapesData + i);
	for (i = 0; i < proceduralShapesCount; ++i)
		a3geometryReleaseData(proceduralShapesData + i);
	for (i = 0; i < loadedModelsCount; ++i)
		a3geometryReleaseData(loadedModelsData + i);
	for (i = 0; i < morphingModelsCount; ++i)
		for (j = 0; j < morphingModelsMaxTargets; ++j)
			a3geometryReleaseData(morphingModelsData[i] + j);
}


// utility to load shaders
void a3demo_loadShaders(a3_DemoState *demoState)
{
	// structure to help with shader management
	typedef struct a3_TAG_DEMOSTATESHADER {
		a3_Shader shader[1];
		a3byte shaderName[32];

		a3_ShaderType shaderType;
		a3ui32 srcCount;
		const a3byte* filePath[8];	// max number of source files per shader
	} a3_DemoStateShader;

	// direct to demo programs
	a3_DemoStateShaderProgram *currentDemoProg;
	a3i32 flag;
	a3ui32 i;

	// maximum uniform buffer size
	const a3ui32 uBlockSzMax = a3shaderUniformBlockMaxSize();

	// some default uniform values
	const a3f32 defaultFloat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const a3f64 defaultDouble[] = { 0.0, 0.0, 0.0, 1.0 };
	const a3i32 defaultInt[] = { 0, 0, 0, 1 };
	const a3i32 defaultTexUnits[] = {
		a3tex_unit00, a3tex_unit01, a3tex_unit02, a3tex_unit03,
		a3tex_unit04, a3tex_unit05, a3tex_unit06, a3tex_unit07,
		a3tex_unit08, a3tex_unit09, a3tex_unit10, a3tex_unit11,
		a3tex_unit12, a3tex_unit13, a3tex_unit14, a3tex_unit15
	};

	// FSQ matrix
	const a3mat4 fsq = {
		2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 2.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};


	// list of all unique shaders
	// this is a good idea to avoid multi-loading 
	//	those that are shared between programs
	union {
		struct {
			// vertex shaders
			// base
			a3_DemoStateShader
				passthru_transform_vs[1],
				passColor_transform_vs[1],
				passthru_transform_instanced_vs[1],
				passColor_transform_instanced_vs[1];
			// 00-common
			a3_DemoStateShader
				passTexcoord_transform_vs[1],
				passTangentBasis_transform_vs[1],
				passTexcoord_transform_instanced_vs[1],
				passTangentBasis_transform_instanced_vs[1];
			// 01-pipeline
			a3_DemoStateShader
				passTangentBasis_shadowCoord_transform_vs[1],
				passTangentBasis_shadowCoord_transform_instanced_vs[1];
			// 02-pipeline-deferred
			a3_DemoStateShader
				passTangentBasis_ubo_transform_vs[1],
				passClipBiased_transform_instanced_vs[1];
			// 03-lod
			a3_DemoStateShader
				empty_vs[1];
			// 04-anim
			a3_DemoStateShader
				passColor_hierarchy_transform_instanced_vs[1],
				passTangentBasis_morph_transform_vs[1];

			// tessellation shaders
			// 03-lod
			a3_DemoStateShader
				tessIso_tcs[1],
				tessTriTangentBasis_tcs[1];
			a3_DemoStateShader
				passColor_interp_tes[1],
				passTangentBasis_displace_tes[1];

			// geometry shaders
			// 00-common
			a3_DemoStateShader
				drawTangentBasis_gs[1];

			// fragment shaders
			// base
			a3_DemoStateShader
				drawColorUnif_fs[1],
				drawColorAttrib_fs[1];
			// 00-common
			a3_DemoStateShader
				drawTexture_fs[1],
				drawLambert_fs[1],
				drawPhong_fs[1];
			// 01-pipeline
			a3_DemoStateShader
				postBright_fs[1],
				postBlur_fs[1],
				postBlend_fs[1],
				drawPhong_shadow_fs[1];
			// 02-pipeline-deferred
			a3_DemoStateShader
				postDeferredLightingComposite_fs[1],
				postDeferredShading_fs[1],
				drawPhongPointLight_fs[1],
				drawGBuffers_fs[1],
				drawPhongNM_fs[1];
			// 03-lod
			a3_DemoStateShader
				drawPhongPOM_fs[1];
		};
	} shaderList = {
		{
			// ****REMINDER: 'Encoded' shaders are available for proof-of-concept
			//	testing ONLY!  Insert /e before file names.
			// DO NOT SUBMIT WORK USING ENCODED SHADERS OR YOU WILL GET ZERO!!!

			// vs
			// base
			{ { { 0 },	"shdr-vs:passthru-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"passthru_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-col-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"passColor_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:passthru-trans-inst",		a3shader_vertex  ,	1,{ A3_DEMO_VS"passthru_transform_instanced_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-col-trans-inst",		a3shader_vertex  ,	1,{ A3_DEMO_VS"passColor_transform_instanced_vs4x.glsl" } } },
			// 00-common
			{ { { 0 },	"shdr-vs:pass-tex-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"00-common/e/passTexcoord_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-tb-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"00-common/e/passTangentBasis_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-tex-trans-inst",		a3shader_vertex  ,	1,{ A3_DEMO_VS"00-common/e/passTexcoord_transform_instanced_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-tb-trans-inst",		a3shader_vertex  ,	1,{ A3_DEMO_VS"00-common/e/passTangentBasis_transform_instanced_vs4x.glsl" } } },
			// 01-pipeline
			{ { { 0 },	"shdr-vs:pass-tb-sc-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"01-pipeline/e/passTangentBasis_shadowCoord_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-tb-sc-trans-inst",	a3shader_vertex  ,	1,{ A3_DEMO_VS"01-pipeline/e/passTangentBasis_shadowCoord_transform_instanced_vs4x.glsl" } } },
			// 02-pipeline-deferred
			{ { { 0 },	"shdr-vs:pass-tb-ubo-trans",		a3shader_vertex  ,	1,{ A3_DEMO_VS"02-pipeline-deferred/e/passTangentBasis_ubo_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-clipb-trans-inst",	a3shader_vertex  ,	1,{ A3_DEMO_VS"02-pipeline-deferred/e/passClipBiased_transform_instanced_vs4x.glsl" } } },
			// 03-lod
			{ { { 0 },	"shdr-vs:empty",					a3shader_vertex  ,	1,{ A3_DEMO_VS"03-lod/e/empty_vs4x.glsl" } } },
			// 04-anim
			{ { { 0 },	"shdr-vs:pass-hcol-trans-inst",		a3shader_vertex  ,	1,{ A3_DEMO_VS"04-anim/passColor_hierarchy_transform_instanced_vs4x.glsl" } } }, // ****DECODE
			{ { { 0 },	"shdr-vs:pass-tb-morph-trans",		a3shader_vertex  ,	2,{ A3_DEMO_VS"04-anim/passTangentBasis_morph_transform_vs4x.glsl", // ****DECODE
																					A3_DEMO_VS"00-common/utilCommon_vs4x.glsl",} } }, // ****DECODE

			// ts
			// 03-lod
			{ { { 0 },	"shdr-tcs:tess-line-curve",			a3shader_tessellationControl,		1,{ A3_DEMO_TS"03-lod/e/tessIso_tcs4x.glsl" } } },
			{ { { 0 },	"shdr-tcs:tess-tri-lod",			a3shader_tessellationControl,		1,{ A3_DEMO_TS"03-lod/e/tessTriTangentBasis_tcs4x.glsl" } } },
			{ { { 0 },	"shdr-tes:passthru-interp",			a3shader_tessellationEvaluation,	1,{ A3_DEMO_TS"03-lod/e/passColor_interp_tes4x.glsl" } } },
			{ { { 0 },	"shdr-tes:pass-tb-disp",			a3shader_tessellationEvaluation,	1,{ A3_DEMO_TS"03-lod/e/passTangentBasis_displace_tes4x.glsl" } } },

			// gs
			// 00-common
			{ { { 0 },	"shdr-gs:draw-tb",					a3shader_geometry,	2,{ A3_DEMO_GS"00-common/e/drawTangentBasis_gs4x.glsl",
																					A3_DEMO_GS"00-common/e/utilCommon_gs4x.glsl",} } },

			// fs
			// base
			{ { { 0 },	"shdr-fs:draw-col-unif",			a3shader_fragment,	1,{ A3_DEMO_FS"drawColorUnif_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-col-attr",			a3shader_fragment,	1,{ A3_DEMO_FS"drawColorAttrib_fs4x.glsl" } } },
			// 00-common
			{ { { 0 },	"shdr-fs:draw-tex",					a3shader_fragment,	1,{ A3_DEMO_FS"00-common/e/drawTexture_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-Lambert",				a3shader_fragment,	2,{ A3_DEMO_FS"00-common/e/drawLambert_fs4x.glsl",
																					A3_DEMO_FS"00-common/e/utilCommon_fs4x.glsl",} } },
			{ { { 0 },	"shdr-fs:draw-Phong",				a3shader_fragment,	2,{ A3_DEMO_FS"00-common/e/drawPhong_fs4x.glsl",
																					A3_DEMO_FS"00-common/e/utilCommon_fs4x.glsl",} } },
			// 01-pipeline
			{ { { 0 },	"shdr-fs:post-bright",				a3shader_fragment,	1,{ A3_DEMO_FS"01-pipeline/e/postBright_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:post-blur",				a3shader_fragment,	1,{ A3_DEMO_FS"01-pipeline/e/postBlur_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:post-blend",				a3shader_fragment,	1,{ A3_DEMO_FS"01-pipeline/e/postBlend_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-Phong-shadow",		a3shader_fragment,	2,{ A3_DEMO_FS"01-pipeline/e/drawPhong_shadow_fs4x.glsl",
																					A3_DEMO_FS"00-common/e/utilCommon_fs4x.glsl",} } },
			// 02-pipeline-deferred
			{ { { 0 },	"shdr-fs:post-defer-comp",			a3shader_fragment,	1,{ A3_DEMO_FS"02-pipeline-deferred/e/postDeferredLightingComposite_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:post-Phong-defer",			a3shader_fragment,	2,{ A3_DEMO_FS"02-pipeline-deferred/e/postDeferredShading_fs4x.glsl",
																					A3_DEMO_FS"00-common/e/utilCommon_fs4x.glsl",} } },
			{ { { 0 },	"shdr-fs:draw-Phong-light",			a3shader_fragment,	2,{ A3_DEMO_FS"02-pipeline-deferred/e/drawPhongPointLight_fs4x.glsl",
																					A3_DEMO_FS"00-common/e/utilCommon_fs4x.glsl",} } },
			{ { { 0 },	"shdr-fs:draw-gbuffers",			a3shader_fragment,	1,{ A3_DEMO_FS"02-pipeline-deferred/e/drawGBuffers_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-Phong-nm",			a3shader_fragment,	2,{ A3_DEMO_FS"02-pipeline-deferred/e/drawPhongNM_fs4x.glsl",
																					A3_DEMO_FS"00-common/e/utilCommon_fs4x.glsl",} } },
			{ { { 0 },	"shdr-fs:draw-Phong-pom",			a3shader_fragment,	2,{ A3_DEMO_FS"03-lod/e/drawPhongPOM_fs4x.glsl",
																					A3_DEMO_FS"00-common/e/utilCommon_fs4x.glsl",} } },
		}
	};
	a3_DemoStateShader *const shaderListPtr = (a3_DemoStateShader *)(&shaderList), *shaderPtr;
	const a3ui32 numUniqueShaders = sizeof(shaderList) / sizeof(a3_DemoStateShader);


	printf("\n\n---------------- LOAD SHADERS STARTED  ---------------- \n");


	// load unique shaders: 
	//	- load file contents
	//	- create and compile shader object
	//	- release file contents
	for (i = 0; i < numUniqueShaders; ++i)
	{
		shaderPtr = shaderListPtr + i;
		flag = a3shaderCreateFromFileList(shaderPtr->shader,
			shaderPtr->shaderName, shaderPtr->shaderType,
			shaderPtr->filePath, shaderPtr->srcCount);
		if (flag == 0)
			printf("\n ^^^^ SHADER %u '%s' FAILED TO COMPILE \n\n", i, shaderPtr->shader->handle->name);
	}


	// setup programs: 
	//	- create program object
	//	- attach shader objects

	// base programs: 
	// transform-only program
	currentDemoProg = demoState->prog_transform;
	a3shaderProgramCreate(currentDemoProg->program, "prog:transform");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_transform_vs->shader);
	// transform-only program with instancing
	currentDemoProg = demoState->prog_transform_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:transform-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_transform_instanced_vs->shader);
	// uniform color program
	currentDemoProg = demoState->prog_drawColorUnif;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-col-unif");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorUnif_fs->shader);
	// color attrib program
	currentDemoProg = demoState->prog_drawColorAttrib;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-col-attr");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passColor_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// uniform color program with instancing
	currentDemoProg = demoState->prog_drawColorUnif_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-col-unif-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorUnif_fs->shader);
	// color attrib program with instancing
	currentDemoProg = demoState->prog_drawColorAttrib_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-col-attr-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passColor_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);

	// 00-common programs: 
	// texturing
	currentDemoProg = demoState->prog_drawTexture;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tex");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTexture_fs->shader);
	// texturing with instancing
	currentDemoProg = demoState->prog_drawTexture_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tex-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTexture_fs->shader);
	// Lambert
	currentDemoProg = demoState->prog_drawLambert;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Lambert");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawLambert_fs->shader);
	// Lambert with instancing
	currentDemoProg = demoState->prog_drawLambert_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Lambert-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawLambert_fs->shader);
	// Phong
	currentDemoProg = demoState->prog_drawPhong;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_fs->shader);
	// Phong with instancing
	currentDemoProg = demoState->prog_drawPhong_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_fs->shader);

	// tangent basis
	currentDemoProg = demoState->prog_drawTangentBasis;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tb");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTangentBasis_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// tangent basis with instancing
	currentDemoProg = demoState->prog_drawTangentBasis_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tb-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTangentBasis_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);

	// 01-pipeline programs: 
	// Phong shading with shadow mapping
	currentDemoProg = demoState->prog_drawPhong_shadow;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-shadow");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_shadowCoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_shadow_fs->shader);
	// Phong shading with shadow mapping, instanced
	currentDemoProg = demoState->prog_drawPhong_shadow_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-shadow-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_shadowCoord_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_shadow_fs->shader);
	// bright pass
	currentDemoProg = demoState->prog_postBright;
	a3shaderProgramCreate(currentDemoProg->program, "prog:post-bright");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.postBright_fs->shader);
	// blurring
	currentDemoProg = demoState->prog_postBlur;
	a3shaderProgramCreate(currentDemoProg->program, "prog:post-blur");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.postBlur_fs->shader);
	// blending
	currentDemoProg = demoState->prog_postBlend;
	a3shaderProgramCreate(currentDemoProg->program, "prog:post-blend");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.postBlend_fs->shader);

	// 02-pipeline-deferred programs: 
	// Phong shading with normal mapping
	currentDemoProg = demoState->prog_drawPhongNM_ubo;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-PhongNM-ubo");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_ubo_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhongNM_fs->shader);
	// g-buffers
	currentDemoProg = demoState->prog_drawGBuffers;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-gbuffers");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_ubo_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawGBuffers_fs->shader);
	// point light volume
	currentDemoProg = demoState->prog_drawPhongPointLight_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-light");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passClipBiased_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhongPointLight_fs->shader);
	// deferred shading
	currentDemoProg = demoState->prog_postDeferredShading;
	a3shaderProgramCreate(currentDemoProg->program, "prog:post-Phong-def");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.postDeferredShading_fs->shader);
	// deferred lighting composite
	currentDemoProg = demoState->prog_postDeferredLightingComposite;
	a3shaderProgramCreate(currentDemoProg->program, "prog:post-def-comp");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.postDeferredLightingComposite_fs->shader);

	// 03-lod programs: 
	// Phong LOD
	currentDemoProg = demoState->prog_drawPhongLOD;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-PhongLOD");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_ubo_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.tessTriTangentBasis_tcs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_displace_tes->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhongNM_fs->shader);
	// Phong with parallax occlusion mapping
	currentDemoProg = demoState->prog_drawPhongPOM;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-PhongPOM");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_ubo_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhongPOM_fs->shader);
	// tangent basis LOD
	currentDemoProg = demoState->prog_drawTangentBasisPOM;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tb-POM");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_ubo_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTangentBasis_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// tangent basis LOD
	currentDemoProg = demoState->prog_drawTangentBasisLOD;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tb-LOD");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_ubo_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.tessTriTangentBasis_tcs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_displace_tes->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTangentBasis_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// curve
	currentDemoProg = demoState->prog_drawCurvePath;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-curve");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.empty_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.tessIso_tcs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passColor_interp_tes->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);

	// 04-anim programs: 
	// Phong POM morphing
	currentDemoProg = demoState->prog_drawPhongPOM_morph;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-PhongPOM-morph");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_morph_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhongPOM_fs->shader);

	// tangent basis POM morphing
	currentDemoProg = demoState->prog_drawTangentBasisPOM_morph;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tb-POM-morph");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_morph_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTangentBasis_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);

	// hierarchical color
	currentDemoProg = demoState->prog_drawColorHierarchy_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-hcol-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passColor_hierarchy_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);


	// activate a primitive for validation
	// makes sure the specified geometry can draw using programs
	// good idea to activate the drawable with the most attributes
	a3vertexDrawableActivate(demoState->draw_axes);

	// link and validate all programs
	for (i = 0; i < demoStateMaxCount_shaderProgram; ++i)
	{
		currentDemoProg = demoState->shaderProgram + i;
		flag = a3shaderProgramLink(currentDemoProg->program);
		if (flag == 0)
			printf("\n ^^^^ PROGRAM %u '%s' FAILED TO LINK \n\n", i, currentDemoProg->program->handle->name);

		flag = a3shaderProgramValidate(currentDemoProg->program);
		if (flag == 0)
			printf("\n ^^^^ PROGRAM %u '%s' FAILED TO VALIDATE \n\n", i, currentDemoProg->program->handle->name);
	}

	// if linking fails, contingency plan goes here
	// otherwise, release shaders
	for (i = 0; i < numUniqueShaders; ++i)
	{
		shaderPtr = shaderListPtr + i;
		a3shaderRelease(shaderPtr->shader);
	}


	// prepare uniforms algorithmically instead of manually for all programs
	// get uniform and uniform block locations and set default values for all 
	//	programs that have a uniform that will either never change or is
	//	consistent for all programs
	for (i = 0; i < demoStateMaxCount_shaderProgram; ++i)
	{
		// activate program
		currentDemoProg = demoState->shaderProgram + i;
		a3shaderProgramActivate(currentDemoProg->program);

		// common VS
		a3demo_setUniformDefaultMat4(currentDemoProg, uMVP);
		a3demo_setUniformDefaultMat4(currentDemoProg, uMV);
		a3demo_setUniformDefaultMat4(currentDemoProg, uP);
		a3demo_setUniformDefaultMat4(currentDemoProg, uP_inv);
		a3demo_setUniformDefaultMat4(currentDemoProg, uPB);
		a3demo_setUniformDefaultMat4(currentDemoProg, uPB_inv);
		a3demo_setUniformDefaultMat4(currentDemoProg, uMV_nrm);
		a3demo_setUniformDefaultMat4(currentDemoProg, uMVPB);
		a3demo_setUniformDefaultMat4(currentDemoProg, uMVPB_other);
		a3demo_setUniformDefaultMat4(currentDemoProg, uAtlas);

		// common TS
		a3demo_setUniformDefaultInteger(currentDemoProg, uLevelInner, defaultInt);
		a3demo_setUniformDefaultInteger(currentDemoProg, uLevelOuter, defaultInt);

		// common texture
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_dm, defaultTexUnits + 0);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_sm, defaultTexUnits + 1);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_nm, defaultTexUnits + 2);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_hm, defaultTexUnits + 3);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_ramp_dm, defaultTexUnits + 4);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_ramp_sm, defaultTexUnits + 5);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_shadow, defaultTexUnits + 6);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_project, defaultTexUnits + 7);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage00, defaultTexUnits + 0);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage01, defaultTexUnits + 1);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage02, defaultTexUnits + 2);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage03, defaultTexUnits + 3);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage04, defaultTexUnits + 4);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage05, defaultTexUnits + 5);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage06, defaultTexUnits + 6);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage07, defaultTexUnits + 7);

		// common general
		a3demo_setUniformDefaultInteger(currentDemoProg, uIndex, defaultInt);
		a3demo_setUniformDefaultInteger(currentDemoProg, uCount, defaultInt);
		a3demo_setUniformDefaultDouble(currentDemoProg, uAxis, defaultDouble);
		a3demo_setUniformDefaultDouble(currentDemoProg, uSize, defaultDouble);
		a3demo_setUniformDefaultDouble(currentDemoProg, uFlag, defaultDouble);
		a3demo_setUniformDefaultDouble(currentDemoProg, uTime, defaultDouble);
		a3demo_setUniformDefaultVec4(currentDemoProg, uColor0, a3vec4_one.v);
		a3demo_setUniformDefaultVec4(currentDemoProg, uColor, a3vec4_one.v);

		// transformation uniform blocks
		a3demo_setUniformDefaultBlock(currentDemoProg, ubTransformStack, demoProg_blockTransformStack);
		a3demo_setUniformDefaultBlock(currentDemoProg, ubTransformBlend, demoProg_blockTransformBlend);
		a3demo_setUniformDefaultBlock(currentDemoProg, ubTransformMVP, demoProg_blockTransformStack);
		a3demo_setUniformDefaultBlock(currentDemoProg, ubTransformMVPB, demoProg_blockTransformBlend);

		// lighting and shading uniform blocks
		a3demo_setUniformDefaultBlock(currentDemoProg, ubMaterial, demoProg_blockMaterial);
		a3demo_setUniformDefaultBlock(currentDemoProg, ubLight, demoProg_blockLight);

		// curve editor uniform blocks
		a3demo_setUniformDefaultBlock(currentDemoProg, ubHandle, demoProg_blockHandle);
		a3demo_setUniformDefaultBlock(currentDemoProg, ubCurve, demoProg_blockCurve);
	}


	// additional pre-configuration for some programs whose uniforms will not change
	// e.g. post-processing effects must transform unit FSQ to true FSQ
	for (currentDemoProg = demoState->prog_postBright;
		currentDemoProg <= demoState->prog_postBlend;
		++currentDemoProg)
	{
		a3shaderProgramActivate(currentDemoProg->program);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProg->uMVP, 1, fsq.mm);
	}
	for (currentDemoProg = demoState->prog_postDeferredLightingComposite;
		currentDemoProg <= demoState->prog_postDeferredShading;
		++currentDemoProg)
	{
		a3shaderProgramActivate(currentDemoProg->program);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProg->uMVP, 1, fsq.mm);
	}


	// allocate uniform buffers
	a3bufferCreate(demoState->ubo_curve, "ubo:curve", a3buffer_uniform, a3index_countMaxShort, 0);
	a3bufferCreate(demoState->ubo_light, "ubo:light", a3buffer_uniform, a3index_countMaxShort, 0);
	a3bufferCreate(demoState->ubo_mvp, "ubo:mvp", a3buffer_uniform, a3index_countMaxShort, 0);
	a3bufferCreate(demoState->ubo_mvp + 1, "ubo:mvp1", a3buffer_uniform, a3index_countMaxShort, 0);
	a3bufferCreate(demoState->ubo_transform, "ubo:transform", a3buffer_uniform, a3index_countMaxShort, 0);


	printf("\n\n---------------- LOAD SHADERS FINISHED ---------------- \n");

	//done
	a3shaderProgramDeactivate();
	a3vertexDrawableDeactivate();
}


// utility to load textures
void a3demo_loadTextures(a3_DemoState* demoState)
{	
	// indexing
	a3_Texture* tex;
	a3ui32 i;
	a3ui16 w, h;

	// structure for texture loading
	typedef struct a3_TAG_DEMOSTATETEXTURE {
		a3_Texture* texture;
		a3byte textureName[32];
		const a3byte* filePath;
	} a3_DemoStateTexture;

	// texture objects
	union {
		struct {
			a3_DemoStateTexture
				texEarthDM[1],
				texEarthSM[1],
				texEarthNM[1],
				texEarthHM[1],
				texMarsDM[1],
				texMarsSM[1],
				texMarsNM[1],
				texMarsHM[1],
				texStoneDM[1],
				texStoneNM[1],
				texStoneHM[1],
				texSunDM[1];
			a3_DemoStateTexture
				texAtlasDM[1],
				texAtlasSM[1],
				texAtlasNM[1],
				texAtlasHM[1];
			a3_DemoStateTexture
				texSkyClouds[1],
				texSkyWater[1],
				texRampDM[1],
				texRampSM[1],
				texTestSprite[1],
				texChecker[1];
		};
	} textureList = {
		{
			{ demoState->tex_earth_dm,		"tex:earth-dm",		A3_DEMO_TEX"earth/2k/earth_dm_2k.png" },
			{ demoState->tex_earth_sm,		"tex:earth-sm",		A3_DEMO_TEX"earth/2k/earth_sm_2k.png" },
			{ demoState->tex_earth_nm,		"tex:earth-nm",		A3_DEMO_TEX"earth/2k/earth_nm_2k.png" },
			{ demoState->tex_earth_hm,		"tex:earth-hm",		A3_DEMO_TEX"earth/2k/earth_hm_2k.png" },
			{ demoState->tex_mars_dm,		"tex:mars-dm",		A3_DEMO_TEX"mars/1k/mars_1k_dm.png" },
			{ demoState->tex_mars_sm,		"tex:mars-sm",		A3_DEMO_TEX"mars/1k/mars_1k_sm.png" },
			{ demoState->tex_mars_nm,		"tex:mars-nm",		A3_DEMO_TEX"mars/1k/mars_1k_nm.png" },
			{ demoState->tex_mars_hm,		"tex:mars-hm",		A3_DEMO_TEX"mars/1k/mars_1k_hm.png" },
			{ demoState->tex_stone_dm,		"tex:stone-dm",		A3_DEMO_TEX"stone/stone_dm.png" },
			{ demoState->tex_stone_nm,		"tex:stone-nm",		A3_DEMO_TEX"stone/stone_nm.png" },
			{ demoState->tex_stone_hm,		"tex:stone-hm",		A3_DEMO_TEX"stone/stone_hm.png" },
			{ demoState->tex_sun_dm,		"tex:sun-dm",		A3_DEMO_TEX"sun/1k/sun_dm.png" },

			{ demoState->tex_atlas_dm,		"tex:atlas-dm",		A3_DEMO_TEX"atlas/atlas_scene_dm.png" },
			{ demoState->tex_atlas_sm,		"tex:atlas-sm",		A3_DEMO_TEX"atlas/atlas_scene_sm.png" },
			{ demoState->tex_atlas_nm,		"tex:atlas-nm",		A3_DEMO_TEX"atlas/atlas_scene_nm.png" },
			{ demoState->tex_atlas_hm,		"tex:atlas-hm",		A3_DEMO_TEX"atlas/atlas_scene_hm.png" },

			{ demoState->tex_skybox_clouds,	"tex:sky-clouds",	A3_DEMO_TEX"bg/sky_clouds.png" },
			{ demoState->tex_skybox_water,	"tex:sky-water",	A3_DEMO_TEX"bg/sky_water.png" },
			{ demoState->tex_ramp_dm,		"tex:ramp-dm",		A3_DEMO_TEX"sprite/celRamp_dm.png" },
			{ demoState->tex_ramp_sm,		"tex:ramp-sm",		A3_DEMO_TEX"sprite/celRamp_sm.png" },
			{ demoState->tex_testsprite,	"tex:testsprite",	A3_DEMO_TEX"sprite/spriteTest8x8.png" },
			{ demoState->tex_checker,		"tex:checker",		A3_DEMO_TEX"sprite/checker.png" },
		}
	};
	const a3ui32 numTextures = sizeof(textureList) / sizeof(a3_DemoStateTexture);
	a3_DemoStateTexture* const textureListPtr = (a3_DemoStateTexture*)(&textureList), * texturePtr;

	// load all textures
	for (i = 0; i < numTextures; ++i)
	{
		texturePtr = textureListPtr + i;
		a3textureCreateFromFile(texturePtr->texture, texturePtr->textureName, texturePtr->filePath);
		a3textureActivate(texturePtr->texture, a3tex_unit00);
		a3textureDefaultSettings();
	}

	// change settings on a per-texture or per-type basis
	// materials
	tex = demoState->tex_earth_dm;
	for (i = 0; i < 16; ++i, ++tex)
	{
		a3textureActivate(tex, a3tex_unit00);
		a3textureChangeFilterMode(a3tex_filterLinear); // linear pixel blending
		a3textureChangeRepeatMode(a3tex_repeatNormal, a3tex_repeatClamp); // repeat horizontal, clamp vertical
	}
	// stone
	tex = demoState->tex_stone_dm;
	for (i = 0; i < 3; ++i, ++tex)
	{
		a3textureActivate(tex, a3tex_unit00);
		a3textureChangeFilterMode(a3tex_filterLinear); // linear pixel blending
		a3textureChangeRepeatMode(a3tex_repeatNormal, a3tex_repeatNormal); // repeat both
	}
	// atlases & skyboxes
	tex = demoState->tex_atlas_dm;
	for (i = 0; i < 6; ++i, ++tex)
	{
		a3textureActivate(tex, a3tex_unit00);
		a3textureChangeFilterMode(a3tex_filterLinear); // linear pixel blending
	}
	// ramps
	tex = demoState->tex_ramp_dm;
	for (i = 0; i < 2; ++i, ++tex)
	{
		a3textureActivate(tex, a3tex_unit00);
		a3textureChangeRepeatMode(a3tex_repeatClamp, a3tex_repeatClamp); // clamp both axes
	}


	// texture atlas matrices
	tex = demoState->tex_atlas_dm;
	w = tex->width;
	h = tex->height;
	a3demo_setAtlasTransform_internal(demoState->atlas_earth.m, w, h, 0, 0, 1024, 512, 8, 8);
	a3demo_setAtlasTransform_internal(demoState->atlas_mars.m, w, h, 0, 544, 1024, 512, 8, 8);
	a3demo_setAtlasTransform_internal(demoState->atlas_moon.m, w, h, 0, 1088, 1024, 512, 8, 8);
	a3demo_setAtlasTransform_internal(demoState->atlas_marble.m, w, h, 1056, 0, 512, 512, 8, 8);
	a3demo_setAtlasTransform_internal(demoState->atlas_copper.m, w, h, 1056, 544, 512, 512, 8, 8);
	a3demo_setAtlasTransform_internal(demoState->atlas_stone.m, w, h, 1600, 0, 256, 256, 8, 8);
	a3demo_setAtlasTransform_internal(demoState->atlas_checker.m, w, h, 1888, 0, 128, 128, 8, 8);


	// texture sets
	a3demo_initTextureSet(demoState->texSet_earth, demoState->tex_earth_dm, demoState->tex_earth_sm, demoState->tex_earth_nm, demoState->tex_earth_hm);
	a3demo_initTextureSet(demoState->texSet_mars, demoState->tex_mars_dm, demoState->tex_mars_sm, demoState->tex_mars_nm, demoState->tex_mars_hm);
	a3demo_initTextureSet(demoState->texSet_stone, demoState->tex_stone_dm, demoState->tex_stone_dm, demoState->tex_stone_nm, demoState->tex_stone_hm);


	// done
	a3textureDeactivate(a3tex_unit00);
}


// utility to load framebuffers
void a3demo_loadFramebuffers(a3_DemoState* demoState)
{
	// create framebuffers and change their texture settings if need be
	a3_Framebuffer* fbo;
	a3ui32 i, j;

	// frame sizes
	const a3ui16 frameWidth1 = demoState->frameWidth, frameHeight1 = demoState->frameHeight;
	const a3ui16 frameWidth2 = frameWidth1 / 2, frameHeight2 = frameHeight1 / 2;
	const a3ui16 frameWidth4 = frameWidth2 / 2, frameHeight4 = frameHeight2 / 2;
	const a3ui16 frameWidth8 = frameWidth4 / 2, frameHeight8 = frameHeight4 / 2;
	const a3ui16 shadowMapSize = 2048;

	// storage precision and targets
	const a3_FramebufferColorType colorType_scene = a3fbo_colorRGBA16;
	const a3_FramebufferDepthType depthType_scene = a3fbo_depth24_stencil8;
	const a3ui32 targets_scene = 1;
	const a3_FramebufferColorType colorType_composite = a3fbo_colorRGBA16;
	const a3_FramebufferDepthType depthType_composite = a3fbo_depthDisable;
	const a3ui32 targets_composite = 1;


	// initialize framebuffers: MRT, color and depth formats, size
	fbo = demoState->fbo_c16x4_d24s8;
	a3framebufferCreate(fbo, "fbo:c16x4;d24s8",
		4, a3fbo_colorRGBA16, a3fbo_depth24_stencil8,
		frameWidth1, frameHeight1);

	fbo = demoState->fbo_c32f;
	a3framebufferCreate(fbo, "fbo:c32f",
		1, a3fbo_colorRGBA32F, a3fbo_depthDisable,
		frameWidth1, frameHeight1);

	fbo = demoState->fbo_d32;
	a3framebufferCreate(fbo, "fbo:d32",
		0, a3fbo_colorDisable, a3fbo_depth32,
		shadowMapSize, shadowMapSize);

	// initialize sets
	for (i = 0, j = a3demoArrayLen(demoState->fbo_c16x4), fbo = demoState->fbo_c16x4;
		i < j;
		++i, ++fbo)
	{
		a3framebufferCreate(fbo, "fbo:c16x4[]",
			4, a3fbo_colorRGBA16, a3fbo_depthDisable,
			frameWidth1, frameHeight1);
	}
	for (i = 0, j = a3demoArrayLen(demoState->fbo_c16_szHalf), fbo = demoState->fbo_c16_szHalf;
		i < j;
		++i, ++fbo)
	{
		a3framebufferCreate(fbo, "fbo:c16;size=half[]",
			1, a3fbo_colorRGBA16, a3fbo_depthDisable,
			frameWidth2, frameHeight2);
		a3framebufferCreate(fbo + j, "fbo:c16;size=quarter[]",
			1, a3fbo_colorRGBA16, a3fbo_depthDisable,
			frameWidth4, frameHeight4);
		a3framebufferCreate(fbo + j + j, "fbo:c16;size=eighth[]",
			1, a3fbo_colorRGBA16, a3fbo_depthDisable,
			frameWidth8, frameHeight8);
	}


	// change texture settings for all framebuffers
	for (i = 0, fbo = demoState->framebuffer;
		i < demoStateMaxCount_framebuffer;
		++i, ++fbo)
	{
		// color, if applicable
		for (j = 0; j < fbo->color; ++j)
		{
			a3framebufferBindColorTexture(fbo, a3tex_unit00, j);
			a3textureChangeRepeatMode(a3tex_repeatClamp, a3tex_repeatClamp);
			a3textureChangeFilterMode(a3tex_filterLinear);
		}

		// depth, if applicable
		if (fbo->depthStencil)
		{
			a3framebufferBindDepthTexture(fbo, a3tex_unit00);
			a3textureChangeRepeatMode(a3tex_repeatClamp, a3tex_repeatClamp);
			a3textureChangeFilterMode(a3tex_filterLinear);
		}
	}


	// deactivate texture
	a3textureDeactivate(a3tex_unit00);
}


//-----------------------------------------------------------------------------

// internal utility for refreshing drawable
inline void a3_refreshDrawable_internal(a3_VertexDrawable *drawable, a3_VertexArrayDescriptor *vertexArray, a3_IndexBuffer *indexBuffer)
{
	drawable->vertexArray = vertexArray;
	if (drawable->indexType)
		drawable->indexBuffer = indexBuffer;
}


// the handle release callbacks are no longer valid; since the library was 
//	reloaded, old function pointers are out of scope!
// could reload everything, but that would mean rebuilding GPU data...
//	...or just set new function pointers!
void a3demo_loadValidate(a3_DemoState* demoState)
{
	a3_BufferObject* currentBuff = demoState->drawDataBuffer,
		* const endBuff = currentBuff + demoStateMaxCount_drawDataBuffer;
	a3_VertexArrayDescriptor* currentVAO = demoState->vertexArray,
		* const endVAO = currentVAO + demoStateMaxCount_vertexArray;
	a3_DemoStateShaderProgram* currentProg = demoState->shaderProgram,
		* const endProg = currentProg + demoStateMaxCount_shaderProgram;
	a3_UniformBuffer* currentUBO = demoState->uniformBuffer,
		* const endUBO = currentUBO + demoStateMaxCount_uniformBuffer;
	a3_Texture* currentTex = demoState->texture,
		* const endTex = currentTex + demoStateMaxCount_texture;
	a3_Framebuffer* currentFBO = demoState->framebuffer,
		* const endFBO = currentFBO + demoStateMaxCount_framebuffer;

	// set pointers to appropriate release callback for different asset types
	while (currentBuff < endBuff)
		a3bufferHandleUpdateReleaseCallback(currentBuff++);
	while (currentVAO < endVAO)
		a3vertexArrayHandleUpdateReleaseCallback(currentVAO++);
	while (currentProg < endProg)
		a3shaderProgramHandleUpdateReleaseCallback((currentProg++)->program);
	while (currentUBO < endUBO)
		a3bufferHandleUpdateReleaseCallback(currentUBO++);
	while (currentTex < endTex)
		a3textureHandleUpdateReleaseCallback(currentTex++);
	while (currentFBO < endFBO)
		a3framebufferHandleUpdateReleaseCallback(currentFBO++);

	// re-link specific object pointers for different asset types
	currentBuff = demoState->vbo_staticSceneObjectDrawBuffer;

	currentVAO = demoState->vao_position_color;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_axes, currentVAO, currentBuff);

	currentVAO = demoState->vao_position;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_grid, currentVAO, currentBuff);

	currentVAO = demoState->vao_tangentbasis_texcoord;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_teapot, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unit_plane_z, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unit_box, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unit_sphere, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unit_cylinder, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unit_capsule, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unit_torus, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unit_cone, currentVAO, currentBuff);

	currentVAO = demoState->vao_tangentbasis_texcoord_morph;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_teapot_morph, currentVAO, currentBuff);
}


//-----------------------------------------------------------------------------
