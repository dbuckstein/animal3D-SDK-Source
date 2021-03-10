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

	a3_DemoMode3_Curves-idle-render.c
	Demo mode implementations: animation scene.

	********************************************
	*** RENDERING FOR ANIMATION SCENE MODE   ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode3_Curves.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoRenderUtils.h"


// OpenGL
#ifdef _WIN32
#include <gl/glew.h>
#include <Windows.h>
#include <GL/GL.h>
#else	// !_WIN32
#include <OpenGL/gl3.h>
#endif	// _WIN32


//-----------------------------------------------------------------------------

// controls for pipelines mode
void a3curves_render_controls(a3_DemoState const* demoState, a3_DemoMode3_Curves const* demoMode,
	a3_TextRenderer const* text, a3vec4 const col,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{
	// forward pipeline names
	a3byte const* renderModeName[curves_renderMode_max] = {
		"Phong, normal mapping",
		"Phong, parallax occlusion mapping",
		"Phong, level-of-detail",
	};

	// pipeline names
	a3byte const* renderPipelineName[curves_renderPipe_max] = {
		"Forward",
	};

	// render pass names
	a3byte const* renderPassName[curves_renderPass_max] = {
		"Scene",
		"Composite",
	};

	// render target names
	a3byte const* renderTargetName_scene[curves_renderTargetScene_max] = {
		"Color buffer 0 (final color)",
		"Color buffer 1 (normals [opt])",
		"Color buffer 2 (diffuse sample [opt])",
		"Color buffer 3 (specular sample [opt])",
		"Depth buffer",
	};
	a3byte const* renderTargetName_post[curves_renderTargetPost_max] = {
		"Color buffer 0 (final color)",
	};
	a3byte const* const* renderTargetName[curves_renderPass_max] = {
		renderTargetName_scene,
		renderTargetName_post,
	};

	// modes
	a3_DemoMode3_Curves_RenderMode const renderMode = demoMode->renderMode;
	a3_DemoMode3_Curves_RenderPass const renderPipeline = demoMode->renderPipeline;
	a3_DemoMode3_Curves_RenderPass const renderPass = demoMode->renderPass;
	a3_DemoMode3_Curves_RenderTarget const renderTarget = demoMode->renderTarget[renderPass],
		renderTargetCount = demoMode->renderTargetCount[renderPass];

	// lighting modes
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Render mode (%u / %u) ('j' | 'k'): %s", renderMode + 1, curves_renderMode_max, renderModeName[renderMode]);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Render pipeline (%u / %u) ('G' | 'H'): %s", renderPipeline + 1, curves_renderPipe_max, renderPipelineName[renderPipeline]);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"        Render pass (%u / %u) ('J' | 'K'): %s", renderPass + 1, curves_renderPass_max, renderPassName[renderPass]);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"            Render target (%u / %u) ('N' | 'M'): %s", renderTarget + 1, renderTargetCount, renderTargetName[renderPass][renderTarget]);
}


//-----------------------------------------------------------------------------

a3ret a3vertexDrawableRenderIsoPatches(a3ui32 const count)
{
	if (count)
	{
		// ****TO-DO: 
		//	-> set patch vertices parameter for isolines
		//	-> disable anything that would result in a VAO, VBO and/or IBO based render
		//	-> invoke rendering enough vertices to cover all path segments
		// force isoline patches

		return 1;
	}
	return -1;
}

// draw as patches
a3ret a3vertexDrawableRenderTriPatches(a3_VertexDrawable const* drawable)
{
	// triangle: use first 3 outer levels and only first inner
	// https://www.khronos.org/opengl/wiki/Tessellation 
	if (drawable)
	{
		// ****TO-DO: 
		//	-> set patch vertices parameter for triangles
		//	-> copy regular rendering algorithm
		//	-> replace primitive type with "patches" keyword
		// draw
		
		return 1;
	}
	return -1;
}
typedef a3ret(*a3_VertexDrawableRenderFunc)(a3_VertexDrawable const* drawable);


//-----------------------------------------------------------------------------

