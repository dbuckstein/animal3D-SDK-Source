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
	
	utilCommon_fs4x.glsl
	Common utilities for fragment shaders.
*/


#define iVal		0
#define iValSq 		1
#define iValInv 	2
#define iValInvSq	3


float lenSq(float x)
{
	return (x * x);
}

float lenSq(vec2 x)
{
	return dot(x, x);
}

float lenSq(vec3 x)
{
	return dot(x, x);
}

float lenSq(vec4 x)
{
	return dot(x, x);
}

float pow64(float x)
{
	x *= x;	// x^ 2
	x *= x;	// x^ 4
	x *= x;	// x^ 8
	x *= x;	// x^16
	x *= x;	// x^32
	x *= x;	// x^64
	return x;
}

float attenuation(in float dist, in float distSq, in float lightRadiusInv, in float lightRadiusInvSq)
{
	float x = 1.0 + 2.0 * dist * lightRadiusInv + distSq * lightRadiusInvSq;
	return (1.0 / x);
}

float attenuation(in float dist, in float distSq, in vec4 lightRadiusInfo)
{
	return attenuation(dist, distSq, lightRadiusInfo[iValInv], lightRadiusInfo[iValInvSq]);
}

vec3 refl(in float kd, in vec3 n, in vec3 v)
{
	return ((2.0 * kd) * n - v);
}

vec4 refl(in float kd, in vec4 n, in vec4 v)
{
	return ((2.0 * kd) * n - v);
}

void Lambert(out float kd, in vec4 n, in vec4 vl)
{
	kd = max(0.0, dot(n, vl)) * 0.9 + 0.1;
}

void Phong(out float kd, out float ks, out vec4 vr, in vec4 n, in vec4 vl, in vec4 ve)
{
	kd = dot(n, vl);
	vr = refl(kd, n, vl);
	kd = max(0.0, kd) * 0.9 + 0.1;
	ks = dot(vr, ve);
	ks = max(0.0, ks);
	ks = pow64(ks);
}


void calcLambertPoint(out vec4 diffuseColor,
	in vec4 fragPos, in vec4 fragNrm, in vec4 fragColor,
	in vec4 lightPos, in vec4 lightRadiusInfo, in vec4 lightColor)
{
	vec4 lightVec = lightPos - fragPos;
	float kd, distSq = lenSq(lightVec), dist = sqrt(distSq);
	Lambert(kd, fragNrm, lightVec / dist);
	
	vec4 color = fragColor * lightColor * attenuation(dist, distSq, lightRadiusInfo);
	diffuseColor = color * kd;
}

void calcPhongPoint(out vec4 diffuseColor, out vec4 specularColor, in vec4 eyeVec,
	in vec4 fragPos, in vec4 fragNrm, in vec4 fragColor,
	in vec4 lightPos, in vec4 lightRadiusInfo, in vec4 lightColor)
{
	vec4 lightVec = lightPos - fragPos, vr;
	float kd, ks, distSq = lenSq(lightVec), dist = sqrt(distSq);
	Phong(kd, ks, vr, fragNrm, lightVec / dist, eyeVec);
	
	vec4 color = fragColor * lightColor * attenuation(dist, distSq, lightRadiusInfo);
	diffuseColor = color * kd;
	specularColor = color * ks;
}


void initDefaultPoint(out sPointLight light)
{
	const float r = 256.0, rSq = r * r, rInv = 1.0 / r, rSqInv = 1.0 / rSq;
	const vec4 c = vec4(1.0);
	const vec4 p = vec4(1.0);
	light = sPointLight(p, p, c, vec4(r, rSq, rInv, rSqInv));
}
