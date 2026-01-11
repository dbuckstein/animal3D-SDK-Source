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
	
	utilCommon_gs4x.glsl
	Common utilities for geometry shaders.
*/

void drawWireframe(in vec4 color, in vec4 p0, in vec4 p1, in vec4 p2)
{
	vColor = color;

	gl_Position = uP * p0;
	EmitVertex();

	gl_Position = uP * p1;
	EmitVertex();

	gl_Position = uP * p2;
	EmitVertex();

	gl_Position = uP * p0;
	EmitVertex();

	EndPrimitive();
}

void calcTangentBasis(out mat4 tangentBasis, in vec4 p0, in vec4 p1, in vec4 p2, in vec4 c0, in vec4 c1, in vec4 c2)
{
	vec4 dp1 = p1 - p0;
	vec4 dp2 = p2 - p0;
	vec4 dc1 = c1 - c0;
	vec4 dc2 = c2 - c0;

	//float detInv = 1.0 / (dc1.x * dc2.y - dc1.y * dc2.x);
	vec4 t = (dp1 * dc2.y - dc1.y * dp2); // * detInv;
	vec4 b = (dp2 * dc1.x - dc2.x * dp1); // * detInv;
	vec4 n = vec4(cross(dp1.xyz, dp2.xyz), 0.0);
	vec4 p = (p0 + p1 + p2) / 3.0;

	n = normalize(n);
	b = normalize(b - dot(b, n) * n);
	t = normalize(t - dot(t, n) * n - dot(t, b) * b);

	tangentBasis = mat4(t, b, n, p);
}

void drawTangentBasis(in mat4 tangentBasis, in vec4 clipPos, in float sz)
{
	vColor = vec4(1.0, 0.0, 0.0, 1.0);
	gl_Position = clipPos;
	EmitVertex();
	gl_Position = uP * (tangentBasis[3] + normalize(tangentBasis[0]) * sz);
	EmitVertex();
	EndPrimitive();
	
	vColor = vec4(0.0, 1.0, 0.0, 1.0);
	gl_Position = clipPos;
	EmitVertex();
	gl_Position = uP * (tangentBasis[3] + normalize(tangentBasis[1]) * sz);
	EmitVertex();
	EndPrimitive();
	
	vColor = vec4(0.0, 0.0, 1.0, 1.0);
	gl_Position = clipPos;
	EmitVertex();
	gl_Position = uP * (tangentBasis[3] + normalize(tangentBasis[2]) * sz);
	EmitVertex();
	EndPrimitive();
}
