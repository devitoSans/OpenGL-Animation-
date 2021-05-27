#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texCoord;

//in out shader
out VS_OUT {
	vec2 texCoord;
	vec3 normal;
	vec3 fragPos;
} vs_out;

//uniform
layout (std140) uniform Matrices {
	mat4 PC;
};
uniform mat4 model;

void main() {
	//properties
	vs_out.texCoord = vertex_texCoord;
	vs_out.normal = mat3(transpose(inverse(model))) * vertex_normal;
	vs_out.fragPos = vec3(model * vec4(vertex_position, 1.f)); 

	//final
	gl_Position = PC * model * vec4(vertex_position, 1.f);
}
