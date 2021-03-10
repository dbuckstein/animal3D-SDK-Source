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

	a3_DemoMode3_Curves.h
	Demo mode interface: animation editor scene.

	********************************************
	*** THIS IS ONE DEMO MODE'S HEADER FILE  ***
	********************************************
*/

#ifndef __ANIMAL3D_DEMOMODE3_CURVES_H
#define __ANIMAL3D_DEMOMODE3_CURVES_H


//-----------------------------------------------------------------------------

#include "_a3_demo_utilities/a3_DemoSceneObject.h"

#include "_animation/a3_Hierarchy.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_DemoMode3_Curves						a3_DemoMode3_Curves;
typedef enum a3_DemoMode3_Curves_RenderMode				a3_DemoMode3_Curves_RenderMode;
typedef enum a3_DemoMode3_Curves_RenderPipeline			a3_DemoMode3_Curves_RenderPipeline;
typedef enum a3_DemoMode3_Curves_RenderPass				a3_DemoMode3_Curves_RenderPass;
typedef enum a3_DemoMode3_Curves_RenderTarget			a3_DemoMode3_Curves_RenderTarget;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// render modes
enum a3_DemoMode3_Curves_RenderMode
{
	curves_renderModePhong,		// Phong shading model
	curves_renderModePhongPOM,	// Phong shading model with parallax occlusion mapping
	curves_renderModePhongLOD,	// Phong shading model with level-of-detail

	curves_renderMode_max
};


// render pipelines
enum a3_DemoMode3_Curves_RenderPipeline
{
	curves_renderPipeForward,		// forward pipeline

	curves_renderPipe_max
};


// render passes
enum a3_DemoMode3_Curves_RenderPass
{
	curves_renderPassScene,
	curves_renderPassComposite,

	curves_renderPass_max
};


// render targets
enum a3_DemoMode3_Curves_RenderTarget
{
	// scene targets
	curves_renderTargetSceneColor = 0,
	curves_renderTargetSceneNormal,
	curves_renderTargetSceneDiffuseSample,
	curves_renderTargetSceneSpecularSample,
	curves_renderTargetSceneDepth,
	curves_renderTargetScene_max,

	// post-processing targets
	curves_renderTargetPostColor = 0,
	curves_renderTargetPost_max,
};


// maximum unique objects
enum a3_DemoMode3_Curves_ObjectMaxCount
{
	curvesMaxCount_sceneObject = 8,
	curvesMaxCount_projector = 1,	// how many of the above behave as projectors
	curvesMaxCount_pointLight = 1,	// how many of the above behave as lights

	curvesMaxCount_waypoint = 32,
};


//-----------------------------------------------------------------------------

// demo mode for basic shading
struct a3_DemoMode3_Curves
{
	// render mode
	a3_DemoMode3_Curves_RenderMode renderMode;

	// render pipeline
	a3_DemoMode3_Curves_RenderPipeline renderPipeline;

	// render pass
	a3_DemoMode3_Curves_RenderPass renderPass;

	// render targets
	a3_DemoMode3_Curves_RenderTarget renderTarget[curves_renderPass_max], renderTargetCount[curves_renderPass_max];

	// scene hierarchy and data
	a3_Hierarchy hierarchy_scene[1];

	// waypoint and handle data
	a3vec4 curveWaypoint[curvesMaxCount_waypoint], curveTangent[curvesMaxCount_waypoint];
	a3ui32 curveWaypointCount;

	// animation data for teapot
	a3f32 curveSegmentDuration, curveSegmentDurationInv, curveSegmentTime, curveSegmentParam;
	a3ui32 curveSegmentIndex;

	// scene object components and related data
	union {
		a3_SceneObjectComponent sceneObject[curvesMaxCount_sceneObject];
		struct {
			a3_SceneObjectComponent
				objgroup_world_root[1],
				obj_light_main[1],
				obj_camera_main[1];
			a3_SceneObjectComponent
				obj_curve[1];
			a3_SceneObjectComponent
				obj_skybox[1],
				obj_sphere[1],
				obj_teapot[1],
				obj_ground[1];
		};
	};
	a3_SceneObjectData sceneObjectData[curvesMaxCount_sceneObject];
	a3_ModelMatrixStack modelMatrixStack[curvesMaxCount_sceneObject];

	// projector components and related data
	union {
		a3_ProjectorComponent projector[curvesMaxCount_projector];
		struct {
			a3_ProjectorComponent
				proj_camera_main[1];
		};
	};
	a3_ProjectorData projectorData[curvesMaxCount_projector];
	a3_ProjectorMatrixStack projectorMatrixStack[curvesMaxCount_projector];

	// light components and related data
	union {
		a3_PointLightComponent pointLight[curvesMaxCount_pointLight];
		struct {
			a3_PointLightComponent
				light_point_main[1];
		};
	};
	a3_PointLightData pointLightData[curvesMaxCount_pointLight];
};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMOMODE3_CURVES_H