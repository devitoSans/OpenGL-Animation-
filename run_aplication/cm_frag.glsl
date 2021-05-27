#version 330 core

//resulting
out vec4 fragColor;

//in out shader
in vec3 position;

//uniform
uniform samplerCube cubemapTexture;

void main() {
	//result
	fragColor = texture(cubemapTexture, position);
}