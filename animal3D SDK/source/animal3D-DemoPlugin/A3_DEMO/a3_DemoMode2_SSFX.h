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

	a3_DemoMode2_SSFX.h
	Demo mode interface: animation editor scene.

	********************************************
	*** THIS IS ONE DEMO MODE'S HEADER FILE  ***
	********************************************
*/

#ifndef __ANIMAL3D_DEMOMODE2_SSFX_H
#define __ANIMAL3D_DEMOMODE2_SSFX_H


//-----------------------------------------------------------------------------

#include "_a3_demo_utilities/a3_DemoSceneObject.h"

#include "_animation/a3_Hierarchy.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_DemoMode2_SSFX						a3_DemoMode2_SSFX;
typedef enum a3_DemoMode2_SSFX_RenderMode				a3_DemoMode2_SSFX_RenderMode;
typedef enum a3_DemoMode2_SSFX_RenderPass				a3_DemoMode2_SSFX_RenderPass;
typedef enum a3_DemoMode2_SSFX_RenderTarget				a3_DemoMode2_SSFX_RenderTarget;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// render modes
enum a3_DemoMode2_SSFX_RenderMode
{
	ssfx_renderModePhong,			// Phong shading model
	ssfx_renderModePhongDS,			// Phong shading model deferred
	ssfx_renderModePhongDL,			// Phong shading model deferred with light pre-pass

	ssfx_renderMode_max
};


// render passes
enum a3_DemoMode2_SSFX_RenderPass
{
	ssfx_renderPassScene,
	ssfx_renderPassLights,
	ssfx_renderPassComposite,

	ssfx_renderPassBright2,
	ssfx_renderPassBlurH2,
	ssfx_renderPassBlurV2,
	ssfx_renderPassBright4,
	ssfx_renderPassBlurH4,
	ssfx_renderPassBlurV4,
	ssfx_renderPassBright8,
	ssfx_renderPassBlurH8,
	ssfx_renderPassBlurV8,
	ssfx_renderPassDisplay,

	ssfx_renderPass_max
};


// render targets
enum a3_DemoMode2_SSFX_RenderTarget
{
	// scene targets
	ssfx_renderTargetSceneColor = 0,
	ssfx_renderTargetSceneNormal,
	ssfx_renderTargetSceneDiffuseSample,
	ssfx_renderTargetSceneSpecularSample,
	ssfx_renderTargetSceneDepth,
	ssfx_renderTargetScene_max,

	// deferred lighting targets
	ssfx_renderTargetLightDiffuseShading = 0,
	ssfx_renderTargetLightSpecularShading,
	ssfx_renderTargetLight_max,

	// post-processing targets
	ssfx_renderTargetPostColor = 0,
	ssfx_renderTargetPost_max,
};


// maximum unique objects
enum a3_DemoMode2_SSFX_ObjectMaxCount
{
	ssfxMaxCount_sceneObject = 16,
	ssfxMaxCount_projector = 1,		// how many of the above behave as projectors

	ssfxMaxCount_pointLight = 1024,	// how many standalone lights
};


//-----------------------------------------------------------------------------

// demo mode for basic shading
struct a3_DemoMode2_SSFX
{
	// render mode
	a3_DemoMode2_SSFX_RenderMode renderMode;

	// render pass
	a3_DemoMode2_SSFX_RenderPass renderPass;

	// render targets
	a3_DemoMode2_SSFX_RenderTarget renderTarget[ssfx_renderPass_max], renderTargetCount[ssfx_renderPass_max];

	// scene hierarchy and data
	a3_Hierarchy hierarchy_scene[1];

	// scene object components and related data
	union {
		a3_SceneObjectComponent sceneObject[ssfxMaxCount_sceneObject];
		struct {
			a3_SceneObjectComponent
				objgroup_world_root[1],
				obj_camera_main[1];
			a3_SceneObjectComponent
				obj_skybox[1],
				obj_sphere[1],
				obj_cylinder[1],
				obj_capsule[1],
				obj_torus[1],
				obj_cone[1],
				obj_teapot[1],
				obj_ground[1];
		};
	};
	a3_SceneObjectData sceneObjectData[ssfxMaxCount_sceneObject];
	a3_ModelMatrixStack modelMatrixStack[ssfxMaxCount_sceneObject];

	// projector components and related data
	union {
		a3_ProjectorComponent projector[ssfxMaxCount_projector];
		struct {
			a3_ProjectorComponent
				proj_camera_main[1];
		};
	};
	a3_ProjectorData projectorData[ssfxMaxCount_projector];
	a3_ProjectorMatrixStack projectorMatrixStack[ssfxMaxCount_projector];

	// light components and related data
	//union {
	//	a3_PointLightComponent pointLight[ssfxMaxCount_pointLight];
	//	struct {
	//	};
	//};
	a3_PointLightData pointLightData[ssfxMaxCount_pointLight];
	a3mat4 pointLightMVP[ssfxMaxCount_pointLight];
};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMOMODE2_SSFX_H