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

	a3_DemoMode4_Animate-unload.c
	Demo mode implementations: animation scene.

	********************************************
	*** UNLOADING FOR ANIMATION SCENE MODE   ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode4_Animate.h"

typedef struct a3_DemoState a3_DemoState;
//#include "../a3_DemoState.h"


//-----------------------------------------------------------------------------

void a3animate_unload(a3_DemoState const* demoState, a3_DemoMode4_Animate* demoMode)
{
	// release scene hierarchy
	a3hierarchyRelease(demoMode->hierarchy_scene);

	// release skeleton hierarchy
	a3hierarchyRelease(demoMode->hierarchy_skel);
}


void a3animate_unloadValidate(a3_DemoState const* demoState, a3_DemoMode4_Animate* demoMode)
{

}


//-----------------------------------------------------------------------------
