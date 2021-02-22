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

	a3_DemoMode2_SSFX-idle-render.c
	Demo mode implementations: animation scene.

	********************************************
	*** RENDERING FOR ANIMATION SCENE MODE   ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode2_SSFX.h"

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
void a3ssfx_render_controls(a3_DemoState const* demoState, a3_DemoMode2_SSFX const* demoMode,
	a3_TextRenderer const* text, a3vec4 const col,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{
	// forward pipeline names
	a3byte const* renderModeName[ssfx_renderMode_max] = {
		"Phong shading (forward shading)",
		"Phong shading (deferred shading)",
		"Phong shading (deferred light pre-pass)",
	};

	// render pass names
	a3byte const* renderPassName[ssfx_renderPass_max] = {
		"Scene/g-buffers",
		"Light pre-pass",
		"Composition",
		"Bright pass (half)",
		"Blur pass, 1st axis (half)",
		"Blur pass, 2nd axis (half)",
		"Bright pass (quarter)",
		"Blur pass, 1st axis (quarter)",
		"Blur pass, 2nd axis (quarter)",
		"Bright pass (eighth)",
		"Blur pass, 1st axis (eighth)",
		"Blur pass, 2nd axis (eighth)",
		"Final composite (bloom)",
	};

	// render target names
	a3byte const* renderTargetName_scene[ssfx_renderTargetScene_max] = {
		"Color buffer 0 (color/texcoords)",
		"Color buffer 1 (normals)",
		"Color buffer 2 (diffuse sample)",
		"Color buffer 3 (specular sample)",
		"Depth buffer",
	};
	a3byte const* renderTargetName_light[ssfx_renderTargetLight_max] = {
		"Diffuse shading total",
		"Specular shading total",
	};
	a3byte const* renderTargetName_post[ssfx_renderTargetPost_max] = {
		"Color buffer",
	};
	a3byte const* const* renderTargetName[ssfx_renderPass_max] = {
		renderTargetName_scene,
		renderTargetName_light,
		renderTargetName_post,
		renderTargetName_post,
		renderTargetName_post,
		renderTargetName_post,
		renderTargetName_post,
		renderTargetName_post,
		renderTargetName_post,
		renderTargetName_post,
		renderTargetName_post,
		renderTargetName_post,
		renderTargetName_post,
	};

	// modes
	a3_DemoMode2_SSFX_RenderMode const renderMode = demoMode->renderMode;
	a3_DemoMode2_SSFX_RenderPass const renderPass = demoMode->renderPass;
	a3_DemoMode2_SSFX_RenderTarget const renderTarget = demoMode->renderTarget[renderPass],
		renderTargetCount = demoMode->renderTargetCount[renderPass];

	// lighting modes
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Render mode (%u / %u) ('j' | 'k'): %s", renderMode + 1, ssfx_renderMode_max, renderModeName[renderMode]);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Render pass (%u / %u) ('J' | 'K'): %s", renderPass + 1, ssfx_renderPass_max, renderPassName[renderPass]);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"        Render target (%u / %u) ('N' | 'M'): %s", renderTarget + 1, renderTargetCount, renderTargetName[renderPass][renderTarget]);
}


//-----------------------------------------------------------------------------

// sub-routine for rendering the demo state using the shading pipeline
void a3ssfx_render(a3_DemoState const* demoState, a3_DemoMode2_SSFX const* demoMode, a3f64 const dt)
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
	const a3_VertexDrawable* drawable[] = {
		0,								// root
		0,								// camera
		demoState->draw_unit_box,		// skybox
		demoState->draw_unit_sphere,	// objects
		demoState->draw_unit_cylinder,
		demoState->draw_unit_capsule,
		demoState->draw_unit_torus,
		demoState->draw_unit_cone,
		demoState->draw_teapot,
		demoState->draw_unit_plane_z,
	};

	// forward pipeline shader programs
	const a3_DemoStateShaderProgram* renderProgram[ssfx_renderMode_max] = {
		demoState->prog_drawPhongNM_ubo,
		demoState->prog_drawGBuffers,
		demoState->prog_drawGBuffers,
	};
	const a3ui32 renderModeLightCount[ssfx_renderMode_max] = {
		ssfxMaxCount_pointLight,
		ssfxMaxCount_pointLight,
		ssfxMaxCount_pointLight,
	};

	// framebuffer target for each pass
	const a3_Framebuffer* writeFBO[ssfx_renderPass_max] = {
		demoState->fbo_c16x4_d24s8,			// scene
		demoState->fbo_c16x4 + 0,			// light pre-pass
		demoState->fbo_c16x4 + 1,			// deferred composite
		demoState->fbo_c16_szHalf + 0,		// bright
		demoState->fbo_c16_szHalf + 1,		// blur
		demoState->fbo_c16_szHalf + 2,		
		demoState->fbo_c16_szQuarter + 0,
		demoState->fbo_c16_szQuarter + 1,
		demoState->fbo_c16_szQuarter + 2,
		demoState->fbo_c16_szEighth + 0,
		demoState->fbo_c16_szEighth + 1,
		demoState->fbo_c16_szEighth + 2,
		demoState->fbo_c16x4 + 3,			// final composite
	};

	// target info
	a3_DemoMode2_SSFX_RenderMode const renderMode = demoMode->renderMode;
	a3_DemoMode2_SSFX_RenderPass const renderPass = demoMode->renderPass;
	a3_DemoMode2_SSFX_RenderTarget const renderTarget = demoMode->renderTarget[renderPass],
		renderTargetCount = demoMode->renderTargetCount[renderPass];

	// final model matrix and full matrix stack
	a3mat4 projectionMat = activeCamera->projectorMatrixStackPtr->projectionMat;