// sub-routine for rendering the demo state using the shading pipeline
void a3curves_render(a3_DemoState const* demoState, a3_DemoMode3_Curves const* demoMode, a3f64 const dt)
{
	// pointers
	const a3_DemoStateShaderProgram* currentDemoProgram;

	// framebuffers
	const a3_Framebuffer* currentWriteFBO, * currentDisplayFBO;

	// indices
	a3ui32 i = 0, j = 0;

	// RGB
	const a3vec4 rgba4[] = {
		{ 1.00f, 0.00f, 0.00f, 1.00f },	// red
		{ 1.00f, 0.25f, 0.00f, 1.00f },
		{ 1.00f, 0.50f, 0.00f, 1.00f },	// orange
		{ 1.00f, 0.75f, 0.00f, 1.00f },
		{ 1.00f, 1.00f, 0.00f, 1.00f },	// yellow
		{ 0.75f, 1.00f, 0.00f, 1.00f },
		{ 0.50f, 1.00f, 0.00f, 1.00f },	// lime
		{ 0.25f, 1.00f, 0.00f, 1.00f },
		{ 0.00f, 1.00f, 0.00f, 1.00f },	// green
		{ 0.00f, 1.00f, 0.25f, 1.00f },
		{ 0.00f, 1.00f, 0.50f, 1.00f },	// aqua
		{ 0.00f, 1.00f, 0.75f, 1.00f },
		{ 0.00f, 1.00f, 1.00f, 1.00f },	// cyan
		{ 0.00f, 0.75f, 1.00f, 1.00f },
		{ 0.00f, 0.50f, 1.00f, 1.00f },	// sky
		{ 0.00f, 0.25f, 1.00f, 1.00f },
		{ 0.00f, 0.00f, 1.00f, 1.00f },	// blue
		{ 0.25f, 0.00f, 1.00f, 1.00f },
		{ 0.50f, 0.00f, 1.00f, 1.00f },	// purple
		{ 0.75f, 0.00f, 1.00f, 1.00f },
		{ 1.00f, 0.00f, 1.00f, 1.00f },	// magenta
		{ 1.00f, 0.00f, 0.75f, 1.00f },
		{ 1.00f, 0.00f, 0.50f, 1.00f },	// rose
		{ 1.00f, 0.00f, 0.25f, 1.00f },
	};
	const a3vec4 grey4[] = {
		{ 0.5f, 0.5f, 0.5f, 1.0f },	// solid grey
		{ 0.5f, 0.5f, 0.5f, 0.5f },	// translucent grey
	};
	const a3ui32 hueCount = sizeof(rgba4) / sizeof(*rgba4),
		redIndex = 0, orangeIndex = 2, yellowIndex = 4, limeIndex = 6,
		greenIndex = 8, aquaIndex = 10, cyanIndex = 12, skyIndex = 14,
		blueIndex = 16, purpleIndex = 18, magentaIndex = 20, roseIndex = 22;
	const a3real
		* const red = rgba4[redIndex].v, * const orange = rgba4[orangeIndex].v, * const yellow = rgba4[yellowIndex].v, * const lime = rgba4[limeIndex].v,
		* const green = rgba4[greenIndex].v, * const aqua = rgba4[aquaIndex].v, * const cyan = rgba4[cyanIndex].v, * const sky = rgba4[skyIndex].v,
		* const blue = rgba4[blueIndex].v, * const purple = rgba4[purpleIndex].v, * const magenta = rgba4[magentaIndex].v, * const rose = rgba4[roseIndex].v,
		* const grey = grey4[0].v, * const grey_t = grey4[1].v;

	// camera used for drawing
	const a3_ProjectorComponent* activeCamera = demoMode->projector;
	const a3_SceneObjectComponent* activeCameraObject = activeCamera->sceneObjectPtr;
	const a3_SceneObjectComponent* currentSceneObject, * endSceneObject;

	// temp drawable pointers
	const a3_VertexDrawable* drawable[curvesMaxCount_sceneObject] = {
		0,								// root
		0,								// light
		0,								// camera
		0,								// curve
		demoState->draw_unit_box,		// skybox
		demoState->draw_unit_sphere,	// objects
		demoState->draw_teapot,
		demoState->draw_unit_plane_z,
	};

	// textures (diffuse, specular, normal, height)
	const a3_Texture* const* textureSet[curvesMaxCount_sceneObject] = {
		0, 0, 0, 0, 0,
		demoState->texSet_earth,	// sphere
		demoState->texSet_stone,	// teapot
		demoState->texSet_stone,	// ground
	};

	// height map scale
	const a3f32 htScale[curvesMaxCount_sceneObject] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.005f,	// sphere
		0.010f,	// teapot
		0.015f,	// ground
	};
	// tessellation levels
	const a3f32 tessLevel[curvesMaxCount_sceneObject][4] = {
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		{ 7.0f, 7.0f, 7.0f, 8.0f },
		{ 2.0f, 2.0f, 2.0f, 3.0f },
		{ 4.0f, 4.0f, 4.0f, 5.0f },
	}, tessLevelCurve[1][2] = {
		{ 1.0f, 32.0f }
	};

	// forward pipeline shader programs
	const a3_DemoStateShaderProgram* renderProgram[curves_renderMode_max] = {
		demoState->prog_drawPhongNM_ubo,
		demoState->prog_drawPhongPOM,
		demoState->prog_drawPhongLOD,
	};
	// overlay shader programs
	const a3_DemoStateShaderProgram* overlayProgram[curves_renderMode_max] = {
		demoState->prog_drawTangentBasisPOM,
		demoState->prog_drawTangentBasisPOM,
		demoState->prog_drawTangentBasisLOD,
	};
	// drawable render function
	const a3_VertexDrawableRenderFunc renderFunc[curves_renderMode_max] = {
		a3vertexDrawableActivateAndRender,
		a3vertexDrawableActivateAndRender,
		a3vertexDrawableRenderTriPatches,
	};
	// lights
	const a3ui32 renderModeLightCount[curves_renderMode_max] = {
		curvesMaxCount_pointLight,
		curvesMaxCount_pointLight,
		curvesMaxCount_pointLight,
	};

	// framebuffer target for each pass
	const a3_Framebuffer* writeFBO[curves_renderPass_max] = {
		demoState->fbo_c16x4_d24s8,			// scene
		demoState->fbo_c16x4,				// composite
	};

	// target info
	a3_DemoMode3_Curves_RenderMode const renderMode = demoMode->renderMode;
	a3_DemoMode3_Curves_RenderPipeline const renderPipeline = demoMode->renderPipeline;
	a3_DemoMode3_Curves_RenderPass const renderPass = demoMode->renderPass;
	a3_DemoMode3_Curves_RenderTarget const renderTarget = demoMode->renderTarget[renderPass],
		renderTargetCount = demoMode->renderTargetCount[renderPass];

	// model drawing function
	a3_VertexDrawableRenderFunc const a3vertexDrawableRender = renderFunc[renderMode];

	// final model matrix and full matrix stack
	a3mat4 projectionMat = activeCamera->projectorMatrixStackPtr->projectionMat;
	a3mat4 projectionMatInv = activeCamera->projectorMatrixStackPtr->projectionMatInverse;
	a3mat4 viewProjectionMat = activeCamera->projectorMatrixStackPtr->viewProjectionMat;
	a3mat4 modelMat, modelViewProjectionMat;

	// FSQ matrix
	const a3mat4 fsq = {
		2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 2.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};


	//-------------------------------------------------------------------------
	// 1) SCENE PASS: render scene with desired shader
	//	- activate scene framebuffer
	//	- draw scene
	//		- clear buffers
	//		- render shapes using appropriate shaders
	//		- capture color and depth

	// bind scene FBO
	currentWriteFBO = writeFBO[curves_renderPassScene]; //demoState->fbo_c16x4_d24s8
	a3framebufferActivate(currentWriteFBO);

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// stencil test
	//if (demoState->stencilTest)
	//	a3demo_drawStencilTest(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, demoState->prog_drawColorUnif, demoState->draw_unit_sphere);

	// select program based on settings
	currentDemoProgram = renderProgram[renderMode];
	a3shaderProgramActivate(currentDemoProgram->program);

	// send shared data: 
	//	- projection matrix
	//	- light data
	//	- activate shared textures including atlases if using
	//	- shared animation data
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP, 1, projectionMat.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP_inv, 1, projectionMatInv.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
	a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor0, hueCount, rgba4->v);
	if (demoState->updateAnimation)
		a3shaderUniformSendDouble(a3unif_single, currentDemoProgram->uTime, 1, &demoState->timer_display->totalTime);

	// send lighting uniforms and bind blocks where appropriate
	a3shaderUniformBufferActivate(demoState->ubo_transform, demoProg_blockTransformStack);
	a3shaderUniformBufferActivate(demoState->ubo_light, demoProg_blockLight);
	a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uCount, 1, renderModeLightCount + renderMode);

	// select pipeline algorithm
	glDisable(GL_BLEND);

	// forward shading algorithms
	for (currentSceneObject = demoMode->obj_sphere, endSceneObject = demoMode->obj_ground;
		currentSceneObject <= endSceneObject; ++currentSceneObject)
	{
		// send index as uniform and draw; all other model data is shared
		j = currentSceneObject->sceneHierarchyIndex;
		a3textureActivate(textureSet[j][0], a3tex_unit00);
		a3textureActivate(textureSet[j][1], a3tex_unit01);
		a3textureActivate(textureSet[j][2], a3tex_unit02);
		a3textureActivate(textureSet[j][3], a3tex_unit03);
		a3shaderUniformSendFloat(a3unif_vec3, currentDemoProgram->uLevelOuter, 1, tessLevel[j]);
		a3shaderUniformSendFloat(a3unif_single, currentDemoProgram->uLevelInner, 1, tessLevel[j] + 3);
		a3shaderUniformSendFloat(a3unif_single, currentDemoProgram->uSize, 1, htScale + j);
		a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uIndex, 1, &j);
		a3vertexDrawableRender(drawable[j]);
	}

	// stop using stencil
	if (demoState->stencilTest)
		glDisable(GL_STENCIL_TEST);


	//-------------------------------------------------------------------------
	// COMPOSITION
	//	- activate target framebuffer
	//	- draw background if applicable

	// skybox
	currentWriteFBO = writeFBO[curves_renderPassComposite];
	a3framebufferActivate(currentWriteFBO);
	if (demoState->displaySkybox)
	{
		// skybox clear: just draw skybox
		modelMat = demoMode->obj_skybox->modelMatrixStackPtr->modelMat;
		a3demo_drawModelTexturedColored_invertModel(
			modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, a3mat4_identity.m,
			demoState->prog_drawTexture, demoState->draw_unit_box, demoState->tex_skybox_clouds, a3vec4_one.v);
	}
	else
	{
		// clear all color buffers
		glClear(GL_COLOR_BUFFER_BIT);
	}

	switch (renderPipeline)
	{
	case curves_renderPipeForward:
		// forward shading
		//	- paste scene image in front of skybox
		currentDemoProgram = demoState->prog_drawTexture;
		a3shaderProgramActivate(currentDemoProgram->program);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, fsq.mm);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
		a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, a3vec4_one.v);
		a3framebufferBindColorTexture(demoState->fbo_c16x4_d24s8, a3tex_unit00, 0);	// scene color
		break;
	}


	//-------------------------------------------------------------------------
	// PREPARE FOR POST-PROCESSING
	//	- double buffer swap (if applicable)
	//	- ensure blending is disabled
	//	- re-activate FSQ drawable IF NEEDED (i.e. changed in previous step)

	// draw FSQ to complete forward/deferred composition pipeline
	a3demo_enableCompositeBlending();
	a3vertexDrawableActivateAndRender(demoState->draw_unit_plane_z);
	glDisable(GL_BLEND);


	//-------------------------------------------------------------------------
	// POST-PROCESSING
	//	- activate target framebuffer
	//	- activate texture from previous framebuffer
	//	- draw FSQ with processing program active


	//-------------------------------------------------------------------------
	// DISPLAY: final pass, perform and present final composite
	//	- finally draw to back buffer
	//	- select display texture(s)
	//	- activate final pass program
	//	- draw final FSQ

	// revert to back buffer and disable depth testing
	a3framebufferDeactivateSetViewport(a3fbo_depthDisable,
		-demoState->frameBorder, -demoState->frameBorder, demoState->frameWidth, demoState->frameHeight);

	// select framebuffer to display based on mode
	currentDisplayFBO = writeFBO[renderPass];

	// select output to display
	switch (renderPass)
	{
	case curves_renderPassScene:
		if (currentDisplayFBO->color && (!currentDisplayFBO->depthStencil || renderTarget < renderTargetCount - 1))
			a3framebufferBindColorTexture(currentDisplayFBO, a3tex_unit00, renderTarget);
		else
			a3framebufferBindDepthTexture(currentDisplayFBO, a3tex_unit00);
		break;
	case curves_renderPassComposite:
		a3framebufferBindColorTexture(currentDisplayFBO, a3tex_unit00, renderTarget);
		break;
	}

	// final display: activate desired final program and draw FSQ
	if (currentDisplayFBO)
	{
		// determine if additional passes are required
		// set appropriate display program
		currentDemoProgram = demoState->prog_drawTexture;
		a3shaderProgramActivate(currentDemoProgram->program);

		// done
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, fsq.mm);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
		a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, a3vec4_one.v);
		a3vertexDrawableRenderActive();
	}


	//-------------------------------------------------------------------------
	// OVERLAYS: done after FSQ so they appear over everything else
	//	- disable depth testing
	//	- draw overlays appropriately

	// enable alpha
	a3demo_enableCompositeBlending();

	// scene overlays
	if (demoState->displayGrid || demoState->displayTangentBases || demoState->displayWireframe)
	{
		// activate scene FBO and clear color; reuse depth
		currentWriteFBO = demoState->fbo_c16x4_d24s8;
		a3framebufferActivate(currentWriteFBO);
		glDisable(GL_STENCIL_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw grid aligned to world
		if (demoState->displayGrid)
		{
			// actual grid
			a3demo_drawModelSolidColor(modelViewProjectionMat.m, viewProjectionMat.m, a3mat4_identity.m, demoState->prog_drawColorUnif, demoState->draw_grid, blue);

			// curve
			currentDemoProgram = demoState->prog_drawCurvePath;
			a3shaderProgramActivate(currentDemoProgram->program);
			a3shaderUniformBufferActivate(demoState->ubo_curve, demoProg_blockCurve);
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP, 1, viewProjectionMat.mm);
			a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uLevelOuter, 1, tessLevelCurve[0]);
			a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uCount, 1, &demoMode->curveWaypointCount);
			a3vertexDrawableRenderIsoPatches(demoMode->curveWaypointCount);
		}

		if (demoState->displayTangentBases || demoState->displayWireframe)
		{
			const a3i32 flag[1] = { demoState->displayTangentBases * 3 + demoState->displayWireframe * 4 };
			const a3f32 size[1] = { 0.0625f };

			currentDemoProgram = overlayProgram[renderMode];
			a3shaderProgramActivate(currentDemoProgram->program);

			// send lighting uniforms and bind blocks where appropriate
			a3shaderUniformBufferActivate(demoState->ubo_transform, demoProg_blockTransformStack);
			// projection matrix
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP, 1, projectionMat.mm);
			// wireframe color
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor0, hueCount, rgba4->v);
			// blend color
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, a3vec4_one.v);
			// tangent basis size
			a3shaderUniformSendFloat(a3unif_single, currentDemoProgram->uSize, 1, size);
			// overlay flag
			a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uFlag, 1, flag);

			for (currentSceneObject = demoMode->obj_sphere, endSceneObject = demoMode->obj_ground;
				currentSceneObject <= endSceneObject; ++currentSceneObject)
			{
				j = currentSceneObject->sceneHierarchyIndex;
				a3textureActivate(textureSet[j][3], a3tex_unit03);
				a3shaderUniformSendFloat(a3unif_vec3, currentDemoProgram->uLevelOuter, 1, tessLevel[j]);
				a3shaderUniformSendFloat(a3unif_single, currentDemoProgram->uLevelInner, 1, tessLevel[j] + 3);
				a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uIndex, 1, &j);
				a3vertexDrawableRender(drawable[j]);
			}
		}

		// display color target with scene overlays
		a3framebufferDeactivateSetViewport(a3fbo_depthDisable,
			-demoState->frameBorder, -demoState->frameBorder, demoState->frameWidth, demoState->frameHeight);
		currentDemoProgram = demoState->prog_drawTexture;
		a3vertexDrawableActivate(demoState->draw_unit_plane_z);
		a3shaderProgramActivate(currentDemoProgram->program);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, fsq.mm);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
		a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, a3vec4_one.v);
		a3framebufferBindColorTexture(currentWriteFBO, a3tex_unit00, 0);
		a3vertexDrawableRenderActive();
	}

	// overlays with no depth
	glDisable(GL_DEPTH_TEST);

	// hidden volumes
	if (demoState->displayHiddenVolumes)
	{
		// lights
		currentDemoProgram = demoState->prog_drawColorUnif;
		a3shaderProgramActivate(currentDemoProgram->program);
		a3vertexDrawableActivate(demoState->draw_unit_sphere);
		a3real4x4SetScale(modelMat.m, 0.25f);
		for (i = 0; i < curvesMaxCount_pointLight; ++i)
		{
			modelMat.v3 = demoMode->pointLightData[i].worldPos;
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, demoMode->pointLightData[i].color.v);
			a3demo_drawModelSimple(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, currentDemoProgram);
		}
	}

	// superimpose axes
	// draw coordinate axes in front of everything
	currentDemoProgram = demoState->prog_drawColorAttrib;
	a3shaderProgramActivate(currentDemoProgram->program);
	a3vertexDrawableActivate(demoState->draw_axes);

	// center of world from current viewer
	// also draw other viewer/viewer-like object in scene
	if (demoState->displayWorldAxes)
	{
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, viewProjectionMat.mm);
		a3vertexDrawableRenderActive();
	}

	// individual objects (based on scene graph)
	if (demoState->displayObjectAxes)
	{
		for (currentSceneObject = demoMode->obj_sphere, endSceneObject = demoMode->obj_ground;
			currentSceneObject <= endSceneObject; ++currentSceneObject)
		{
			j = currentSceneObject->sceneHierarchyIndex;
			modelMat = currentSceneObject->modelMatrixStackPtr->modelMat;
			a3demo_drawModelSimple(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, currentDemoProgram);
		}
	}
}


//-----------------------------------------------------------------------------
