#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 2) in vec2 vertex_texCoord;

//in out shader
out VS_OUT {
	vec2 texCoord;
} vs_out;

//uniform
uniform mat4 PCM;

void main() {
	vs_out.texCoord = vertex_texCoord;

	//final
	gl_Position = PCM * vec4(vertex_position, 1.f);
}
