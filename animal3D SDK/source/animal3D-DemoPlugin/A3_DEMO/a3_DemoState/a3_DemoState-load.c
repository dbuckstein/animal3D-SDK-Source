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


// initialize dummy drawable
inline void a3demo_initDummyDrawable_internal(a3_DemoState *demoState)
{
	// dummy drawable for point drawing: copy any of the existing ones, 
	//	set vertex count to 1 and primitive to points (0x0000)
	// DO NOT RELEASE THIS DRAWABLE; it is a managed stand-in!!!

}


//-----------------------------------------------------------------------------
// uniform helpers



//-----------------------------------------------------------------------------
// LOADING

// utility to load geometry
void a3demo_loadGeometry(a3_DemoState *demoState)
{
	// tmp descriptor for loaded model
	typedef struct a3_TAG_DEMOSTATELOADEDMODEL {
		const a3byte *modelFilePath;
		const a3real *transform;
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
	a3ui32 sharedVertexStorage = 0, sharedIndexStorage = 0;
	a3ui32 numVerts = 0;
	a3ui32 i = 0, j = 0;


	// file streaming (if requested)
	a3_FileStream fileStream[1] = { 0 };
	const a3byte *const geometryStream = "./data/gpro_base_geom.dat";


	// procedural scene objects
	// attempt to load stream if requested
	if (demoState->streaming && a3fileStreamOpenRead(fileStream, geometryStream))
	{
		// read from stream


		// done
		a3fileStreamClose(fileStream);
	}
	// not streaming or stream doesn't exist
	else if (!demoState->streaming || a3fileStreamOpenWrite(fileStream, geometryStream))
	{
		// ****TO-DO: 
		// create new raw data


		// write to stream


		// done
		a3fileStreamClose(fileStream);
	}


	// ****TO-DO: 
	// GPU data upload process: 
	//	- determine storage requirements
	//	- allocate buffer
	//	- create vertex arrays using unique formats
	//	- create drawable and upload data
	

	// ****TO-DO: 
	// release raw data when done


	// dummy
	a3demo_initDummyDrawable_internal(demoState);
}


// utility to load shaders
void a3demo_loadShaders(a3_DemoState *demoState)
{
	// structure to help with shader management
	typedef struct a3_TAG_DEMOSTATESHADER {
		a3byte shaderName[32];
		a3ui32 srcCount;
		const a3byte* filePath[8];	// max number of source files per shader
	} a3_DemoStateShader;

	// direct to demo programs
	a3i32 flag = 0;
	a3ui32 i = 0;

	// some default uniform values
	const a3f32 defaultFloat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const a3f64 defaultDouble[] = { 0.0, 0.0, 0.0, 1.0 };
	const a3i32 defaultInt[] = { 0, 0, 0, 1 };
	const a3i32 defaultTexUnits[] = { 0, 1, 2, 3, 4, 5, 6, 7, };


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
		};
	} shaderList = {
		{
			// ****REMINDER: 'Encoded' shaders are available for proof-of-concept
			//	testing ONLY!  Insert /e before file names.
			// DO NOT SUBMIT WORK USING ENCODED SHADERS OR YOU WILL GET ZERO!!!

			// vs
			// base
			{ { "shdr-vs:passthru-trans",				1,{ A3_DEMO_VS"e/passthru_transform_vs4x.glsl" } } },
			{ { "shdr-vs:pass-col-trans",				1,{ A3_DEMO_VS"e/passColor_transform_vs4x.glsl" } } },
			{ { "shdr-vs:passthru-trans-inst",			1,{ A3_DEMO_VS"e/passthru_transform_instanced_vs4x.glsl" } } },
			{ { "shdr-vs:pass-col-trans-inst",			1,{ A3_DEMO_VS"e/passColor_transform_instanced_vs4x.glsl" } } },
			// 00-common
			{ { "shdr-vs:pass-tex-trans",				1,{ A3_DEMO_VS"00-common/e/passTexcoord_transform_vs4x.glsl" } } },
			{ { "shdr-vs:pass-tb-trans",				1,{ A3_DEMO_VS"00-common/e/passTangentBasis_transform_vs4x.glsl" } } },
			{ { "shdr-vs:pass-tex-trans-inst",			1,{ A3_DEMO_VS"00-common/e/passTexcoord_transform_instanced_vs4x.glsl" } } },
			{ { "shdr-vs:pass-tb-trans-inst",			1,{ A3_DEMO_VS"00-common/e/passTangentBasis_transform_instanced_vs4x.glsl" } } },

			// gs
			// 00-common
			{ { "shdr-gs:draw-tb",						2,{ A3_DEMO_GS"00-common/e/drawTangentBasis_gs4x.glsl",
															A3_DEMO_GS"00-common/e/utilCommon_gs4x.glsl",} } },

			// fs
			// base
			{ { "shdr-fs:draw-col-unif",				1,{ A3_DEMO_FS"e/drawColorUnif_fs4x.glsl" } } },
			{ { "shdr-fs:draw-col-attr",				1,{ A3_DEMO_FS"e/drawColorAttrib_fs4x.glsl" } } },
			// 00-common
			{ { "shdr-fs:draw-tex",						1,{ A3_DEMO_FS"00-common/e/drawTexture_fs4x.glsl" } } },
			{ { "shdr-fs:draw-Lambert",					2,{ A3_DEMO_FS"00-common/e/drawLambert_fs4x.glsl",
															A3_DEMO_FS"00-common/e/utilCommon_fs4x.glsl",} } },
			{ { "shdr-fs:draw-Phong",					2,{ A3_DEMO_FS"00-common/e/drawPhong_fs4x.glsl",
															A3_DEMO_FS"00-common/e/utilCommon_fs4x.glsl",} } },
		}
	};
	a3_DemoStateShader *const shaderListPtr = (a3_DemoStateShader *)(&shaderList), *shaderPtr;
	const a3ui32 numUniqueShaders = sizeof(shaderList) / sizeof(a3_DemoStateShader);


	printf("\n\n---------------- LOAD SHADERS STARTED  ---------------- \n");


	// ****TO-DO: 
	// load unique shaders: 
	//	- load file contents
	//	- create and compile shader object
	//	- release file contents
	for (i = 0; i < numUniqueShaders; ++i)
	{
		shaderPtr = shaderListPtr + i;

	}


	// ****TO-DO: 
	// setup programs: 
	//	- create program object
	//	- attach shader objects


	// ****TO-DO: 
	// activate a primitive for validation
	// makes sure the specified geometry can draw using programs
	// good idea to activate the drawable with the most attributes


	// ****TO-DO: 
	// link and validate all programs


	// ****TO-DO: 
	// if linking fails, contingency plan goes here
	// otherwise, release shaders
	for (i = 0; i < numUniqueShaders; ++i)
	{
		shaderPtr = shaderListPtr + i;

	}


	// ****TO-DO: 
	// prepare uniforms algorithmically instead of manually for all programs
	// get uniform and uniform block locations and set default values for all 
	//	programs that have a uniform that will either never change or is
	//	consistent for all programs


	printf("\n\n---------------- LOAD SHADERS FINISHED ---------------- \n");

	//done
}


