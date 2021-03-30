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

	a3_DemoMode4_Animate.h
	Demo mode interface: animated objects scene.

	********************************************
	*** THIS IS ONE DEMO MODE'S HEADER FILE  ***
	********************************************
*/

#ifndef __ANIMAL3D_DEMOMODE4_ANIMATE_H
#define __ANIMAL3D_DEMOMODE4_ANIMATE_H


//-----------------------------------------------------------------------------

#include "_a3_demo_utilities/a3_DemoSceneObject.h"

#include "_animation/a3_Hierarchy.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_DemoMode4_Animate						a3_DemoMode4_Animate;
typedef enum a3_DemoMode4_Animate_RenderMode			a3_DemoMode4_Animate_RenderMode;
typedef enum a3_DemoMode4_Animate_RenderPipeline		a3_DemoMode4_Animate_RenderPipeline;
typedef enum a3_DemoMode4_Animate_RenderPass			a3_DemoMode4_Animate_RenderPass;
typedef enum a3_DemoMode4_Animate_RenderTarget			a3_DemoMode4_Animate_RenderTarget;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// render modes
enum a3_DemoMode4_Animate_RenderMode
{
	animate_renderModeDefault,

	animate_renderMode_max
};


// render pipelines
enum a3_DemoMode4_Animate_RenderPipeline
{
	animate_renderPipeForward,		// forward pipeline

	animate_renderPipe_max
};


// render passes
enum a3_DemoMode4_Animate_RenderPass
{
	animate_renderPassScene,
	animate_renderPassComposite,

	animate_renderPass_max
};


// render targets
enum a3_DemoMode4_Animate_RenderTarget
{
	// scene targets
	animate_renderTargetSceneColor = 0,
	animate_renderTargetSceneNormal,
	animate_renderTargetSceneDiffuseSample,
	animate_renderTargetSceneSpecularSample,
	animate_renderTargetSceneDepth,
	animate_renderTargetScene_max,

	// post-processing targets
	animate_renderTargetPostColor = 0,
	animate_renderTargetPost_max,
};


// maximum unique objects
enum a3_DemoMode4_Animate_ObjectMaxCount
{
	animateMaxCount_sceneObject = 8,
	animateMaxCount_projector = 1,	// how many of the above behave as projectors
	animateMaxCount_pointLight = 1,	// how many of the above behave as lights

	animateMaxCount_skeletonPose = 4,
	animateMaxCount_skeletonJoint = 128,
};


//-----------------------------------------------------------------------------

typedef struct a3_KeyframeController
{
	a3f32 duration, durationInv;
	a3f32 time, param;
	a3ui32 index, count;
} a3_KeyframeController;

typedef enum a3_TransformChannel
{
	channel_none = 0x0000,
	channel_rotate_x = 0x0001,
	channel_rotate_y = 0x0002,
	channel_rotate_z = 0x0004,
	channel_rotate_w = 0x0008,
	channel_rotate_xy = channel_rotate_x | channel_rotate_y,
	channel_rotate_yz = channel_rotate_y | channel_rotate_z,
	channel_rotate_zx = channel_rotate_z | channel_rotate_x,
	channel_rotate_xyz = channel_rotate_xy | channel_rotate_z,
	channel_translate_x = 0x0010,
	channel_translate_y = 0x0020,
	channel_translate_z = 0x0040,
	channel_translate_w = 0x0080,
	channel_translate_xy = channel_translate_x | channel_translate_y,
	channel_translate_yz = channel_translate_y | channel_translate_z,
	channel_translate_zx = channel_translate_z | channel_translate_x,
	channel_translate_xyz = channel_translate_xy | channel_translate_z,
	channel_scale_x = 0x0100,
	channel_scale_y = 0x0200,
	channel_scale_z = 0x0400,
	channel_scale_w = 0x0800,
	channel_scale_xy = channel_scale_x | channel_scale_y,
	channel_scale_yz = channel_scale_y | channel_scale_z,
	channel_scale_zx = channel_scale_z | channel_scale_x,
	channel_scale_xyz = channel_scale_xy | channel_scale_z,
} a3_TransformChannel;


//-----------------------------------------------------------------------------

// demo mode for basic shading
struct a3_DemoMode4_Animate
{
	// render mode
	a3_DemoMode4_Animate_RenderMode renderMode;

	// render pipeline
	a3_DemoMode4_Animate_RenderPipeline renderPipeline;

	// render pass
	a3_DemoMode4_Animate_RenderPass renderPass;

	// render targets
	a3_DemoMode4_Animate_RenderTarget renderTarget[animate_renderPass_max], renderTargetCount[animate_renderPass_max];

	// scene hierarchy and data
	a3_Hierarchy hierarchy_scene[1];

	// animation data
	a3_KeyframeController animMorphTeapot[1], animPoseSkel[1];
	a3_Hierarchy hierarchy_skel[1];
	a3ui32 hierarchyDepth_skel[animateMaxCount_skeletonJoint];
	a3_SceneObjectData skeletonPose[animateMaxCount_skeletonPose][animateMaxCount_skeletonJoint];
	a3_TransformChannel skeletonChannel[animateMaxCount_skeletonJoint];
	a3mat4 skeletonPose_local[animateMaxCount_skeletonJoint], skeletonPose_object[animateMaxCount_skeletonJoint],
		skeletonPose_render[animateMaxCount_skeletonJoint], skeletonPose_renderAxes[animateMaxCount_skeletonJoint];

	// scene object components and related data
	union {
		a3_SceneObjectComponent sceneObject[animateMaxCount_sceneObject];
		struct {
			a3_SceneObjectComponent
				objgroup_world_root[1],
				obj_light_main[1],
				obj_camera_main[1];
			a3_SceneObjectComponent
				obj_skybox[1],
				obj_skeleton[1],
				obj_teapot[1],
				obj_ground[1];
		};
	};
	a3_SceneObjectData sceneObjectData[animateMaxCount_sceneObject];
	a3_ModelMatrixStack modelMatrixStack[animateMaxCount_sceneObject];

	// projector components and related data
	union {
		a3_ProjectorComponent projector[animateMaxCount_projector];
		struct {
			a3_ProjectorComponent
				proj_camera_main[1];
		};
	};
	a3_ProjectorData projectorData[animateMaxCount_projector];
	a3_ProjectorMatrixStack projectorMatrixStack[animateMaxCount_projector];

	// light components and related data
	union {
		a3_PointLightComponent pointLight[animateMaxCount_pointLight];
		struct {
			a3_PointLightComponent
				light_point_main[1];
		};
	};
	a3_PointLightData pointLightData[animateMaxCount_pointLight];
};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMOMODE4_ANIMATE_H