//	a3mat4 projectionBiasMatInv = activeCamera->projectorMatrixStackPtr->projectionBiasMatInverse;
	a3mat4 viewProjectionMat = activeCamera->projectorMatrixStackPtr->viewProjectionMat;
	a3mat4 modelMat, modelViewMat, modelViewProjectionMat;

	// FSQ matrix
	const a3mat4 fsq = {
		2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 2.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// pixel size and effect axis
	a3vec2 pixelSize = a3vec2_one;


	//-------------------------------------------------------------------------
	// 1) SCENE PASS: render scene with desired shader
	//	- activate scene framebuffer
	//	- draw scene
	//		- clear buffers
	//		- render shapes using appropriate shaders
	//		- capture color and depth

	// bind scene FBO
	currentWriteFBO = writeFBO[ssfx_renderPassScene]; //demoState->fbo_c16x4_d24s8
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
	a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor0, hueCount, rgba4->v);
	if (demoState->updateAnimation)
		a3shaderUniformSendDouble(a3unif_single, currentDemoProgram->uTime, 1, &demoState->timer_display->totalTime);

	// send lighting uniforms and bind blocks where appropriate
	a3shaderUniformBufferActivate(demoState->ubo_transform, demoProg_blockTransformStack);
	a3shaderUniformBufferActivate(demoState->ubo_light, demoProg_blockLight);
	a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uCount, 1, renderModeLightCount + renderMode);

	// activate texture atlases
	a3textureActivate(demoState->tex_atlas_dm, a3tex_unit00);
	a3textureActivate(demoState->tex_atlas_sm, a3tex_unit01);
	a3textureActivate(demoState->tex_atlas_nm, a3tex_unit02);
	a3textureActivate(demoState->tex_atlas_hm, a3tex_unit03);

	// select pipeline algorithm
	glDisable(GL_BLEND);

	// forward shading algorithms
	for (currentSceneObject = demoMode->obj_sphere, endSceneObject = demoMode->obj_ground;
		currentSceneObject <= endSceneObject; ++currentSceneObject)
	{
		// send index as uniform and draw; all other model data is shared
		j = currentSceneObject->sceneHierarchyIndex;
		a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uIndex, 1, &j);
		a3vertexDrawableActivateAndRender(drawable[j]);
	}

	// stop using stencil
	if (demoState->stencilTest)
		glDisable(GL_STENCIL_TEST);


	//-------------------------------------------------------------------------
	// DEFERRED LIGHTING PRE-PASS
	//	- bind normal and depth textures from previous pass for lighting
	//	- draw a bunch of light volumes, inverted and additively

	if (renderMode == ssfx_renderModePhongDL)
	{
		// ****TO-DO:
		//	-> uncomment deferred shading program and diffuse texture activations
		//	-> activate pertinent textures for deferred lighting composition
		//		(hint: all outputs from previous passes)
		//	-> activate and send pertinent uniform blocks and values
		//		(hint: light buffer, light transforms, inverse bias-projection)
		//		(hint: inverse bias-projection variable is commented out above)
	/*	// draw light volumes
		currentDemoProgram = demoState->prog_drawPhongPointLight_instanced;
		a3shaderProgramActivate(currentDemoProgram->program);
		//...*/

		currentWriteFBO = writeFBO[ssfx_renderPassLights];
		a3framebufferActivate(currentWriteFBO);
		glClear(GL_COLOR_BUFFER_BIT);

		// ****TO-DO:
		//	-> draw many inverted instances of the unit sphere model (because 
		//		point lights are spheres), and using additive blending

	}


	//-------------------------------------------------------------------------
	// COMPOSITION
	//	- activate target framebuffer
	//	- draw background if applicable

	// skybox
	currentWriteFBO = writeFBO[ssfx_renderPassComposite];
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

	switch (renderMode)
	{
	case ssfx_renderModePhong:
		// forward shading
		//	- paste scene image in front of skybox
		currentDemoProgram = demoState->prog_drawTexture;
		a3shaderProgramActivate(currentDemoProgram->program);
		a3framebufferBindColorTexture(demoState->fbo_c16x4_d24s8, a3tex_unit00, 0);	// scene color
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, fsq.mm);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
		a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, a3vec4_one.v);
		break;
	case ssfx_renderModePhongDS:
		// ****TO-DO:
		//	-> uncomment deferred shading program and diffuse texture activations
		//	-> activate pertinent textures for deferred lighting composition
		//		(hint: all outputs from previous passes)
		//	-> activate and send pertinent uniform blocks and values
		//		(hint: light buffer, light count, inverse bias-projection)
	/*	// deferred shading
		//	- similar to light pre-pass but all at once on FSQ
		currentDemoProgram = demoState->prog_postDeferredShading;
		a3shaderProgramActivate(currentDemoProgram->program);
		a3textureActivate(demoState->tex_atlas_dm, a3tex_unit00); // diffuse texture atlas
		//...*/
		break;
	case ssfx_renderModePhongDL:
		// ****TO-DO:
		//	-> uncomment deferred lighting composite program and diffuse texture activations
		//	-> activate pertinent textures for deferred lighting composition
		//		(hint: all outputs from previous passes)
	/*	// deferred lighting composite
		//	- simple composition: multiply lighting colors by respective texture sample
		currentDemoProgram = demoState->prog_postDeferredLightingComposite;
		a3shaderProgramActivate(currentDemoProgram->program);
		a3textureActivate(demoState->tex_atlas_dm, a3tex_unit00); // diffuse texture atlas
		//...*/
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

	// optimized bloom algorithm: 
	//	-> render scene
	//	-> repeat a few times: 
	//		-> bright pass, half size
	//		-> blur in one direction (e.g. horizontal)
	//		-> blur in other direction (e.g. vertical)
	//	-> composite original scene result with final blur iteration results

	currentDemoProgram = demoState->prog_postBright;
	a3shaderProgramActivate(currentDemoProgram->program);
	a3framebufferBindColorTexture(currentWriteFBO, a3tex_unit00, 0);
	currentWriteFBO = writeFBO[ssfx_renderPassBright2];
	a3framebufferActivate(currentWriteFBO);
	a3vertexDrawableRenderActive();

	currentDemoProgram = demoState->prog_postBlur;
	a3shaderProgramActivate(currentDemoProgram->program);
	pixelSize.x = a3real_one / (a3real)currentWriteFBO->frameWidth;
	pixelSize.y = a3real_one / (a3real)currentWriteFBO->frameHeight;
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, pixelSize.v);
	a3framebufferBindColorTexture(currentWriteFBO, a3tex_unit00, 0);
	currentWriteFBO = writeFBO[ssfx_renderPassBlurH2];
	a3framebufferActivate(currentWriteFBO);
	a3vertexDrawableRenderActive();

	pixelSize.y = -pixelSize.y;
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, pixelSize.v);
	a3framebufferBindColorTexture(currentWriteFBO, a3tex_unit00, 0);
	currentWriteFBO = writeFBO[ssfx_renderPassBlurV2];
	a3framebufferActivate(currentWriteFBO);
	a3vertexDrawableRenderActive();

	currentDemoProgram = demoState->prog_postBright;
	a3shaderProgramActivate(currentDemoProgram->program);
	a3framebufferBindColorTexture(currentWriteFBO, a3tex_unit00, 0);
	currentWriteFBO = writeFBO[ssfx_renderPassBright4];
	a3framebufferActivate(currentWriteFBO);
	a3vertexDrawableRenderActive();

	currentDemoProgram = demoState->prog_postBlur;
	a3shaderProgramActivate(currentDemoProgram->program);
	pixelSize.x = a3real_one / (a3real)currentWriteFBO->frameWidth;
	pixelSize.y = a3real_one / (a3real)currentWriteFBO->frameHeight;
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, pixelSize.v);
	a3framebufferBindColorTexture(currentWriteFBO, a3tex_unit00, 0);
	currentWriteFBO = writeFBO[ssfx_renderPassBlurH4];
	a3framebufferActivate(currentWriteFBO);
	a3vertexDrawableRenderActive();

	pixelSize.y = -pixelSize.y;
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, pixelSize.v);
	a3framebufferBindColorTexture(currentWriteFBO, a3tex_unit00, 0);
	currentWriteFBO = writeFBO[ssfx_renderPassBlurV4];
	a3framebufferActivate(currentWriteFBO);
	a3vertexDrawableRenderActive();

	currentDemoProgram = demoState->prog_postBright;
	a3shaderProgramActivate(currentDemoProgram->program);
	a3framebufferBindColorTexture(currentWriteFBO, a3tex_unit00, 0);
	currentWriteFBO = writeFBO[ssfx_renderPassBright8];
	a3framebufferActivate(currentWriteFBO);
	a3vertexDrawableRenderActive();

	currentDemoProgram = demoState->prog_postBlur;
	a3shaderProgramActivate(currentDemoProgram->program);
	pixelSize.x = a3real_one / (a3real)currentWriteFBO->frameWidth;
	pixelSize.y = a3real_one / (a3real)currentWriteFBO->frameHeight;
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, pixelSize.v);
	a3framebufferBindColorTexture(currentWriteFBO, a3tex_unit00, 0);
	currentWriteFBO = writeFBO[ssfx_renderPassBlurH8];
	a3framebufferActivate(currentWriteFBO);
	a3vertexDrawableRenderActive();

	pixelSize.y = -pixelSize.y;
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, pixelSize.v);
	a3framebufferBindColorTexture(currentWriteFBO, a3tex_unit00, 0);
	currentWriteFBO = writeFBO[ssfx_renderPassBlurV8];
	a3framebufferActivate(currentWriteFBO);
	a3vertexDrawableRenderActive();

	currentDemoProgram = demoState->prog_postBlend;
	a3shaderProgramActivate(currentDemoProgram->program);
	a3framebufferBindColorTexture(writeFBO[ssfx_renderPassComposite], a3tex_unit00, 0);
	a3framebufferBindColorTexture(writeFBO[ssfx_renderPassBlurV2], a3tex_unit01, 0);
	a3framebufferBindColorTexture(writeFBO[ssfx_renderPassBlurV4], a3tex_unit02, 0);
	a3framebufferBindColorTexture(writeFBO[ssfx_renderPassBlurV8], a3tex_unit03, 0);
	currentWriteFBO = writeFBO[ssfx_renderPassDisplay];
	a3framebufferActivate(currentWriteFBO);
	a3vertexDrawableRenderActive();


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
	case ssfx_renderPassScene:
		if (currentDisplayFBO->color && (!currentDisplayFBO->depthStencil || renderTarget < renderTargetCount - 1))
			a3framebufferBindColorTexture(currentDisplayFBO, a3tex_unit00, renderTarget);
		else
			a3framebufferBindDepthTexture(currentDisplayFBO, a3tex_unit00);
		break;
	case ssfx_renderPassComposite:
	case ssfx_renderPassLights:
	case ssfx_renderPassBright2:
	case ssfx_renderPassBlurH2:
	case ssfx_renderPassBlurV2:
	case ssfx_renderPassBright4:
	case ssfx_renderPassBlurH4:
	case ssfx_renderPassBlurV4:
	case ssfx_renderPassBright8:
	case ssfx_renderPassBlurH8:
	case ssfx_renderPassBlurV8:
	case ssfx_renderPassDisplay:
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
			a3demo_drawModelSolidColor(modelViewProjectionMat.m, viewProjectionMat.m, a3mat4_identity.m, demoState->prog_drawColorUnif, demoState->draw_grid, blue);
		}

		if (demoState->displayTangentBases || demoState->displayWireframe)
		{
			const a3i32 flag[1] = { demoState->displayTangentBases * 3 + demoState->displayWireframe * 4 };
			const a3f32 size[1] = { 0.0625f };

			currentDemoProgram = demoState->prog_drawTangentBasis;
			a3shaderProgramActivate(currentDemoProgram->program);

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
				i = (j * 3) % hueCount;
				modelViewMat = currentSceneObject->modelMatrixStackPtr->modelViewMat;
				a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMV, 1, modelViewMat.mm);
				modelViewMat = currentSceneObject->modelMatrixStackPtr->modelViewMatInverseTranspose;
				a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMV_nrm, 1, modelViewMat.mm);
				modelViewProjectionMat = currentSceneObject->modelMatrixStackPtr->modelViewProjectionMat;
				a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);
				a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uIndex, 1, &i);
				a3vertexDrawableActivateAndRender(drawable[j]);
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
		for (i = 0; i < ssfxMaxCount_pointLight; ++i)
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
