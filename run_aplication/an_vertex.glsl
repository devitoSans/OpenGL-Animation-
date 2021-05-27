#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;

//define
#define MAX_BONES 100
#define MAX_BONE_INFLUENCE 4

//in out shader
out VS_OUT {
	vec2 texCoord;
	vec3 normal;
	vec3 fragPos;
} vs_out;

//uniform
uniform mat4 PCM;
uniform mat4 model;
uniform mat4 modelCPP;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main() {
	vec4 totalPosition = vec4(0.f);
	vec3 localNormal;
	vec3 localFragPos;
	for(int i = 0; i < MAX_BONE_INFLUENCE; i++) {
		if(boneIds[i] == -1) 
			continue;
		if(boneIds[i] >= MAX_BONES) {
			totalPosition = vec4(vPosition, 1.f);
			break;
		}
		vec4 localPosition = modelCPP * finalBonesMatrices[boneIds[i]] * vec4(vPosition, 1.f);
		totalPosition += localPosition * weights[i];
		localNormal = mat3(model * modelCPP * finalBonesMatrices[boneIds[i]]) * vNormal;
		localFragPos = vec3(model * localPosition);
	}

	vs_out.texCoord = vTexCoord;
	vs_out.normal = localNormal;
	vs_out.fragPos = localFragPos;

	gl_Position = PCM * totalPosition;
}
