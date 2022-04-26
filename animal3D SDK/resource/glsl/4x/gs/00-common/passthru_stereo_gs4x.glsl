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
	
	passthru_stereo_gs4x.glsl
	Duplicate triangle to be drawn in stereo.
*/

#version 450

#define MAX_VERTICES 6

layout (triangles) in;

layout (triangle_strip, max_vertices = MAX_VERTICES) out;

in vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
} vVertexData[];

out vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
};

void copyVertexData(int i)
{
	vTangentBasis_view = vVertexData[i].vTangentBasis_view;
	vTexcoord_atlas = vVertexData[i].vTexcoord_atlas;
}

void drawMono()
{
	copyVertexData(0);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	copyVertexData(1);
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	
	copyVertexData(2);
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}

// ****TO-DO: 
//	-> declare appropriate stereo projection matrix uniform
//	-> implement method called 'drawStereo' and invoke it in 'main' below: 
//		-> calculate 'gl_Position' for each vertex for one eye
//		-> repeat the algorithm for the other eye
//		-> refer to the OpenGL and GLSL specifications: 
//		https://www.khronos.org/registry/OpenGL/specs/gl/glspec46.core.pdf
//			11.3.4.6 "Layer and Viewport Selection"
//		https://www.khronos.org/registry/OpenGL/specs/gl/GLSLangSpec.4.60.pdf 
//			7.1.4 "Geometry Shader Special Variables"
//
// | YOUR CODE HERE |
// V                V
//-----------------------------------------------------------------------------


void drawStereo()
{
	
}
//-----------------------------------------------------------------------------
// ^ YOUR CODE HERE ^


void main()
{
	drawMono();
	//drawStereo();
}