// utility to load textures
void a3demo_loadTextures(a3_DemoState* demoState)
{	
	// indexing
	a3ui32 i;

	// structure for texture loading
	typedef struct a3_TAG_DEMOSTATETEXTURE {
		a3byte textureName[32];
		const a3byte* filePath;
	} a3_DemoStateTexture;

	// texture objects
	union {
		struct {
			a3_DemoStateTexture texSkyClouds[1];
			a3_DemoStateTexture texSkyWater[1];
			a3_DemoStateTexture texRampDM[1];
			a3_DemoStateTexture texRampSM[1];
			a3_DemoStateTexture texTestSprite[1];
			a3_DemoStateTexture texChecker[1];
		};
	} textureList = {
		{
			{ "tex:sky-clouds",	"../../../../resource/tex/bg/sky_clouds.png" },
			{ "tex:sky-water",	"../../../../resource/tex/bg/sky_water.png" },
			{ "tex:ramp-dm",	"../../../../resource/tex/sprite/celRamp_dm.png" },
			{ "tex:ramp-sm",	"../../../../resource/tex/sprite/celRamp_sm.png" },
			{ "tex:testsprite",	"../../../../resource/tex/sprite/spriteTest8x8.png" },
			{ "tex:checker",	"../../../../resource/tex/sprite/checker.png" },
		}
	};
	const a3ui32 numTextures = sizeof(textureList) / sizeof(a3_DemoStateTexture);
	a3_DemoStateTexture* const textureListPtr = (a3_DemoStateTexture*)(&textureList), * texturePtr;

	// ****TO-DO: 
	// load all textures
	for (i = 0; i < numTextures; ++i)
	{
		texturePtr = textureListPtr + i;

	}

	// ****TO-DO: 
	// change settings on a per-texture or per-type basis


	// done
}


// utility to load framebuffers
void a3demo_loadFramebuffers(a3_DemoState* demoState)
{
	// frame sizes
	const a3ui16 frameWidth1 = demoState->frameWidth, frameHeight1 = demoState->frameHeight;

	// ****LATER: 
	// create framebuffers and change their texture settings if need be


	// done
}


//-----------------------------------------------------------------------------

// the handle release callbacks are no longer valid; since the library was 
//	reloaded, old function pointers are out of scope!
// could reload everything, but that would mean rebuilding GPU data...
//	...or just set new function pointers!
void a3demo_loadValidate(a3_DemoState* demoState)
{
	// ****LATER: 

}


//-----------------------------------------------------------------------------
