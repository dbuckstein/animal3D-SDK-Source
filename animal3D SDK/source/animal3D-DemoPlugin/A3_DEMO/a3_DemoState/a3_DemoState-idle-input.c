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
	
	a3_DemoState_idle-input.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     HANDLING INPUT in this file.             ***
	****************************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"


//-----------------------------------------------------------------------------
// INPUT SUB-ROUTINES



//-----------------------------------------------------------------------------
// CALLBACKS

// ascii key callback
void a3demo_input_keyCharPress(a3_DemoState* demoState, a3i32 const asciiKey)
{
	switch (asciiKey)
	{
		// change pipeline mode
		//a3demoCtrlCasesLoop(demoState->demoMode, demoState_mode_max, '>', '<');
		//a3demoCtrlCasesLoop(demoState->demoMode, demoState_mode_max, '.', ',');

		// toggle grid
		a3demoCtrlCaseToggle(demoState->displayGrid, 'g');

		// toggle skybox
		a3demoCtrlCaseToggle(demoState->displaySkybox, 'b');

		// toggle hidden volumes
		a3demoCtrlCaseToggle(demoState->displayHiddenVolumes, 'h');

		// toggle world axes
		a3demoCtrlCaseToggle(demoState->displayWorldAxes, 'x');

		// toggle object axes
		a3demoCtrlCaseToggle(demoState->displayObjectAxes, 'z');

		// toggle tangent bases on vertices or other
		a3demoCtrlCaseToggle(demoState->displayTangentBases, 'B');

		// toggle wireframe overlay
		a3demoCtrlCaseToggle(demoState->displayWireframe, 'F');

		// update animation
		a3demoCtrlCaseToggle(demoState->updateAnimation, 'm');

		// toggle stencil test
		a3demoCtrlCaseToggle(demoState->stencilTest, 'i');

		// toggle stencil test
		a3demoCtrlCaseToggle(demoState->skipIntermediatePasses, 'I');
	}


	// update current mode pointer
	//demoState->demoModeCallbacksPtr = demoState->demoModeCallbacks + demoState->demoMode;

	// callbacks for current mode
	//if (demoState->demoModeCallbacksPtr->handleKeyPress)
	//	demoState->demoModeCallbacksPtr->handleKeyPress(demoState,
	//		demoState->demoModeCallbacksPtr->demoMode, asciiKey, 0);
	//if (demoState->demoModeCallbacksPtr->handleKeyHold)
	//	demoState->demoModeCallbacksPtr->handleKeyHold(demoState,
	//		demoState->demoModeCallbacksPtr->demoMode, asciiKey, 0);
}

// ascii key hold callback
void a3demo_input_keyCharHold(a3_DemoState* demoState, a3i32 asciiKey)
{
//	switch (asciiKey)
//	{
//
//	}


	// callback for current mode
	//if (demoState->demoModeCallbacksPtr->handleKeyHold)
	//	demoState->demoModeCallbacksPtr->handleKeyHold(demoState,
	//		demoState->demoModeCallbacksPtr->demoMode, asciiKey, 0);
}


//-----------------------------------------------------------------------------
// INPUT

void a3demo_input(a3_DemoState* demoState, a3f64 const dt)
{
	// callback for current mode
	//if (demoState->demoModeCallbacksPtr->handleInput)
	//	demoState->demoModeCallbacksPtr->handleInput(demoState,
	//		demoState->demoModeCallbacksPtr->demoMode, dt);
}


//-----------------------------------------------------------------------------
