/*
	Copyright 2011-2025 Daniel S. Buckstein

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
	
	drawTangentBases_gs4x.glsl
	Draw tangent bases of vertices and/or faces, and/or wireframe shapes, 
		determined by flag passed to program.
*/

#version 450

// (2 verts/axis * 3 axes/basis * (3 vertex bases + 1 face basis) + 4 to 8 wireframe verts = 28 to 32 verts)
#define MAX_VERTICES 32
#define MAX_COLORS 24

layout (triangles) in;

layout (line_strip, max_vertices = MAX_VERTICES) out;

in vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
} vVertexData[];

uniform mat4 uP;

uniform int uIndex;
uniform int uFlag;
uniform float uSize;

uniform vec4 uColor0[MAX_COLORS];

out vec4 vColor;


void drawWireframe(in vec4 color, in vec4 p0, in vec4 p1, in vec4 p2);
void calcTangentBasis(out mat4 tangentBasis, in vec4 p0, in vec4 p1, in vec4 p2, in vec4 c0, in vec4 c1, in vec4 c2);
void drawTangentBasis(in mat4 tangentBasis, in vec4 clipPos, in float sz);


void main()
{
	if ((uFlag & 1) > 0)
	{
		drawTangentBasis(vVertexData[0].vTangentBasis_view, gl_in[0].gl_Position, uSize);
		drawTangentBasis(vVertexData[1].vTangentBasis_view, gl_in[1].gl_Position, uSize);
		drawTangentBasis(vVertexData[2].vTangentBasis_view, gl_in[2].gl_Position, uSize);
	}
	if ((uFlag & 2) > 0)
	{
		mat4 tangentBasis_face;
		calcTangentBasis(tangentBasis_face,
			vVertexData[0].vTangentBasis_view[3],
			vVertexData[1].vTangentBasis_view[3],
			vVertexData[2].vTangentBasis_view[3],
			vVertexData[0].vTexcoord_atlas,
			vVertexData[1].vTexcoord_atlas,
			vVertexData[2].vTexcoord_atlas);
		drawTangentBasis(tangentBasis_face, uP * tangentBasis_face[3], uSize);
	}
	if ((uFlag & 4) > 0)
	{
		vec4 n0 = (uSize * uSize) * normalize(vVertexData[0].vTangentBasis_view[2]);
		vec4 n1 = (uSize * uSize) * normalize(vVertexData[1].vTangentBasis_view[2]);
		vec4 n2 = (uSize * uSize) * normalize(vVertexData[2].vTangentBasis_view[2]);
		drawWireframe(uColor0[uIndex],
			vVertexData[0].vTangentBasis_view[3] + n0,
			vVertexData[1].vTangentBasis_view[3] + n1,
			vVertexData[2].vTangentBasis_view[3] + n2);
		drawWireframe(uColor0[uIndex],
			vVertexData[0].vTangentBasis_view[3] - n0,
			vVertexData[1].vTangentBasis_view[3] - n1,
			vVertexData[2].vTangentBasis_view[3] - n2);
	}
}
