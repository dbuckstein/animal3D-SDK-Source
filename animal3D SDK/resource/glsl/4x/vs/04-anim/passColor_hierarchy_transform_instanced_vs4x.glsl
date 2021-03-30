/*
	Copyright 2011-2020 Daniel S. Buckstein

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
	
	passColor_hierarchy_transform_instanced_vs4x.glsl
	Transform position attribute for instance and pass hierarchical color.
*/

#version 450

#define MAX_INSTANCES 128

#define MAX_COLORS 24

layout (location = 0) in vec4 aPosition;

uniform ubTransformMVP {
	mat4 uMVP[MAX_INSTANCES];
};

uniform vec4 uColor0[MAX_COLORS];

out vec4 vColor;

flat out int vVertexID;
flat out int vInstanceID;

void main()
{
	// DUMMY OUTPUT: directly assign input position to output position
//	gl_Position = aPosition;
	gl_Position = uMVP[gl_InstanceID] * aPosition;
	
	vColor = uColor0[gl_InstanceID];

	vVertexID = gl_VertexID;
	vInstanceID = gl_InstanceID;
}
