#version 330 core

layout (location = 0) in vec3 vertex_position;

//in out shader
out vec3 position;

//uniform
uniform mat4 PCM;

void main() {
	position = vertex_position;

	//final
	vec4 position = PCM * vec4(vertex_position, 1.f);
	gl_Position = position.xyww;
}
