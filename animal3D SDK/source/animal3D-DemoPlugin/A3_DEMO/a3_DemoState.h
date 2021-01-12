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
	
	a3_DemoState.h
	Demo state interface and programmer function declarations.

	********************************************
	*** THIS IS YOUR DEMO'S MAIN HEADER FILE ***
	********************************************
*/

#ifndef __ANIMAL3D_DEMOSTATE_H
#define __ANIMAL3D_DEMOSTATE_H


//-----------------------------------------------------------------------------
// animal3D framework includes

#include "animal3D/animal3D.h"
#include "animal3D-A3DM/animal3D-A3DM.h"
#include "animal3D-A3DG/a3graphics/a3_TextRenderer.h"


//-----------------------------------------------------------------------------
// other demo includes



//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct	a3_DemoState					a3_DemoState;
typedef struct	a3_DemoModeCallbacks			a3_DemoModeCallbacks;
typedef enum	a3_DemoState_ModeName			a3_DemoState_ModeName;
typedef enum	a3_DemoState_TextDisplayName	a3_DemoState_TextDisplayName;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// demo mode names
enum a3_DemoState_ModeName
{
	demoState_mode_max
};


// text mode names
enum a3_DemoState_TextDisplayName
{
	demoState_textDisabled,			// no text overlay
	demoState_textControls,			// display controls
	demoState_textControls_gen,		// display general controls
	demoState_textData,				// display data

	demoState_text_max
};


// object maximum counts for easy array storage
// good idea to make these numbers greater than what you actually need 
//	and if you end up needing more just increase the count... there's 
//	more than enough memory to hold extra objects
enum a3_DemoState_ObjectMaxCount
{
	demoStateMaxCount_timer = 1,
};

	
//-----------------------------------------------------------------------------

// callback types
struct a3_DemoState;
typedef void (*a3_DemoMode_EventCallbackConst)(a3_DemoState const* demoState, void const* demoMode, a3f64 const dt);
typedef void (*a3_DemoMode_EventCallback)(a3_DemoState* demoState, void* demoMode, a3f64 const dt);
typedef void (*a3_DemoMode_InputCallback)(a3_DemoState const* demoState, void* demoMode, a3i32 const button, a3i32 const state);

// callbacks pointers for each mode
struct a3_DemoModeCallbacks
{
	void* demoMode;
	a3_DemoMode_EventCallbackConst handleRender;
	a3_DemoMode_EventCallback handleInput, handleUpdate;
	a3_DemoMode_InputCallback handleKeyPress, handleKeyHold;
};


// persistent demo state data structure
struct a3_DemoState
{
	//-------------------------------------------------------------------------
	// general variables pertinent to the state

	// terminate key pressed
	a3i32 exitFlag;

	// asset streaming between loads enabled (careful!)
	a3boolean streaming;

	// window and full-frame dimensions
	a3ui32 windowWidth, windowHeight;
	a3real windowWidthInv, windowHeightInv, windowAspect;
	a3ui32 frameWidth, frameHeight;
	a3real frameWidthInv, frameHeightInv, frameAspect;
	a3i32 frameBorder;


	//-------------------------------------------------------------------------
	// objects that have known or fixed instance count in the whole demo

	// text renderer
	a3boolean textInit;
	a3_DemoState_TextDisplayName textMode;
	a3_TextRenderer text[2];

	// input
	a3_MouseInput mouse[1];
	a3_KeyboardInput keyboard[1];
	a3_XboxControllerInput xcontrol[4];

	// pointer to fast trig table
	a3f32 trigTable[4096 * 4];

	// more accurate time tracking
	a3f64 t_timer, dt_timer, dt_timer_tot;
	a3i64 n_timer;


	//-------------------------------------------------------------------------
	// scene variables and objects

	// demo modes
	a3_DemoState_ModeName demoMode;
	//a3_DemoModeCallbacks demoModeCallbacks[demoState_mode_max];
	a3_DemoModeCallbacks const* demoModeCallbacksPtr;
	void* demoModePtr;

	// toggle grid in scene and axes superimposed, as well as other mods
	a3boolean displayGrid, displaySkybox, displayHiddenVolumes;
	a3boolean displayWorldAxes, displayObjectAxes;
	a3boolean displayTangentBases, displayWireframe;
	a3boolean updateAnimation;
	a3boolean stencilTest;
	a3boolean skipIntermediatePasses;


	//-------------------------------------------------------------------------
	// object arrays: organized as anonymous unions for two reasons: 
	//	1. easy to manage entire sets of the same type of object using the 
	//		array component
	//	2. at the same time, variables are named pointers


	// timers
	union {
		a3_Timer timer[demoStateMaxCount_timer];
		struct {
			a3_Timer
				timer_display[1];						// render FPS timer
		};
	};


	//-------------------------------------------------------------------------
	// the end
};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMOSTATE_H