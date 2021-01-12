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
	
	a3_DemoState_unloading.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     UNLOADING in this file.                  ***
	****************************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoState.h"

#include <stdio.h>


//-----------------------------------------------------------------------------
// UNLOADING
// this is where the union array style comes in handy; don't need a single 
//	release statement for each and every object... just iterate and release!

// utility to unload geometry
void a3demo_unloadGeometry(a3_DemoState* demoState)
{
	// ****TO-DO: 

}

// utility to unload shaders
void a3demo_unloadShaders(a3_DemoState* demoState)
{
	// ****TO-DO: 

}

// utility to unload textures
void a3demo_unloadTextures(a3_DemoState* demoState)
{
	// ****TO-DO: 

}

// utility to unload framebuffers
void a3demo_unloadFramebuffers(a3_DemoState* demoState)
{
	// ****LATER: 

}


//-----------------------------------------------------------------------------

// confirm that all graphics objects were unloaded
void a3demo_unloadValidate(const a3_DemoState* demoState)
{
	// ****LATER: 

}


//-----------------------------------------------------------------------------
